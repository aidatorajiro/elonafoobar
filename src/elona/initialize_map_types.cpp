#include "area.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "ctrl_file.hpp"
#include "data/types/type_map.hpp"
#include "event.hpp"
#include "gdata.hpp"
#include "i18n.hpp"
#include "itemgen.hpp"
#include "lua_env/lua_class/lua_class_map_generator.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "mapgen.hpp"
#include "quest.hpp"
#include "text.hpp"
#include "variables.hpp"

namespace elona
{

static void _init_map_shelter()
{
    if (game_data.current_dungeon_level == 1)
    {
        map_initcustom(u8"shelter_2"s);
        map_data.refresh_type = 0;
        map_data.type = static_cast<int>(mdata_t::MapType::shelter);
    }
    else
    {
        map_initcustom(u8"shelter_1"s);
        map_data.user_map_flag = 0;
    }
    map_data.max_crowd_density = 0;
    map_data.max_item_count = 5;
    map_placeplayer();
    map_data.bgm = 68;
}

static void _init_map_nefia()
{
    generate_random_nefia();
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        event_add(4);
    }
}

static void _init_map_museum()
{
    map_initcustom(u8"museum_1"s);
    map_data.bgm = 53;
    map_placeplayer();
    map_data.user_map_flag = 0;
    flt();
    itemcreate(-1, 24, 15, 17, 0);
    inv[ci].param1 = 4;
}

static void _init_map_shop()
{
    map_initcustom(u8"shop_1"s);
    map_data.bgm = 53;
    map_data.max_item_count = 10;
    map_placeplayer();
    map_data.user_map_flag = 0;
    flt();
    itemcreate(-1, 24, 17, 14, 0);
    inv[ci].param1 = 8;
    flt();
    itemcreate(-1, 561, 19, 10, 0);
    inv[ci].count = 5;
    flt();
    itemcreate(-1, 562, 17, 11, 0);
}

static void _init_map_crop()
{
    map_initcustom(u8"crop_1"s);
    map_data.bgm = 68;
    map_placeplayer();
    map_data.max_item_count = 80;
    map_data.user_map_flag = 0;
    flt();
    itemcreate(-1, 24, 14, 5, 0);
    inv[ci].param1 = 9;
}

static void _init_map_ranch()
{
    map_initcustom(u8"ranch_1"s);
    map_data.bgm = 68;
    map_placeplayer();
    map_data.max_item_count = 80;
    map_data.user_map_flag = 0;
    flt();
    itemcreate(-1, 24, 23, 8, 0);
    inv[ci].param1 = 11;
    flt();
    itemcreate(-1, 562, 22, 6, 0);
}

static void _init_map_your_dungeon()
{
    map_initcustom(u8"dungeon1"s);
    map_data.bgm = 68;
    map_placeplayer();
    map_data.max_item_count = 350;
    map_data.user_map_flag = 0;
    flt();
    itemcreate(-1, 24, 39, 54, 0);
    inv[ci].param1 = 15;
}

static void _init_map_storage_house()
{
    map_initcustom(u8"storage_1"s);
    map_data.bgm = 68;
    map_placeplayer();
    map_data.max_item_count = 200;
    map_data.user_map_flag = 0;
}

static void _init_map_test_site()
{
    map_data.width = 16;
    map_data.height = 16;
    map_data.max_crowd_density = 0;
    map_initialize();
    for (int cnt = 0, cnt_end = (map_data.height); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        for (int cnt = 0, cnt_end = (map_data.width); cnt < cnt_end; ++cnt)
        {
            cell_data.at(cnt, p).chip_id_actual = tile_default +
                (rnd(tile_default(2)) == 0) * rnd(tile_default(1));
        }
    }
    map_placeplayer();
    for (int cnt = 0; cnt < 0; ++cnt)
    {
        flt();
        chara_create(
            -1, 0, cdata.player().position.x, cdata.player().position.y + 5);
    }
    for (int cnt = 0; cnt < 100; ++cnt)
    {
        x = rnd(map_data.width);
        y = rnd(map_data.height);
    }
    flt();
    chara_create(-1, 41, cdata.player().position.x, cdata.player().position.y);
    cdata[rc].character_role = 22;
    cdata[rc].is_livestock() = true;
}

static void _init_map_lumiest_graveyard()
{
    map_initcustom(u8"grave_1"s);
    map_data.max_crowd_density = 7;
    map_data.bgm = 79;
    map_placeplayer();
    for (int cnt = 0, cnt_end = (map_data.max_crowd_density / 2); cnt < cnt_end;
         ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 0, -3, 0);
    }
}

static void _init_map_jail()
{
    map_initcustom(u8"jail1"s);
    map_data.max_crowd_density = 0;
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_truce_ground()
{
    map_initcustom(u8"shrine_1"s);
    map_data.max_crowd_density = 10;
    flt();
    itemcreate(-1, 171, 10, 8, 0);
    inv[ci].param1 = 1;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 13, 8, 0);
    inv[ci].param1 = 2;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 10, 13, 0);
    inv[ci].param1 = 5;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 13, 13, 0);
    inv[ci].param1 = 4;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 20, 8, 0);
    inv[ci].param1 = 3;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 23, 8, 0);
    inv[ci].param1 = 7;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 20, 13, 0);
    inv[ci].param1 = 6;
    inv[ci].own_state = 1;
    flt();
    itemcreate(-1, 171, 23, 13, 0);
    inv[ci].own_state = 1;
    map_data.bgm = 79;
    map_placeplayer();
    for (int cnt = 0, cnt_end = (map_data.max_crowd_density / 2); cnt < cnt_end;
         ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 0, -3, 0);
    }
}

static void _init_map_embassy()
{
    map_initcustom(u8"office_1"s);
    map_data.max_crowd_density = 0;
    flt();
    chara_create(-1, 173, 9, 2);
    cdata[rc].character_role = 1012;
    cdata[rc].shop_rank = 10;
    flt();
    chara_create(-1, 173, 15, 2);
    cdata[rc].character_role = 1012;
    cdata[rc].shop_rank = 10;
    flt();
    chara_create(-1, 173, 21, 2);
    cdata[rc].character_role = 1013;
    cdata[rc].shop_rank = 10;
    flt();
    chara_create(-1, 173, 3, 2);
    cdata[rc].character_role = 1013;
    cdata[rc].shop_rank = 10;
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 77, 3 + cnt * 6, 9);
        cdata[rc].character_role = 14;
    }
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_test_world_north_border()
{
    map_initcustom(u8"test2"s);
    map_data.max_crowd_density = 0;
    flt();
    chara_create(-1, 1, 7, 23);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 1, 5, 17);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 1, 16, 19);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 70, 17, 13);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 353, 7, 3);
    cdata[rc].character_role = 23;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        flt();
        chara_create(-1, 9, -3, 0);
        flt();
        chara_create(-1, 159, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
        flt();
        chara_create(-1, 160, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
        flt();
        chara_create(-1, 161, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
    }
    flt();
    chara_create(-1, 77, 5, 7);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 8, 7);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    map_data.bgm = 79;
    map_placeplayer();
    event_add(30);
}

