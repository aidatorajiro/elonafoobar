#include "net.hpp"
#include <chrono>
#include <sstream>
#include "../spider/http.hpp"
#include "../thirdparty/nlohmannjson/json.hpp"
#include "../thirdparty/xxHash/xxhashcpp.hpp"
#include "../util/scope_guard.hpp"
#include "config/config.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "message.hpp"
#include "text.hpp"
#include "variables.hpp"

using namespace std::literals::chrono_literals;
using namespace nlohmann;
using namespace spider::http;



namespace elona
{

namespace
{

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;


constexpr size_t ChatKind_size = static_cast<size_t>(ChatKind::_size);

std::array<TimePoint, ChatKind_size> last_chat_sent_at;

constexpr std::array<Duration, ChatKind_size> chat_sending_interval{{
    5min, // chat
    2min, // death
    1h, // wish
    5min, // news
}};

TimePoint last_chat_received_at;

constexpr const char* schema = "https";
constexpr const char* host = "cv.elonafoobar.com";
constexpr const char* version = "v1";

const std::string chat_url =
    std::string(schema) + "://" + host + "/" + version + "/chats/";
const std::string poll_url =
    std::string(schema) + "://" + host + "/" + version + "/polls/";
const std::string vote_url =
    std::string(schema) + "://" + host + "/" + version + "/votes/";

int last_received_chat_id = -1;



Headers common_headers_get{{"Connection", "close"},
                           {"User-Agent", latest_version.user_agent()}};
Headers common_headers_post{{"Connection", "close"},
                            {"User-Agent", latest_version.user_agent()},
                            {"Content-Type", "application/json"}};



Duration chat_receive_interval()
{
    return std::chrono::minutes{
        Config::instance().get<int>("core.net.chat_receive_interval")};
}



bool can_send_chat_now(ChatKind kind)
{
    const auto idx = static_cast<size_t>(kind);
    return chat_sending_interval[idx] < Clock::now() - last_chat_sent_at[idx];
}



std::string replace_f_word(std::string str)
{
    // same as vanilla.
    str = strutil::replace(str, u8"fucking", u8"nyoro~n");
    str = strutil::replace(str, u8"fuck", u8"nyou talk funny");
    return str;
}



void send_chat(ChatKind kind, const std::string& message)
{
    if (!can_send_chat_now(kind))
    {
        return;
    }

    json payload;
    payload["kind"] = static_cast<int>(kind);
    payload["message"] = replace_f_word(message);

    Request req{
        Verb::POST, chat_url, common_headers_post, Body{payload.dump()}};
    req.send(
        [](const auto& response) {
            if (response.status / 100 != 2)
            {
                txt(i18n::s.get("core.net.failed_to_send"));
                ELONA_WARN("net.post") << chat_url << " " << response.status
                                       << " " << response.body;
                return;
            }
        },
        [](const auto& error) {
            txt(i18n::s.get("core.net.failed_to_send"));
            ELONA_WARN(error.what());
        });

    last_chat_sent_at[static_cast<size_t>(kind)] = Clock::now();
}



template <typename Seed, typename F>
auto eval_with_random_seed(Seed seed, F func)
{
    randomize(seed);
    lib::scope_guard reset_seed{[]() { randomize(); }};
    return func();
}



std::string get_pc_name()
{
    if (Config::instance().get<bool>("core.net.hide_your_name"))
    {
        const auto seed = xxhash::xxhash32(cdatan(0, 0));
        return eval_with_random_seed(seed, []() { return random_name(); });
    }
    else
    {
        return cdatan(0, 0);
    }
}



std::string get_pc_alias()
{
    if (Config::instance().get<bool>("core.net.hide_your_alias"))
    {
        const auto seed = xxhash::xxhash32(cdatan(1, 0));
        return eval_with_random_seed(
            seed, []() { return random_title(RandomTitleType::character); });
    }
    else
    {
        return cdatan(1, 0);
    }
}



std::string get_date()
{
    // E.g., 517/08/14
    const auto y = game_data.date.year;
    const auto m = game_data.date.month;
    const auto d = game_data.date.day;
    std::stringstream ss;
    ss.width(2);
    ss << y << '/' << m << '/' << d;
    return ss.str();
}

} // namespace



std::vector<ChatData> net_receive_chats(bool skip_old_chat)
{
    if (!Config::instance().net)
    {
        return {};
    }
    if (skip_old_chat &&
        Clock::now() - last_chat_received_at < chat_receive_interval())
    {
        return {};
    }

    bool done = false;
    std::vector<ChatData> chats;

    Request req{Verb::GET, chat_url, common_headers_get};
    req.send(
        [&](const auto& response) {
            if (response.status / 100 != 2)
            {
                txt(i18n::s.get("core.net.failed_to_receive"));
                ELONA_WARN("net.get") << chat_url << " " << response.status
                                      << " " << response.body;
                done = true;
                return;
            }

            const auto chats_json = json::parse(response.body);
            for (const json& chat_json : chats_json)
            {
                auto kind = chat_json["kind"].get<int>();
                auto id = chat_json["id"].get<int>();
                if (id <= last_received_chat_id)
                {
                    if (skip_old_chat)
                    {
                        continue;
                    }
                }
                else
                {
                    last_received_chat_id = id;
                }

                switch (static_cast<ChatKind>(kind))
                {
                case ChatKind::chat:
                    if (Config::instance().get<std::string>("core.net.chat") ==
                        "disabled")
                    {
                        continue;
                    }
                case ChatKind::death:
                    if (Config::instance().get<std::string>("core.net.death") ==
                        "disabled")
                    {
                        continue;
                    }
                case ChatKind::wish:
                    if (Config::instance().get<std::string>("core.net.wish") ==
                        "disabled")
                    {
                        continue;
                    }
                case ChatKind::news:
                    if (Config::instance().get<std::string>("core.net.news") ==
                        "disabled")
                    {
                        continue;
                    }
                default: break;
                }

                chats.emplace_back(
                    id,
                    static_cast<ChatKind>(kind),
                    chat_json["created_at"].get<std::string>(),
                    chat_json["message"].get<std::string>());
            }
            done = true;
        },
        [&](const auto& error) {
            txt(i18n::s.get("core.net.failed_to_receive"));
            ELONA_WARN(error.what());
            done = true;
        });

    while (!done)
    {
        // TODO: show loading cycle.
        await(10);
    }

    last_chat_received_at = Clock::now();
    return chats;
}



std::vector<PollData> net_receive_polls()
{
    if (!Config::instance().net)
    {
        return {};
    }
    if (!Config::instance().get<bool>("core.net.is_alias_vote_enabled"))
    {
        return {};
    }

    bool done = false;
    std::vector<PollData> polls;

    Request req{Verb::GET, poll_url, common_headers_get};
    req.send(
        [&](const auto& response) {
            if (response.status / 100 != 2)
            {
                txt(i18n::s.get("core.net.failed_to_receive"));
                ELONA_WARN("net.get") << poll_url << " " << response.status
                                      << " " << response.body;
                done = true;
                return;
            }

            const auto polls_json = json::parse(response.body);
            for (const json& poll_json : polls_json)
            {
                polls.emplace_back(
                    poll_json["id"].get<int>(),
                    poll_json["name"].get<std::string>(),
                    poll_json["vote_count"].get<int>());
            }
            done = true;
        },
        [&](const auto& error) {
            txt(i18n::s.get("core.net.failed_to_receive"));
            ELONA_WARN(error.what());
            done = true;
        });

    while (!done)
    {
        // TODO: show loading cycle.
        await(10);
    }

    return polls;
}



void net_send_chat(const std::string& message)
{
    if (!Config::instance().net)
    {
        return;
    }
    if (Config::instance().get<std::string>("core.net.chat") != "send_receive")
    {
        return;
    }

    send_chat(
        ChatKind::chat,
        i18n::s.get(
            "core.net.chat.sent_message",
            get_pc_alias(),
            get_pc_name(),
            message));
}



void net_send_death(
    const std::string& cause,
    const std::string& map,
    const std::string& last_words)
{
    if (!Config::instance().net)
    {
        return;
    }
    if (Config::instance().get<std::string>("core.net.death") != "send_receive")
    {
        return;
    }
    if (game_data.wizard)
    {
        return;
    }

    send_chat(
        ChatKind::death,
        i18n::s.get(
            "core.net.death.sent_message",
            get_pc_alias(),
            get_pc_name(),
            cause,
            map,
            last_words));
}



void net_send_wish(const std::string& input, const std::string& result)
{
    if (!Config::instance().net)
    {
        return;
    }
    if (Config::instance().get<std::string>("core.net.wish") != "send_receive")
    {
        return;
    }
    if (game_data.wizard)
    {
        return;
    }

    send_chat(
        ChatKind::wish,
        i18n::s.get(
            "core.net.wish.sent_message",
            get_pc_alias(),
            get_pc_name(),
            input,
            result));
}



void net_send_news(const std::string& locale_id, const std::string& extra_info)
{
    if (!Config::instance().net)
    {
        return;
    }
    if (Config::instance().get<std::string>("core.net.news") != "send_receive")
    {
        return;
    }
    if (game_data.wizard)
    {
        return;
    }

    send_chat(
        ChatKind::news,
        i18n::s.get(
            "core.net.news." + locale_id,
            get_date(),
            get_pc_alias(),
            get_pc_name(),
            extra_info));
}



void net_register_your_name()
{
    if (!Config::instance().net)
    {
        return;
    }
    if (!Config::instance().get<bool>("core.net.is_alias_vote_enabled"))
    {
        return;
    }

    json payload;
    payload["name"] = get_pc_alias() + i18n::space_if_needed() + get_pc_name();

    Request req{
        Verb::POST, poll_url, common_headers_post, Body{payload.dump()}};
    req.send(
        [](const auto& response) {
            if (response.status / 100 != 2)
            {
                txt(i18n::s.get("core.net.failed_to_send"));
                ELONA_WARN("net.post") << poll_url << " " << response.status
                                       << " " << response.body;
                return;
            }
        },
        [](const auto& error) {
            txt(i18n::s.get("core.net.failed_to_send"));
            ELONA_WARN(error.what());
        });
}



void net_send_vote(int poll_id)
{
    if (!Config::instance().net)
    {
        return;
    }
    if (!Config::instance().get<bool>("core.net.is_alias_vote_enabled"))
    {
        return;
    }

    json payload;
    payload["poll"] = poll_id;

    Request req{
        Verb::POST, vote_url, common_headers_post, Body{payload.dump()}};
    req.send(
        [](const auto& response) {
            if (response.status / 100 != 2)
            {
                txt(i18n::s.get("core.net.failed_to_send"));
                ELONA_WARN("net.post") << vote_url << " " << response.status
                                       << " " << response.body;
                return;
            }
        },
        [](const auto& error) {
            txt(i18n::s.get("core.net.failed_to_send"));
            ELONA_WARN(error.what());
        });
}



bool net_can_send_chat_now()
{
    return can_send_chat_now(ChatKind::chat);
}



std::string ChatData::as_log() const
{
    return message;
}



std::string ChatData::as_chat_history() const
{
    // TODO created_at
    return created_at + message;
}

} // namespace elona
