#include "ui_menu_hire.hpp"
#include "../audio.hpp"
#include "../calc.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"

namespace elona
{
namespace ui
{

bool UIMenuHire::_should_display_chara(const character& chara)
{
    if (_operation == HireOperation::revive)
    {
        if (chara.state() != character::State::pet_dead
            && chara.state() != character::State::villager_dead)
        {
            return false;
        }
    }
    else
    {
        if (_operation == HireOperation::hire)
        {
            if (chara.state() != character::State::servant_being_selected)
            {
                return false;
            }
        }
        else if (chara.state() != character::State::alive)
        {
            return false;
        }
        if (chara.index < 16)
        {
            if (chara.current_map != gdata_current_map)
            {
                return false;
            }
        }
    }
    if (chara.index == 0)
    {
        return false;
    }
    if (chara.is_escorted_in_sub_quest() == 1)
    {
        return false;
    }

    return true;
}

void UIMenuHire::_populate_list()
{
    for (auto&& cnt : cdata.all())
    {
        if (_should_display_chara(cnt))
        {
            list(0, listmax) = cnt.index;
            list(1, listmax) = -cnt.level;
            ++listmax;
        }
    }
    sort_list_by_column1();
}

bool UIMenuHire::init()
{
    snd(26);
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    windowshadow = 1;

    _populate_list();

    return true;
}

void UIMenuHire::update()
{
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
}

void UIMenuHire::_draw_topic()
{
    if (_operation == HireOperation::move)
    {
        s = i18n::s.get("core.locale.ui.npc_list.wage");
    }
    else
    {
        s = i18n::s.get("core.locale.ui.npc_list.init_cost");
    }
    if (_operation != HireOperation::revive)
    {
        display_topic(s, wx + 490, wy + 36);
    }
}

void UIMenuHire::_draw_window()
{
    s(0) = i18n::s.get("core.locale.ui.npc_list.title");
    s(1) = strhint2 + strhint3;
    display_window((windoww - 700) / 2 + inf_screenx, winposy(448), 700, 448);
    display_topic(
        i18n::s.get("core.locale.ui.npc_list.name"), wx + 28, wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.npc_list.info"), wx + 350, wy + 36);

    _draw_topic();
}

static void _draw_key(int cnt)
{
    if (cnt % 2 == 0)
    {
        boxf(wx + 70, wy + 66 + cnt * 19, 600, 18, {12, 14, 16, 16});
    }
    display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
}

static void _draw_keys()
{
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        _draw_key(cnt);
    }
}

static void _draw_list_entry_pic(int cnt, const character& chara)
{
    draw_chara_scale_height(chara, wx + 40, wy + 74 + cnt * 19 - 8);
}

static void _draw_list_entry_name(int cnt, const character& chara)
{
    std::string chara_name = cdatan(0, chara.index);
    cutname(chara_name, 36);

    cs_list(cs == cnt, chara_name, wx + 84, wy + 66 + cnt * 19 - 1);
}

static void _draw_list_entry_info(int cnt, const character& chara)
{
    std::string level = u8"Lv."s + chara.level + u8" "s;

    if (chara.sex == 0)
    {
        level += cnven(i18n::_(u8"ui", u8"male"));
    }
    else
    {
        level += cnven(i18n::_(u8"ui", u8"female"));
    }

    level += i18n::s.get(
        "core.locale.ui.npc_list.age_counter", calcage(chara.index));

    pos(wx + 372, wy + 66 + cnt * 19 + 2);
    mes(level);
}

void UIMenuHire::_draw_list_entry_cost(int cnt, const character& chara)
{
    if (_operation != HireOperation::revive)
    {
        std::string text;
        int cost = calchirecost(chara.index);

        if (_operation == HireOperation::hire)
        {
            text = ""s + (cost * 20) + u8"("s + cost + u8")"s;
        }
        else
        {
            text = ""s + cost;
        }

        pos(wx + 512, wy + 66 + cnt * 19 + 2);
        mes(i18n::s.get("core.locale.ui.npc_list.gold_counter", text));
    }
}


void UIMenuHire::_draw_list_entry(int cnt, const character& chara)
{
    _draw_list_entry_pic(cnt, chara);
    _draw_list_entry_name(cnt, chara);
    _draw_list_entry_info(cnt, chara);
    _draw_list_entry_cost(cnt, chara);
}

void UIMenuHire::_draw_list_entries()
{
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }

        int chara_index = list(0, p);
        const character& chara = cdata[chara_index];

        _draw_list_entry(cnt, chara);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}

void UIMenuHire::draw()
{
    _draw_window();
    _draw_keys();
    _draw_list_entries();
}

optional<UIMenuHire::result_type> UIMenuHire::on_key(const std::string& key)
{
    int _p;

    ELONA_GET_SELECTED_ITEM(_p, 0);

    if (_p != -1)
    {
        return UIMenuHire::result::finish(_p);
    }
    else if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            set_reupdate();
        }
    }
    else if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            set_reupdate();
        }
    }
    else if (key == key_cancel)
    {
        return UIMenuHire::result::cancel();
    }

    return none;
}

} // namespace ui
} // namespace elona