static void _init_map_tyris_border()
{
    map_initcustom(u8"station-nt1"s);
    map_data.max_crowd_density = 0;
    flt();
    chara_create(-1, 1, 7, 23);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 1, 5, 17);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 1, 16, 19);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 70, 17, 13);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 353, 7, 3);
    cdata[rc].character_role = 23;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        flt();
        chara_create(-1, 9, -3, 0);
        flt();
        chara_create(-1, 159, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
        flt();
        chara_create(-1, 160, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
        flt();
        chara_create(-1, 161, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
    }
    flt();
    chara_create(-1, 77, 5, 7);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 8, 7);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_the_smoke_and_pipe()
{
    map_initcustom(u8"inn1"s);
    map_data.max_crowd_density = 0;
    flt();
    chara_create(-1, 1, 19, 10);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 351, 26, 16);
    cdata[rc].character_role = 3;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 35, 25, 15);
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 35, 25, 17);
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 35, 27, 18);
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 35, 27, 16);
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 35, 26, 17);
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 352, 4, 3);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 271, 4, 2);
    flt();
    chara_create(-1, 269, 3, 3);
    flt();
    chara_create(-1, 272, 4, 4);
    flt();
    chara_create(-1, 274, 5, 4);
    flt();
    chara_create(-1, 239, 24, 3);
    flt();
    chara_create(-1, 239, 26, 4);
    flt();
    chara_create(-1, 239, 25, 5);
    flt();
    chara_create(-1, 239, 25, 9);
    flt();
    chara_create(-1, 326, 12, 9);
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        flt();
        chara_create(-1, 9, -3, 0);
        flt();
        chara_create(-1, 159, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
        flt();
        chara_create(-1, 36, -3, 0);
        cdata[rc].relationship = -1;
        cdata[rc].original_relationship = -1;
        flt();
        chara_create(-1, 8, -3, 0);
        flt();
        chara_create(-1, 185, -3, 0);
    }
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_miral_and_garoks_workshop()
{
    map_initcustom(u8"smith0"s);
    map_data.max_crowd_density = 0;
    flt();
    chara_create(-1, 208, 17, 11);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 209, 8, 16);
    cdata[rc].character_role = 1016;
    cdata[rc].shop_rank = 100;
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        flt();
        chara_create(-1, 164, -3, 0);
        cdata[rc].character_role = 3;
    }
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_mansion_of_younger_sister()
{
    map_initcustom(u8"sister"s);
    map_data.max_crowd_density = 0;
    map_data.bgm = 79;
    if (mapupdate == 0)
    {
        flt();
        itemcreate(-1, 668, 12, 8, 0);
        inv[ci].param2 = 4;
    }
    flt();
    chara_create(-1, 249, 12, 6);
    cdata[rc].character_role = 1019;
    for (int cnt = 0; cnt < 6; ++cnt)
    {
        flt();
        chara_create(-1, 211, -3, 0);
        cdata[rc].character_role = 3;
    }
    for (int cnt = 0; cnt < 8; ++cnt)
    {
        flt();
        chara_create(-1, 246, -3, 0);
        cdata[rc].character_role = 3;
    }
    map_placeplayer();
}

static void _init_map_cyber_dome()
{
    map_initcustom(u8"cyberdome"s);
    map_data.max_crowd_density = 10;
    flt();
    itemcreate(-1, 171, 19, 5, 0);
    inv[ci].param1 = 1;
    inv[ci].own_state = 1;
    flt();
    chara_create(-1, 173, 9, 16);
    cdata[rc].character_role = 1011;
    cdata[rc].shop_rank = 10;
    flt();
    chara_create(-1, 173, 9, 8);
    cdata[rc].character_role = 1011;
    cdata[rc].shop_rank = 10;
    flt();
    chara_create(-1, 322, 28, 7);
    cdata[rc].character_role = 3;
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 171, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 172, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0, cnt_end = (map_data.max_crowd_density / 2); cnt < cnt_end;
         ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 0, -3, 0);
    }
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_larna()
{
    map_initcustom(u8"highmountain"s);
    map_data.max_crowd_density = 20;
    flt();
    chara_create(-1, 41, 21, 23);
    cdata[rc].character_role = 21;
    flt();
    chara_create(-1, 1, 9, 44);
    cdata[rc].character_role = 1017;
    cdata[rc].shop_rank = 5;
    cdatan(0, rc) = i18n::s.get("core.chara.job.dye_vendor", cdatan(0, rc));
    flt();
    chara_create(-1, 1, 13, 37);
    cdata[rc].character_role = 1018;
    cdata[rc].shop_rank = 30;
    cdatan(0, rc) =
        i18n::s.get("core.chara.job.souvenir_vendor", cdatan(0, rc));
    flt();
    chara_create(-1, 70, 24, 48);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 239, 7, 36);
    flt();
    chara_create(-1, 239, 9, 38);
    flt();
    chara_create(-1, 239, 6, 33);
    flt();
    chara_create(-1, 239, 3, 33);
    flt();
    chara_create(-1, 239, 8, 31);
    flt();
    chara_create(-1, 239, 4, 36);
    for (int cnt = 0; cnt < 7; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 239, -3, 0);
    }
    for (int cnt = 0; cnt < 15; ++cnt)
    {
        flt();
        chara_create(-1, 239, -3, 0);
    }
    for (int cnt = 0, cnt_end = (map_data.max_crowd_density / 2); cnt < cnt_end;
         ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 0, -3, 0);
    }
    map_data.bgm = 79;
    map_placeplayer();
}

static void _init_map_arena()
{
    map_initcustom(u8"arena_1"s);
    map_placeplayer();
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        if (cdata[cnt].state() == Character::State::alive)
        {
            if (cdata[cnt].relationship == 10)
            {
                if (cnt != 0)
                {
                    cell_data.at(cdata[cnt].position.x, cdata[cnt].position.y)
                        .chara_index_plus_one = 0;
                    cdata[cnt].set_state(Character::State::pet_in_other_map);
                }
            }
        }
    }
    if (arenaop == 0)
    {
        fixlv = static_cast<Quality>(arenaop(2));
        chara_create(
            -1,
            arenaop(1),
            cdata.player().position.x - 1,
            cdata.player().position.y - 4);
        cdata[rc].hate = 30;
        cdata[rc].relationship = -3;
        cdata[rc].relationship = -3;
        cdata[rc].original_relationship = -3;
        cdata[rc].is_lord_of_dungeon() = true;
    }
    if (arenaop == 1)
    {
        for (int cnt = 0, cnt_end = (3 + rnd(4)); cnt < cnt_end; ++cnt)
        {
            flt(arenaop(1), Quality::good);
            chara_create(
                -1,
                0,
                cdata.player().position.x - 1,
                cdata.player().position.y - 5);
            cdata[rc].relationship = -3;
            cdata[rc].original_relationship = -3;
            cdata[rc].hate = 30;
            cdata[rc].relationship = -3;
            cdata[rc].is_lord_of_dungeon() = true;
            if (cdata[rc].level > arenaop(1) || cdata[rc].relationship != -3)
            {
                chara_vanquish(rc);
                --cnt;
                continue;
            }
        }
    }
}

static void _init_map_pet_arena()
{
    map_initcustom(u8"arena_2"s);
    map_data.max_crowd_density = 0;
    map_data.bgm = 81;
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        if (cnt == 0 || cnt == 56)
        {
            continue;
        }
        if (followerin(cnt) == 0)
        {
            cdata[cnt].set_state(Character::State::pet_dead);
            cdata[cnt].position.x = 0;
            cdata[cnt].position.y = 0;
        }
        else
        {
            cdata[cnt].set_state(Character::State::alive);
        }
    }
    map_placeplayer();
    petarenawin = 0;
    for (int cnt = 0, cnt_end = (arenaop(1)); cnt < cnt_end; ++cnt)
    {
        flt(arenaop(2), calcfixlv(Quality::good));
        chara_create(-1, 0, -3, 0);
        cell_data.at(cdata[rc].position.x, cdata[rc].position.y)
            .chara_index_plus_one = 0;
        f = 1;
        if (arenaop == 0)
        {
            if (cdata[rc].level < arenaop(2) / 2)
            {
                f = 0;
            }
        }
        if (cdata[rc].relationship != -3)
        {
            f = 0;
        }
        if (f == 0)
        {
            chara_vanquish(rc);
            --cnt;
            continue;
        }
        map_placearena(rc, true);
        if (cnt == 0)
        {
            enemyteam = rc;
        }
    }
    for (auto&& cnt : cdata.others())
    {
        if (cnt.relationship == -3)
        {
            cnt.has_been_used_stethoscope() = true;
        }
    }
}

static void _init_map_fort_of_chaos_beast()
{
    map_initcustom(u8"god"s);
    map_data.max_crowd_density = 0;
    map_data.bgm = 63;
    flt();
    chara_create(-1, 175, 12, 14);
    map_placeplayer();
}

static void _init_map_fort_of_chaos_machine()
{
    map_initcustom(u8"god"s);
    map_data.max_crowd_density = 0;
    map_data.bgm = 63;
    flt();
    chara_create(-1, 177, 12, 14);
    map_placeplayer();
}

static void _init_map_fort_of_chaos_collapsed()
{
    map_initcustom(u8"god"s);
    map_data.bgm = 63;
    map_data.max_crowd_density = 0;
    flt();
    chara_create(-1, 178, 12, 14);
    map_placeplayer();
}

static void _init_map_your_home()
{
    map_initcustom(u8"home"s + game_data.home_scale);
    map_data.bgm = 68;
    game_data.entrance_type = 4;
    map_placeplayer();
    map_data.user_map_flag = 0;
    map_data.tileset = 3;
    if (game_data.current_dungeon_level == 1)
    {
        if (game_data.home_scale == 0)
        {
            map_data.play_campfire_sound = 1;
            flt();
            chara_create(-1, 33, 18, 10);
            cdata[rc].character_role = 3;
            flt();
            chara_create(-1, 34, 16, 11);
            cdata[rc].character_role = 3;
            flt();
            itemcreate(-1, 510, 6, 10, 0);
            inv[ci].count = 3;
            flt();
            itemcreate(-1, 547, 15, 19, 0);
            inv[ci].count = 4;
            flt();
            itemcreate(-1, 579, 9, 8, 0);
            inv[ci].count = 6;
            flt();
            itemcreate(-1, 24, 18, 19, 0);
            inv[ci].param1 = 1;
        }
        else
        {
            // Move existing characters/items to the center of the
            // map if the home was upgraded.
            ctrl_file(
                FileOperation2::map_items_read, u8"inv_"s + mid + u8".s2");
            for (auto&& item : inv.ground())
            {
                if (item.number() == 0)
                {
                    continue;
                }
                item.position.x = map_data.width / 2;
                item.position.y = map_data.height / 2;
                cell_refresh(item.position.x, item.position.y);
            }
            ctrl_file(FileOperation::map_home_upgrade);
            for (auto&& cnt : cdata.others())
            {
                cnt.position.x = map_data.width / 2;
                cnt.position.y = map_data.height / 2;
                cnt.initial_position.x = map_data.width / 2;
                cnt.initial_position.y = map_data.height / 2;
            }
        }
        if (game_data.home_scale == 5)
        {
            flt();
            chara_create(-1, 1, 31, 20);
            cdata[rc].character_role = 1006;
            cdata[rc].shop_rank = 10;
            cdatan(0, rc) = sngeneral(cdatan(0, rc));
            flt();
            chara_create(-1, 1, 9, 20);
            cdata[rc].character_role = 1001;
            cdata[rc].shop_rank = 12;
            cdatan(0, rc) = snarmor(cdatan(0, rc));
            flt();
            chara_create(-1, 1, 4, 20);
            cdata[rc].character_role = 1008;
            cdata[rc].shop_rank = 10;
            cdatan(0, rc) = sngoods(cdatan(0, rc));
            flt();
            chara_create(-1, 41, 4, 11);
            cdata[rc].character_role = 5;
            flt();
            chara_create(-1, 70, 30, 11);
            cdata[rc].character_role = 9;
            flt();
            chara_create(-1, 74, 30, 4);
            cdata[rc].character_role = 12;
            flt();
            chara_create(-1, 41, 4, 4);
            cdata[rc].character_role = 1004;
            cdata[rc].shop_rank = 11;
            cdatan(0, rc) = snmagic(cdatan(0, rc));
        }
    }
    else
    {
        flt();
        itemcreate(
            -1, 219, cdata.player().position.x, cdata.player().position.y, 0);
    }
    initialize_home_mdata();
}

static void _init_map_north_tyris()
{
    map_initcustom(u8"ntyris"s);
    initialize_world_map();
    map_placeplayer();
}

static void _init_map_south_tyris()
{
    map_initcustom(u8"styris"s);
    initialize_world_map();
    map_placeplayer();
}

static void _init_map_test_world()
{
    map_initcustom(u8"test"s);
    initialize_world_map();
    map_placeplayer();
}

static void _init_map_derphy_town()
{
    map_data.max_crowd_density = 35;
    map_initcustom(u8"rogueden"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    flt();
    chara_create(-1, 253, 23, 14);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 259, 13, 18);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 294, 16, 17);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 1, 10, 17);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 70, 15, 15);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 1, 13, 3);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 29, 23);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 26, 7);
    cdata[rc].character_role = 1008;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngoods(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 30, 4);
    cdata[rc].character_role = 1007;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = snblack(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 29, 4);
    cdata[rc].character_role = 17;
    cdatan(0, rc) = i18n::s.get("core.chara.job.slave_master");
    flt();
    chara_create(-1, 1, 10, 6);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    flt();
    chara_create(-1, 73, 7, 15);
    cdata[rc].character_role = 10;
    flt();
    chara_create(-1, 38, 9, 18);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_derphy", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 13, 18);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 5, 26);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 69, 3, 28);
    cdata[rc].character_role = 8;
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        dbid = 0;
        map_set_chara_generation_filter();
        chara_create(-1, dbid, -3, 0);
    }
}

static void _init_map_derphy_thieves_guild()
{
    map_data.tileset = 0;
    map_initcustom(u8"thiefguild"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::guild);
    map_data.max_crowd_density = 25;
    map_data.bgm = 79;
    map_data.should_regenerate = 0;
    mdatan(0) = i18n::s.get("core.map.unique.thieves_guild.name");
    map_placeplayer();
    flt();
    chara_create(-1, 292, 21, 9);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 40, 3, 6);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 3, 12);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 1, 5, 18);
    cdata[rc].character_role = 1007;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = snblack(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 27, 13);
    cdata[rc].character_role = 1007;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = snblack(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 21, 19);
    cdata[rc].character_role = 1021;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = i18n::s.get("core.chara.job.fence", cdatan(0, rc));
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        flt();
        chara_create(-1, 293, -3, 0);
    }
}

static void _init_map_derphy()
{
    if (game_data.current_dungeon_level == 1)
    {
        _init_map_derphy_town();
    }
    if (game_data.current_dungeon_level == 3)
    {
        _init_map_derphy_thieves_guild();
    }
}

static void _init_map_palmia()
{
    map_data.max_crowd_density = 45;
    map_initcustom(u8"palmia"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    flt();
    chara_create(-1, 70, 42, 27);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 74, 34, 3);
    cdata[rc].character_role = 12;
    flt();
    chara_create(-1, 73, 22, 31);
    cdata[rc].character_role = 10;
    flt();
    chara_create(-1, 142, 5, 15);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 247, 41, 11);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 301, 5, 6);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 320, 24, 6);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 320, 15, 22);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 15, 22);
    cdata[rc].character_role = 3;
    if (game_data.quest_flags.mias_dream == 1000)
    {
        flt();
        chara_create(-1, 246, 42, 11);
        cdata[rc].character_role = 3;
    }
    flt();
    chara_create(-1, 1, 48, 18);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 30, 17);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 48, 3);
    cdata[rc].character_role = 1008;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sngoods(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 42, 17);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 11, 14);
    cdata[rc].character_role = 1003;
    cdata[rc].shop_rank = 9;
    cdatan(0, rc) = snbakery(cdatan(0, rc));
    cdata[rc].image = 138;
    flt();
    chara_create(-1, 41, 41, 3);
    cdata[rc].character_role = 1004;
    cdata[rc].shop_rank = 11;
    cdatan(0, rc) = snmagic(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 41, 28);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 79, 7, 2);
    cdata[rc].character_role = 15;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 80, 6, 2);
    cdata[rc].character_role = 15;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 38, 49, 11);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_palmia", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 30, 27);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 32, 27);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 69, 29, 28);
    cdata[rc].character_role = 8;
    flt();
    chara_create(-1, 77, 16, 5);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 16, 9);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 5, 3);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 8, 3);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 35, 14);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 38, 14);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 29, 2);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 19, 18);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    flt();
    chara_create(-1, 77, 22, 18);
    cdata[rc].character_role = 14;
    cdata[rc].ai_calm = 3;
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 77, -3, 0);
        cdata[rc].character_role = 14;
    }
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 25; ++cnt)
    {
        dbid = 0;
        map_set_chara_generation_filter();
        chara_create(-1, dbid, -3, 0);
    }
}

static void _init_map_lumiest_town()
{
    map_data.max_crowd_density = 40;
    map_initcustom(u8"lumiest"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    if (game_data.quest_flags.sewer_sweeping)
    {
        cell_featset(18, 45, tile_downstairs, 11, 20);
    }
    flt();
    chara_create(-1, 252, 12, 24);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 280, 21, 3);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 290, 5, 20);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 320, 28, 29);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 41, 19);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 32, 43);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 29, 28);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 16, 45);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 13, 24);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 70, 41, 42);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 74, 10, 16);
    cdata[rc].character_role = 12;
    flt();
    chara_create(-1, 1, 47, 30);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 24, 47);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 37, 30);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 37, 12);
    cdata[rc].character_role = 1003;
    cdata[rc].shop_rank = 9;
    cdatan(0, rc) = snbakery(cdatan(0, rc));
    cdata[rc].image = 138;
    flt();
    chara_create(-1, 41, 6, 15);
    cdata[rc].character_role = 1004;
    cdata[rc].shop_rank = 11;
    cdatan(0, rc) = snmagic(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 33, 43);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 47, 12);
    cdata[rc].character_role = 1014;
    cdata[rc].shop_rank = 5;
    cdatan(0, rc) = snfish(cdatan(0, rc));
    flt();
    chara_create(-1, 38, 3, 38);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_lumiest", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 21, 28);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 21, 30);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 69, 23, 38);
    cdata[rc].character_role = 8;
    for (int cnt = 0; cnt < 6; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 7; ++cnt)
    {
        flt();
        chara_create(-1, 77, -3, 0);
        cdata[rc].character_role = 14;
    }
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 25; ++cnt)
    {
        dbid = 0;
        map_set_chara_generation_filter();
        chara_create(-1, dbid, -3, 0);
    }
}

static void _init_map_lumiest_mages_guild()
{
    map_data.tileset = 0;
    map_initcustom(u8"mageguild"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::guild);
    map_data.max_crowd_density = 25;
    map_data.bgm = 79;
    map_data.should_regenerate = 0;
    mdatan(0) = i18n::s.get("core.map.unique.mages_guild.name");
    map_placeplayer();
    flt();
    chara_create(-1, 288, 24, 3);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 41, 27, 8);
    cdata[rc].character_role = 1020;
    cdatan(0, rc) = i18n::s.get("core.chara.job.spell_writer", cdatan(0, rc));
    flt();
    chara_create(-1, 41, 22, 8);
    cdata[rc].character_role = 1004;
    cdata[rc].shop_rank = 11;
    cdatan(0, rc) = snmagic(cdatan(0, rc));
    flt();
    chara_create(-1, 74, 3, 9);
    cdata[rc].character_role = 12;
    flt();
    chara_create(-1, 40, 12, 6);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 3, 3);
    cdata[rc].character_role = 5;
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        flt();
        chara_create(-1, 289, -3, 0);
    }
}

static void _init_map_lumiest_sewer()
{
    map_data.tileset = 0;
    map_initcustom(u8"sqSewer"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    mdatan(0) = i18n::s.get("core.map.unique.the_sewer.name");
    quest_place_target();
    game_data.entrance_type = 1;
    map_placeplayer();
}

static void _init_map_lumiest()
{
    if (game_data.current_dungeon_level == 1)
    {
        _init_map_lumiest_town();
    }
    if (game_data.current_dungeon_level == 3)
    {
        _init_map_lumiest_mages_guild();
    }
    if (game_data.current_dungeon_level == 20)
    {
        _init_map_lumiest_sewer();
    }
}

static void _init_map_yowyn_town()
{
    map_data.max_crowd_density = 35;
    map_initcustom(u8"yowyn"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    if (game_data.quest_flags.cat_house)
    {
        cell_featset(23, 22, tile_downstairs, 11, 3);
    }
    flt();
    chara_create(-1, 224, 3, 17);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 227, 26, 11);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 231, 14, 20);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 1, 11, 5);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 25, 8);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 7, 8);
    cdata[rc].character_role = 1008;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sngoods(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 14, 14);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 35, 18);
    cdata[rc].character_role = 22;
    cdatan(0, rc) = i18n::s.get("core.chara.job.horse_master", cdatan(0, rc));
    flt();
    chara_create(-1, 267, 33, 16);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 267, 37, 19);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 268, 34, 19);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 268, 38, 16);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 38, 3, 4);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_yowyn", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 20, 14);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 24, 16);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 69, 26, 16);
    cdata[rc].character_role = 8;
    flt();
    chara_create(-1, 213, 14, 12);
    cdata[rc].character_role = 3;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        flt();
        chara_create(-1, 77, -3, 0);
        cdata[rc].character_role = 14;
    }
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 15; ++cnt)
    {
        dbid = 0;
        map_set_chara_generation_filter();
        chara_create(-1, dbid, -3, 0);
    }
}

static void _init_map_yowyn_cat_mansion()
{
    map_data.tileset = 0;
    map_initcustom(u8"sqcat"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    mdatan(0) = i18n::s.get("core.map.unique.cat_mansion.name");
    quest_place_target();
    map_placeplayer();
}

static void _init_map_yowyn_battle_field()
{
    map_data.tileset = 0;
    map_initcustom(u8"sqwar"s);
    map_data.indoors_flag = 2;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    map_data.refresh_type = 0;
    mdatan(0) = i18n::s.get("core.map.unique.battle_field.name");
    quest_place_target();
    game_data.entrance_type = 8;
    map_placeplayer();
    listmax = 0;
    for (auto&& cnt : cdata.others())
    {
        if (cnt.state() == Character::State::alive)
        {
            if (cnt.is_quest_target() == 1)
            {
                list(0, listmax) = cnt.index;
                ++listmax;
            }
        }
    }
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        flt();
        chara_create(-1, 233 + 2 * (cnt > 22), 11, 16);
        cdata[rc].relationship = 10;
        cdata[rc].original_relationship = 10;
        cdata[rc].hate = 100;
        p = list(0, rnd(listmax));
        cdata[rc].enemy_id = p;
        cdata[p].hate = 100;
        cdata[p].enemy_id = rc;
    }
    noaggrorefresh = 1;
}

static void _init_map_yowyn()
{
    if (game_data.current_dungeon_level == 1)
    {
        _init_map_yowyn_town();
    }
    if (game_data.current_dungeon_level == 3)
    {
        _init_map_yowyn_cat_mansion();
    }
    if (game_data.current_dungeon_level == 4)
    {
        _init_map_yowyn_battle_field();
    }
}

static void _init_map_noyel()
{
    map_data.max_crowd_density = 35;
    map_initcustom(u8"noyel"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    flt();
    chara_create(-1, 202, 46, 18);
    game_data.fire_giant = rc;
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 203, 47, 18);
    cdata[rc].character_role = 1015;
    flt();
    chara_create(-1, 35, 47, 20);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 35, 45, 19);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 35, 49, 20);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 28, 22);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 221, 19, 3);
    cdata[rc].character_role = 3;
    if (game_data.quest_flags.pael_and_her_mom != 1001)
    {
        flt();
        chara_create(-1, 222, 19, 2);
        cdata[rc].character_role = 3;
    }
    flt();
    chara_create(-1, 70, 40, 33);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 74, 44, 6);
    cdata[rc].character_role = 12;
    flt();
    chara_create(-1, 206, 44, 3);
    cdata[rc].character_role = 19;
    flt();
    chara_create(-1, 1, 19, 31);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 11, 31);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 38, 34);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 5, 27);
    cdata[rc].character_role = 1003;
    cdata[rc].shop_rank = 9;
    cdatan(0, rc) = snbakery(cdatan(0, rc));
    cdata[rc].image = 138;
    flt();
    chara_create(-1, 41, 56, 5);
    cdata[rc].character_role = 1004;
    cdata[rc].shop_rank = 11;
    cdatan(0, rc) = snmagic(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 39, 35);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 38, 5, 18);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_noyel", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 18, 20);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 4, 33);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 69, 6, 33);
    cdata[rc].character_role = 8;
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        flt();
        chara_create(-1, 16, rnd(32), rnd(map_data.height));
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, rnd(32), rnd(map_data.height));
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        flt();
        chara_create(-1, 77, rnd(32), rnd(map_data.height));
        cdata[rc].character_role = 14;
    }
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 8; ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 35, rnd(11) + 25, rnd(5) + 15);
        cdata[rc].character_role = 3;
    }
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        dbid = 0;
        map_set_chara_generation_filter();
        chara_create(-1, dbid, rnd(55), rnd(map_data.height));
    }
}

static void _init_map_port_kapul_town()
{
    map_data.max_crowd_density = 40;
    map_initcustom(u8"kapul"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    flt();
    chara_create(-1, 223, 15, 18);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 243, 36, 27);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 279, 5, 26);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 297, 29, 3);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 320, 24, 21);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 320, 12, 26);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 320, 8, 11);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 8, 14);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 1, 16, 17);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 23, 7);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 32, 14);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 22, 14);
    cdata[rc].character_role = 1008;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngoods(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 16, 25);
    cdata[rc].character_role = 1007;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = snblack(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 17, 28);
    cdata[rc].character_role = 1002;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = snfood(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 22, 22);
    cdata[rc].character_role = 1004;
    cdata[rc].shop_rank = 11;
    cdatan(0, rc) = snmagic(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 35, 3);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 70, 15, 15);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 73, 26, 3);
    cdata[rc].character_role = 10;
    flt();
    chara_create(-1, 179, 25, 4);
    cdata[rc].character_role = 11;
    flt();
    chara_create(-1, 38, 8, 12);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_port_kapul", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 16, 4);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 14, 4);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 69, 17, 5);
    cdata[rc].character_role = 8;
    flt();
    chara_create(-1, 74, 27, 11);
    cdata[rc].character_role = 12;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 71, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        flt();
        chara_create(-1, 76, -3, 0);
        cdata[rc].character_role = 14;
    }
    flt();
    chara_create(-1, 72, 7, 6);
    cdata[rc].character_role = 4;
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 0, -3, 0);
    }
}

static void _init_map_port_kapul_fighters_guild()
{
    map_data.tileset = 0;
    map_initcustom(u8"fighterguild"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::guild);
    map_data.max_crowd_density = 25;
    map_data.bgm = 79;
    map_data.should_regenerate = 0;
    mdatan(0) = i18n::s.get("core.map.unique.fighters_guild.name");
    map_placeplayer();
    flt();
    chara_create(-1, 291, 27, 4);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 74, 28, 10);
    cdata[rc].character_role = 12;
    flt();
    chara_create(-1, 40, 15, 10);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 14, 18);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 1, 29, 15);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        flt();
        chara_create(-1, 295, -3, 0);
    }
}

static void _init_map_port_kapul_doom_ground()
{
    map_data.tileset = 0;
    map_initcustom(u8"sqkamikaze"s);
    map_data.indoors_flag = 2;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    map_data.refresh_type = 0;
    mdatan(0) = i18n::s.get("core.map.unique.doom_ground.name");
    game_data.entrance_type = 4;
    game_data.quest_flags.duration_of_kamikaze_attack = 0;
    map_placeplayer();
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        flt();
        chara_create(
            -1, 204, cdata.player().position.x, cdata.player().position.y);
        cdata[rc].relationship = 10;
        cdata[rc].original_relationship = 10;
    }
    noaggrorefresh = 1;
}

static void _init_map_port_kapul()
{
    if (game_data.current_dungeon_level == 1)
    {
        _init_map_port_kapul_town();
    }
    if (game_data.current_dungeon_level == 3)
    {
        _init_map_port_kapul_fighters_guild();
    }
    if (game_data.current_dungeon_level == 25)
    {
        _init_map_port_kapul_doom_ground();
    }
}

static void _init_map_vernis_town()
{
    map_data.max_crowd_density = 40;
    map_initcustom(u8"vernis"s);
    map_placeplayer();
    map_data.user_map_flag = 0;
    if (game_data.quest_flags.thieves_hideout)
    {
        cell_featset(48, 5, tile_downstairs, 11, 4);
    }
    flt();
    chara_create(-1, 28, 39, 3);
    flt();
    chara_create(-1, 29, 42, 23);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 30, 24, 5);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 31, 40, 24);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 32, 40, 25);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 226, 30, 5);
    cdata[rc].character_role = 3;
    flt();
    chara_create(-1, 326, 42, 24);
    cdata[rc].character_role = 3;
    if (game_data.quest_flags.puppys_cave == 1000)
    {
        flt();
        chara_create(-1, 225, 31, 4);
        cdata[rc].character_role = 3;
    }
    flt();
    chara_create(-1, 1, 47, 9);
    cdata[rc].character_role = 1014;
    cdata[rc].shop_rank = 5;
    cdatan(0, rc) = snfish(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 14, 12);
    cdata[rc].character_role = 1001;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = snarmor(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 39, 27);
    cdata[rc].character_role = 1009;
    cdata[rc].shop_rank = 12;
    cdatan(0, rc) = sntrade(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 10, 15);
    cdata[rc].character_role = 1006;
    cdata[rc].shop_rank = 10;
    cdatan(0, rc) = sngeneral(cdatan(0, rc));
    flt();
    chara_create(-1, 41, 7, 26);
    cdata[rc].character_role = 1004;
    cdata[rc].shop_rank = 11;
    cdatan(0, rc) = snmagic(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 14, 25);
    cdata[rc].character_role = 1005;
    cdata[rc].shop_rank = 8;
    cdatan(0, rc) = sninn(cdatan(0, rc));
    flt();
    chara_create(-1, 1, 22, 26);
    cdata[rc].character_role = 1003;
    cdata[rc].shop_rank = 9;
    cdatan(0, rc) = snbakery(cdatan(0, rc));
    cdata[rc].image = 138;
    flt();
    chara_create(-1, 41, 28, 16);
    cdata[rc].character_role = 5;
    flt();
    chara_create(-1, 70, 38, 27);
    cdata[rc].character_role = 9;
    flt();
    chara_create(-1, 74, 6, 25);
    cdata[rc].character_role = 12;
    flt();
    chara_create(-1, 38, 10, 7);
    cdata[rc].character_role = 6;
    cdatan(0, rc) = i18n::s.get("core.chara.job.of_vernis", cdatan(0, rc));
    flt();
    chara_create(-1, 40, 27, 16);
    cdata[rc].character_role = 7;
    cdatan(0, rc) = sntrainer(cdatan(0, rc));
    flt();
    chara_create(-1, 69, 25, 16);
    cdata[rc].character_role = 8;
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 16, -3, 0);
        cdata[rc].character_role = 4;
        flt();
        chara_create(-1, 39, -3, 0);
        cdata[rc].character_role = 4;
    }
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt();
        chara_create(-1, 77, -3, 0);
        cdata[rc].character_role = 14;
    }
    quest_on_map_initialize();
    for (int cnt = 0; cnt < 25; ++cnt)
    {
        dbid = 0;
        map_set_chara_generation_filter();
        chara_create(-1, dbid, -3, 0);
    }
}

static void _init_map_vernis_the_mine()
{
    map_data.tileset = 0;
    map_initcustom(u8"puti"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    mdatan(0) = i18n::s.get("core.map.unique.the_mine.name");
    quest_place_target();
    map_placeplayer();
}

static void _init_map_vernis_robbers_hideout()
{
    map_data.tileset = 0;
    map_initcustom(u8"sqRogue"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    mdatan(0) = i18n::s.get("core.map.unique.robbers_hideout.name");
    quest_place_target();
    map_placeplayer();
}

static void _init_map_vernis_test_site()
{
    map_data.tileset = 0;
    map_initcustom(u8"sqNightmare"s);
    map_data.indoors_flag = 1;
    map_data.type = static_cast<int>(mdata_t::MapType::dungeon);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_data.should_regenerate = 1;
    mdatan(0) = i18n::s.get("core.map.unique.test_site.name");
    quest_place_target();
    game_data.entrance_type = 7;
    mapstartx = 6;
    mapstarty = 27;
    map_placeplayer();
}

static void _init_map_vernis()
{
    if (game_data.current_dungeon_level == 1)
    {
        _init_map_vernis_town();
    }
    if (game_data.current_dungeon_level == 3)
    {
        _init_map_vernis_the_mine();
    }
    if (game_data.current_dungeon_level == 4)
    {
        _init_map_vernis_robbers_hideout();
    }
    if (game_data.current_dungeon_level == 5)
    {
        _init_map_vernis_test_site();
    }
}

static void _init_map_fields_forest()
{
    mdatan(0) = i18n::s.get_enum_property("core.map.unique", "forest", 2);
    map_randomtile(8, 25);
    map_randomtile(0, 10);
    map_randomtile(1, 4);
    map_randomtile(4, 2);
    for (int cnt = 0, cnt_end = (20 + rnd(20)); cnt < cnt_end; ++cnt)
    {
        flt();
        flttypemajor = 80000;
        itemcreate(-1, 0, -1, -1, 0);
        inv[ci].own_state = 1;
        cell_data.at(inv[ci].position.x, inv[ci].position.y).chip_id_actual = 0;
    }
}

static void _init_map_fields_sea()
{
    mdatan(0) = i18n::s.get_enum_property("core.map.unique", "sea", 2);
}

static void _init_map_fields_grassland()
{
    mdatan(0) = i18n::s.get_enum_property("core.map.unique", "grassland", 2);
    map_randomtile(9, 10);
    map_randomtile(10, 10);
    map_randomtile(0, 30);
    map_randomtile(1, 4);
    map_randomtile(4, 2);
    map_randomtile(3, 2);
    map_randomtile(4, 2);
    map_randomtile(5, 2);
    for (int cnt = 0, cnt_end = (10 + rnd(10)); cnt < cnt_end; ++cnt)
    {
        flt();
        flttypemajor = 80000;
        itemcreate(-1, 0, -1, -1, 0);
        inv[ci].own_state = 1;
    }
}

static void _init_map_fields_desert()
{
    mdatan(0) = i18n::s.get_enum_property("core.map.unique", "desert", 2);
    map_randomtile(18, 25);
    map_randomtile(17, 10);
    map_randomtile(19, 2);
    map_randomtile(20, 4);
    map_randomtile(21, 2);
    for (int cnt = 0, cnt_end = (4 + rnd(4)); cnt < cnt_end; ++cnt)
    {
        flt();
        itemcreate(-1, 527, -1, -1, 0);
        inv[ci].own_state = 1;
    }
}

static void _init_map_fields_snow_field()
{
    mdatan(0) = i18n::s.get_enum_property("core.map.unique", "snow_field", 2);
    map_randomtile(57, 4);
    map_randomtile(56, 4);
    map_randomtile(49, 2);
    map_randomtile(46, 1);
    map_randomtile(47, 1);
    map_randomtile(48, 1);
    map_randomtile(51, 1);
    for (int cnt = 0, cnt_end = (3 + rnd(5)); cnt < cnt_end; ++cnt)
    {
        flt();
        flttypemajor = 80000;
        fltselect = 8;
        itemcreate(-1, 0, -1, -1, 0);
        inv[ci].own_state = 1;
    }
}

static void _init_map_fields_plain_field()
{
    mdatan(0) = i18n::s.get_enum_property("core.map.unique", "plain_field", 2);
    map_randomtile(1, 10);
    map_randomtile(2, 2);
    map_randomtile(3, 2);
    map_randomtile(4, 2);
    map_randomtile(5, 2);
    map_randomtile(6, 2);
    for (int cnt = 0, cnt_end = (5 + rnd(5)); cnt < cnt_end; ++cnt)
    {
        flt();
        flttypemajor = 80000;
        itemcreate(-1, 0, -1, -1, 0);
        inv[ci].own_state = 1;
    }
}

static void _init_map_fields_maybe_generate_encounter()
{
    if (encounter == 0)
    {
        for (int cnt = 0, cnt_end = (map_data.max_crowd_density + 1);
             cnt < cnt_end;
             ++cnt)
        {
            map_set_chara_generation_filter();
            flt();
            chara_create(-1, 0, -3, 0);
        }
    }
    if (encounter == 4)
    {
        map_data.max_crowd_density = 0;
        flt();
        initlv = encounterlv;
        chara_create(
            -1, 302, cdata.player().position.x, cdata.player().position.y);
        for (int cnt = 0, cnt_end = (6 + rnd(6)); cnt < cnt_end; ++cnt)
        {
            flt();
            initlv = encounterlv + rnd(10);
            chara_create(-1, 303 + rnd(3), 14, 11);
            cdatan(0, rc) += u8" Lv"s + cdata[rc].level;
        }
        event_add(23);
    }
    if (encounter == 3)
    {
        map_data.max_crowd_density = 0;
        map_data.type = static_cast<int>(mdata_t::MapType::temporary);
        rq = encounterref;
        game_data.executing_immediate_quest_type = 1007;
        game_data.executing_immediate_quest_show_hunt_remain = 1;
        game_data.executing_immediate_quest = rq;
        game_data.executing_immediate_quest_status = 1;
        p = rnd(3) + 5;
        for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
        {
            flt(quest_data[rq].difficulty, Quality::great);
            int stat = chara_create(
                -1, 0, cdata.player().position.x, cdata.player().position.y);
            if (stat != 0)
            {
                cdata[rc].hate = 30;
                cdata[rc].relationship = -3;
                cdata[rc].original_relationship = -3;
            }
        }
    }
    if (encounter == 2)
    {
        flt();
        chara_create(-1, 1, 10, 11);
        cdata[rc].character_role = 1010;
        cdata[rc].shop_rank = encounterlv;
        cdatan(0, rc) =
            i18n::s.get("core.chara.job.wandering_vendor", cdatan(0, rc));
        generatemoney(rc);
        for (int cnt = 0, cnt_end = (encounterlv / 2 + 1); cnt < cnt_end; ++cnt)
        {
            r2 = 1;
            gain_level(cdata[rc]);
        }
        event_add(11);
        for (int cnt = 0, cnt_end = (6 + rnd(6)); cnt < cnt_end; ++cnt)
        {
            flt();
            initlv = encounterlv + rnd(10);
            chara_create(-1, 159 + rnd(3), 14, 11);
            cdata[rc].character_role = 16;
            cdatan(0, rc) += u8" Lv"s + cdata[rc].level;
        }
    }
    if (encounter == 1)
    {
        p = rnd(9);
        if (cdata.player().level <= 5)
        {
            p = rnd(3);
        }
        for (int cnt = 0, cnt_end = (2 + p); cnt < cnt_end; ++cnt)
        {
            flt(calcobjlv(encounterlv), calcfixlv(Quality::bad));
            if (game_data.weather == 1)
            {
                if ((33 > game_data.stood_world_map_tile ||
                     game_data.stood_world_map_tile >= 66) &&
                    rnd(3) == 0)
                {
                    fixlv = Quality::godly;
                }
            }
            if (cnt < 4)
            {
                chara_create(
                    -1,
                    0,
                    cdata.player().position.x,
                    cdata.player().position.y);
            }
            else
            {
                chara_create(-1, 0, -3, 0);
            }
            cdata[rc].hate = 30;
        }
    }
    encounter = 0;
}

static void _init_map_fields()
{
    map_data.width = 34;
    map_data.height = 22;
    map_data.max_crowd_density = 4;
    map_data.user_map_flag = 0;
    map_initialize();
    for (int cnt = 0, cnt_end = (map_data.height); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        for (int cnt = 0, cnt_end = (map_data.width); cnt < cnt_end; ++cnt)
        {
            cell_data.at(cnt, p).chip_id_actual = tile_default +
                (rnd(tile_default(2)) == 0) * rnd(tile_default(1));
        }
    }

    mdatan(0) = "";

    switch (map_get_field_type())
    {
    case FieldMapType::plain_field: _init_map_fields_plain_field(); break;
    case FieldMapType::forest: _init_map_fields_forest(); break;
    case FieldMapType::sea: _init_map_fields_sea(); break;
    case FieldMapType::grassland: _init_map_fields_grassland(); break;
    case FieldMapType::desert: _init_map_fields_desert(); break;
    case FieldMapType::snow_field: _init_map_fields_snow_field(); break;
    default: assert(0); break;
    }

    map_placeplayer();
    if (264 > game_data.stood_world_map_tile ||
        game_data.stood_world_map_tile >= 363)
    {
        for (int cnt = 0, cnt_end = (4 + rnd(5)); cnt < cnt_end; ++cnt)
        {
            flt();
            flttypeminor = 64000;
            itemcreate(-1, 0, -1, -1, 0);
        }
    }

    _init_map_fields_maybe_generate_encounter();
}

static void _init_map_the_void()
{
    generate_random_nefia();
    if (game_data.void_next_lord_floor == 0)
    {
        game_data.void_next_lord_floor =
            area_data[game_data.current_map].danger_level + 4;
    }
    if (game_data.void_next_lord_floor <= game_data.current_dungeon_level)
    {
        event_add(29);
    }
    else
    {
        area_data[game_data.current_map].has_been_conquered = 0;
    }
}

static void _init_map_lesimas()
{
    map_tileset(map_data.tileset);
    for (int cnt = 0; cnt < 1; ++cnt)
    {
        if (game_data.current_dungeon_level ==
            area_data[game_data.current_map].deepest_level)
        {
            map_initcustom(u8"lesimas_1"s);
            map_data.max_crowd_density = 0;
            map_data.refresh_type = 0;
            map_data.bgm = 66;
            mdatan(0) =
                i18n::s.get_enum_property("core.map.unique", "the_depth", 3);
            if (game_data.quest_flags.main_quest < 170)
            {
                event_add(3);
            }
            x = 16;
            y = 13;
            cell_featset(x, y, tile_upstairs, 10);
            map_data.stair_up_pos = y * 1000 + x;
            map_placeplayer();
            if (npcmemory(0, charaid2int(CharaId::zeome)) == 0)
            {
                flt();
                chara_create(-1, 2, 16, 6);
            }
            else if (npcmemory(0, charaid2int(CharaId::orphe)) == 0)
            {
                flt();
                chara_create(-1, 23, 16, 6);
            }
            break;
        }
        generate_random_nefia();
        break;
    }
    if (game_data.current_dungeon_level == 3)
    {
        chara_create(
            -1, 139, cdata.player().position.x, cdata.player().position.y);
        cdata[rc].character_role = 3;
        cdata[rc].ai_calm = 3;
    }
    if (game_data.current_dungeon_level == 17)
    {
        chara_create(
            -1, 146, cdata.player().position.x, cdata.player().position.y);
        cdata[rc].character_role = 3;
        cdata[rc].ai_calm = 3;
    }
}

static void _init_map_tower_of_fire()
{
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        map_initcustom(u8"firet1"s);
        map_data.max_crowd_density = 0;
        map_data.bgm = 66;
        map_placeplayer();
    }
    else
    {
        generate_random_nefia();
    }
}

static void _init_map_crypt_of_the_damned()
{
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        map_initcustom(u8"undeadt1"s);
        map_data.max_crowd_density = 0;
        map_data.bgm = 66;
        map_placeplayer();
    }
    else
    {
        generate_random_nefia();
    }
}

static void _init_map_ancient_castle()
{
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        map_initcustom(u8"roguet1"s);
        map_data.max_crowd_density = 0;
        map_data.bgm = 66;
        map_placeplayer();
    }
    else
    {
        generate_random_nefia();
    }
}

static void _init_map_dragons_nest()
{
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        map_initcustom(u8"d_1"s);
        map_data.max_crowd_density = 0;
        map_data.bgm = 66;
        map_placeplayer();
    }
    else
    {
        generate_random_nefia();
    }
}



static void _init_map_minotaurs_nest()
{
    generate_random_nefia();
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        if (game_data.quest_flags.minotaur_king < 2)
        {
            flt();
            chara_create(-1, 300, -3, 0);
        }
    }
}

static void _init_map_yeeks_nest()
{
    generate_random_nefia();
    if (game_data.current_dungeon_level ==
        area_data[game_data.current_map].deepest_level)
    {
        if (game_data.quest_flags.novice_knight < 2)
        {
            flt();
            chara_create(-1, 242, -3, 0);
            tc = rc;
            for (int cnt = 0; cnt < 5; ++cnt)
            {
                flt();
                chara_create(
                    -1, 240, cdata[tc].position.x, cdata[tc].position.y);
            }
            for (int cnt = 0; cnt < 10; ++cnt)
            {
                flt();
                chara_create(
                    -1, 238, cdata[tc].position.x, cdata[tc].position.y);
                flt();
                chara_create(
                    -1, 237, cdata[tc].position.x, cdata[tc].position.y);
            }
        }
    }
}

static void _init_map_pyramid_first_floor()
{
    map_initcustom(u8"sqPyramid"s);
    map_data.max_crowd_density = 40;
    map_data.bgm = 61;
    map_placeplayer();
    for (int cnt = 0, cnt_end = (map_data.max_crowd_density + 1); cnt < cnt_end;
         ++cnt)
    {
        map_set_chara_generation_filter();
        chara_create(-1, 0, -3, 0);
    }
}

static void _init_map_pyramid_second_floor()
{
    map_initcustom(u8"sqPyramid2"s);
    map_data.max_crowd_density = 0;
    map_data.bgm = 61;
    map_placeplayer();
}

static void _init_map_pyramid()
{
    if (game_data.current_dungeon_level == 20)
    {
        _init_map_pyramid_first_floor();
    }
    if (game_data.current_dungeon_level == 21)
    {
        _init_map_pyramid_second_floor();
    }
}

void initialize_map_from_map_type()
{
    using namespace mdata_t;
    if (game_data.current_map == MapId::your_home)
    {
        if (mdatan(0) == ""s ||
            mdatan(0) ==
                i18n::s.get_enum_property("core.map.unique", "name", 4))
        {
            mdatan(0) = i18n::s.get_enum_property("core.map.unique", "name", 7);
        }
    }
    else
    {
        mdatan(0) = mapname(game_data.current_map);
    }


    // In most cases the area's map ID will be the same as
    // game_data.current_map. However, multiple player-owned areas can share
    // the same map ID.
    int map_id = area_data[game_data.current_map].id;
    auto map = the_mapdef_db[map_id];

    if (map && map->generator)
    {
        lua::MapGenerator generator{};
        map->generator->call(generator);
        return;
    }

    MapId map_id_ = static_cast<mdata_t::MapId>(map_id);
    switch (map_id_)
    {
        // clang-format off
    case mdata_t::MapId::shelter_:                   _init_map_shelter();                   break;
    case mdata_t::MapId::random_dungeon:             _init_map_nefia();                     break;
    case mdata_t::MapId::museum:                     _init_map_museum();                    break;
    case mdata_t::MapId::shop:                       _init_map_shop();                      break;
    case mdata_t::MapId::crop:                       _init_map_crop();                      break;
    case mdata_t::MapId::ranch:                      _init_map_ranch();                     break;
    case mdata_t::MapId::your_dungeon:               _init_map_your_dungeon();              break;
    case mdata_t::MapId::storage_house:              _init_map_storage_house();             break;
    default: break;
        // clang-format on
    }

    map_id_ = static_cast<mdata_t::MapId>(game_data.current_map);
    switch (map_id_)
    {
        // clang-format off
    case mdata_t::MapId::quest:                      generate_random_nefia();               break;
    case mdata_t::MapId::test_site:                  _init_map_test_site();                 break;
    case mdata_t::MapId::lumiest_graveyard:          _init_map_lumiest_graveyard();         break;
    case mdata_t::MapId::jail:                       _init_map_jail();                      break;
    case mdata_t::MapId::truce_ground:               _init_map_truce_ground();              break;
    case mdata_t::MapId::embassy:                    _init_map_embassy();                   break;
    case mdata_t::MapId::test_world_north_border:    _init_map_test_world_north_border();   break;
    case mdata_t::MapId::north_tyris_south_border:
    case mdata_t::MapId::south_tyris_north_border:   _init_map_tyris_border();              break;
    case mdata_t::MapId::the_smoke_and_pipe:         _init_map_the_smoke_and_pipe();        break;
    case mdata_t::MapId::miral_and_garoks_workshop:  _init_map_miral_and_garoks_workshop(); break;
    case mdata_t::MapId::mansion_of_younger_sister:  _init_map_mansion_of_younger_sister(); break;
    case mdata_t::MapId::cyber_dome:                 _init_map_cyber_dome();                break;
    case mdata_t::MapId::larna:                      _init_map_larna();                     break;
    case mdata_t::MapId::arena:                      _init_map_arena();                     break;
    case mdata_t::MapId::pet_arena:                  _init_map_pet_arena();                 break;
    case mdata_t::MapId::fort_of_chaos_beast:        _init_map_fort_of_chaos_beast();       break;
    case mdata_t::MapId::fort_of_chaos_machine:      _init_map_fort_of_chaos_machine();     break;
    case mdata_t::MapId::fort_of_chaos_collapsed:    _init_map_fort_of_chaos_collapsed();   break;
    case mdata_t::MapId::your_home:                  _init_map_your_home();                 break;
    case mdata_t::MapId::north_tyris:                _init_map_north_tyris();               break;
    case mdata_t::MapId::south_tyris:                _init_map_south_tyris();               break;
    case mdata_t::MapId::test_world:                 _init_map_test_world();                break;
    case mdata_t::MapId::derphy:                     _init_map_derphy();                    break;
    case mdata_t::MapId::palmia:                     _init_map_palmia();                    break;
    case mdata_t::MapId::lumiest:                    _init_map_lumiest();                   break;
    case mdata_t::MapId::yowyn:                      _init_map_yowyn();                     break;
    case mdata_t::MapId::noyel:                      _init_map_noyel();                     break;
    case mdata_t::MapId::port_kapul:                 _init_map_port_kapul();                break;
    case mdata_t::MapId::vernis:                     _init_map_vernis();                    break;
    case mdata_t::MapId::debug_map:                  generate_debug_map();                  break;
    case mdata_t::MapId::fields:                     _init_map_fields();                    break;
    case mdata_t::MapId::the_void:                   _init_map_the_void();                  break;
    case mdata_t::MapId::lesimas:                    _init_map_lesimas();                   break;
    case mdata_t::MapId::tower_of_fire:              _init_map_tower_of_fire();             break;
    case mdata_t::MapId::crypt_of_the_damned:        _init_map_crypt_of_the_damned();       break;
    case mdata_t::MapId::ancient_castle:             _init_map_ancient_castle();            break;
    case mdata_t::MapId::dragons_nest:               _init_map_dragons_nest();              break;
    case mdata_t::MapId::minotaurs_nest:             _init_map_minotaurs_nest();            break;
    case mdata_t::MapId::yeeks_nest:                 _init_map_yeeks_nest();                break;
    case mdata_t::MapId::pyramid:                    _init_map_pyramid();                   break;
    case mdata_t::MapId::mountain_pass:              generate_random_nefia();               break;
    case mdata_t::MapId::show_house:
    case mdata_t::MapId::none:
    default: break;
        // clang-format on
    }
}

} // namespace elona
