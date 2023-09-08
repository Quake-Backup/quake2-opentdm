/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "g_local.h"

#define Function(f) {#f, f}

mmove_t mmove_reloc;

field_t fields[] = {
	{"classname", FOFS(classname), F_LSTRING},
	{"model", FOFS(model), F_LSTRING},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"accel", FOFS(accel), F_FLOAT},
	{"decel", FOFS(decel), F_FLOAT},
	{"target", FOFS(target), F_LSTRING},
	{"targetname", FOFS(targetname), F_LSTRING},
	{"pathtarget", FOFS(pathtarget), F_LSTRING},
	{"deathtarget", FOFS(deathtarget), F_LSTRING},
	{"killtarget", FOFS(killtarget), F_LSTRING},
	{"combattarget", FOFS(combattarget), F_LSTRING},
	{"message", FOFS(message), F_LSTRING},
	{"team", FOFS(team), F_LSTRING},
	{"wait", FOFS(wait), F_FLOAT},
	{"delay", FOFS(delay), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"move_origin", FOFS(move_origin), F_VECTOR},
	{"move_angles", FOFS(move_angles), F_VECTOR},
	{"style", FOFS(style), F_INT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"sounds", FOFS(sounds), F_INT},
	{"light", 0, F_IGNORE},
	{"dmg", FOFS(dmg), F_INT},
	{"mass", FOFS(mass), F_INT},
	{"volume", FOFS(volume), F_FLOAT},
	{"attenuation", FOFS(attenuation), F_FLOAT},
	{"map", FOFS(map), F_LSTRING},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"angle", FOFS(s.angles), F_ANGLEHACK},

/*	{"goalentity", FOFS(goalentity), F_EDICT, FFL_NOSPAWN},
	{"movetarget", FOFS(movetarget), F_EDICT, FFL_NOSPAWN},
	{"enemy", FOFS(enemy), F_EDICT, FFL_NOSPAWN},
	{"oldenemy", FOFS(oldenemy), F_EDICT, FFL_NOSPAWN},
	{"activator", FOFS(activator), F_EDICT, FFL_NOSPAWN},
	{"groundentity", FOFS(groundentity), F_EDICT, FFL_NOSPAWN},
	{"teamchain", FOFS(teamchain), F_EDICT, FFL_NOSPAWN},
	{"teammaster", FOFS(teammaster), F_EDICT, FFL_NOSPAWN},
	{"owner", FOFS(owner), F_EDICT, FFL_NOSPAWN},
	{"mynoise", FOFS(mynoise), F_EDICT, FFL_NOSPAWN},
	{"mynoise2", FOFS(mynoise2), F_EDICT, FFL_NOSPAWN},
	{"target_ent", FOFS(target_ent), F_EDICT, FFL_NOSPAWN},
	{"chain", FOFS(chain), F_EDICT, FFL_NOSPAWN},

	{"prethink", FOFS(prethink), F_FUNCTION, FFL_NOSPAWN},
	{"think", FOFS(think), F_FUNCTION, FFL_NOSPAWN},
	{"blocked", FOFS(blocked), F_FUNCTION, FFL_NOSPAWN},
	{"touch", FOFS(touch), F_FUNCTION, FFL_NOSPAWN},
	{"use", FOFS(use), F_FUNCTION, FFL_NOSPAWN},
	{"pain", FOFS(pain), F_FUNCTION, FFL_NOSPAWN},
	{"die", FOFS(die), F_FUNCTION, FFL_NOSPAWN},

	{"stand", FOFS(monsterinfo.stand), F_FUNCTION, FFL_NOSPAWN},
	{"idle", FOFS(monsterinfo.idle), F_FUNCTION, FFL_NOSPAWN},
	{"search", FOFS(monsterinfo.search), F_FUNCTION, FFL_NOSPAWN},
	{"walk", FOFS(monsterinfo.walk), F_FUNCTION, FFL_NOSPAWN},
	{"run", FOFS(monsterinfo.run), F_FUNCTION, FFL_NOSPAWN},
	{"dodge", FOFS(monsterinfo.dodge), F_FUNCTION, FFL_NOSPAWN},
	{"attack", FOFS(monsterinfo.attack), F_FUNCTION, FFL_NOSPAWN},
	{"melee", FOFS(monsterinfo.melee), F_FUNCTION, FFL_NOSPAWN},
	{"sight", FOFS(monsterinfo.sight), F_FUNCTION, FFL_NOSPAWN},
	{"checkattack", FOFS(monsterinfo.checkattack), F_FUNCTION, FFL_NOSPAWN},
	{"currentmove", FOFS(monsterinfo.currentmove), F_MMOVE, FFL_NOSPAWN},

	{"endfunc", FOFS(moveinfo.endfunc), F_FUNCTION, FFL_NOSPAWN},*/

	// temp spawn vars -- only valid when the spawn function is called
	{"lip", STOFS(lip), F_INT, FFL_SPAWNTEMP},
	{"distance", STOFS(distance), F_INT, FFL_SPAWNTEMP},
	{"height", STOFS(height), F_INT, FFL_SPAWNTEMP},
	{"noise", STOFS(noise), F_LSTRING, FFL_SPAWNTEMP},
	{"pausetime", STOFS(pausetime), F_FLOAT, FFL_SPAWNTEMP},
	{"item", STOFS(item), F_LSTRING, FFL_SPAWNTEMP},

//need for item field in edict struct, FFL_SPAWNTEMP item will be skipped on saves
	{"item", FOFS(item), F_ITEM},

	{"gravity", STOFS(gravity), F_LSTRING, FFL_SPAWNTEMP},
	{"sky", STOFS(sky), F_LSTRING, FFL_SPAWNTEMP},
	{"skyrotate", STOFS(skyrotate), F_FLOAT, FFL_SPAWNTEMP},
	{"skyaxis", STOFS(skyaxis), F_VECTOR, FFL_SPAWNTEMP},
	{"minyaw", STOFS(minyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"maxyaw", STOFS(maxyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"minpitch", STOFS(minpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"maxpitch", STOFS(maxpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"nextmap", STOFS(nextmap), F_LSTRING, FFL_SPAWNTEMP},

	{0, 0, 0, 0}

};

/*
============
InitGame

This will be called when the dll is first loaded, which
only happens when a new game is started or a save game
is loaded.
============
*/
void InitGame (void)
{
    cvar_t *g_features;
    cvar_t *sv_features;

    gi.dprintf ("==== InitGame ====\n");

    init_genrand ((uint32)time(NULL));

    // these are used for server browser replies
    gi.cvar("time_remaining", "N/A", CVAR_SERVERINFO | CVAR_NOSET);
    gi.cvar("match_type", "N/A", CVAR_SERVERINFO | CVAR_NOSET);
    gi.cvar("Score_B", "N/A", CVAR_SERVERINFO | CVAR_NOSET);
    gi.cvar("Score_A", "N/A", CVAR_SERVERINFO | CVAR_NOSET);

    gun_x = gi.cvar("gun_x", "0", 0);
    gun_y = gi.cvar("gun_y", "0", 0);
    gun_z = gi.cvar("gun_z", "0", 0);

    //FIXME: sv_ prefix is wrong for these
    sv_rollspeed = gi.cvar("sv_rollspeed", "200", 0);
    sv_rollangle = gi.cvar("sv_rollangle", "2", 0);
    sv_maxvelocity = gi.cvar("sv_maxvelocity", "2000", 0);
    sv_gravity = gi.cvar("sv_gravity", "800", 0);

    // noset vars
    dedicated = gi.cvar("dedicated", "0", CVAR_NOSET);

    // latched vars
    sv_cheats = gi.cvar("cheats", "0", CVAR_SERVERINFO|CVAR_LATCH);
    gi.cvar("gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_LATCH);
    gi.cvar("gamedate", __DATE__ , CVAR_SERVERINFO | CVAR_LATCH);

    maxclients = gi.cvar("maxclients", "4", CVAR_SERVERINFO | CVAR_LATCH);
    maxentities = gi.cvar("maxentities", "1024", CVAR_LATCH);

    // change anytime vars
    dmflags = gi.cvar("dmflags", "0", CVAR_SERVERINFO);
    fraglimit = gi.cvar("fraglimit", "0", CVAR_SERVERINFO);
    timelimit = gi.cvar("timelimit", "0", CVAR_SERVERINFO);
    password = gi.cvar("password", "", CVAR_USERINFO);
    spectator_password = gi.cvar("spectator_password", "", CVAR_USERINFO);
    filterban = gi.cvar("filterban", "1", 0);

    // whether you can switch to an empty gun or not
    g_select_empty = gi.cvar("g_select_empty", "0", CVAR_ARCHIVE);

    run_pitch = gi.cvar("run_pitch", "0.002", 0);
    run_roll = gi.cvar("run_roll", "0.005", 0);
    bob_up  = gi.cvar("bob_up", "0.005", 0);
    bob_pitch = gi.cvar("bob_pitch", "0.002", 0);
    bob_roll = gi.cvar("bob_roll", "0.002", 0);

    // flood control
    flood_msgs = gi.cvar("flood_msgs", "8", 0);
    flood_persecond = gi.cvar("flood_persecond", "8", 0);
    flood_waitdelay = gi.cvar("flood_waitdelay", "10", 0);

    // dm map list
    sv_maplist = gi.cvar("sv_maplist", "", 0);


    // flood wave control
    flood_waves = gi.cvar("flood_waves", "6", 0);
    flood_waves_perminute = gi.cvar("flood_waves_perminute", "6", 0);
    flood_waves_waitdelay = gi.cvar("flood_waves_waitdelay", "10", 0);

    // random, farthest, fixed farthest
    g_1v1_spawn_mode = gi.cvar("g_1v1_spawn_mode", "1", 0);

    //max timeout via implicit timeout in 1v1 disconnect
    g_1v1_timeout = gi.cvar("g_1v1_timeout", "60", 0);

    // dm flags for 1v1 mode
    g_1v1flags = gi.cvar("g_1v1flags", "1040", 0);

    // password for "admin" command, sets ref status
    g_admin_password = gi.cvar ("g_admin_password", "", 0);

    // an admin's vote wins, vote is over
    g_admin_vote_decide = gi.cvar("g_admin_vote_decide", "1", 0);

    // let players change alias while a match is in progress
    g_allow_name_change_during_match = gi.cvar("g_allow_name_change_during_match", "1", 0);

    // let players vote an available LOCAL configuration file (moddir/configs/<configname>)
    g_allow_vote_config = gi.cvar("g_allow_vote_config", "1", 0);

    // enable the in-hud armor timer (using "armortimer" command)
    g_armor_timer = gi.cvar("g_armor_timer", "1", 0);

    // players can rejoin if they timeout/crash/ragequit if they have joincode set
    g_auto_rejoin_match = gi.cvar("g_auto_rejoin_match", "1", 0);

    //
    g_auto_rejoin_map = gi.cvar("g_auto_rejoin_map", "1", 0);

    // fix known bugs or let them be
    g_bugs = gi.cvar("g_bugs", "0", 0);

    // allow all chat or only players?
    g_chat_mode = gi.cvar("g_chat_mode", "0", 0);

    //
    g_command_mask = gi.cvar("g_command_mask", "-1", 0);

    // testing
    g_debug_spawns = gi.cvar("g_debug_spawns", "0", 0);

    // how quick weapons switch (0 is original quake 2 speed)
    g_fast_weap_switch = gi.cvar("g_fast_weap_switch", "0", 0);

    // stuff screenshot command to players on intermission
    g_force_screenshot = gi.cvar("g_force_screenshot", "0", 0);

    // stuff record command at start of match and stop at intermission
    g_force_record = gi.cvar("g_force_record", "0", 0);

    // TDM, 1v1, Insta
    g_gamemode = gi.cvar("g_gamemode", "0", CVAR_LATCH);
    g_gamemode->modified = false;

    // put a * next to captain in team games in the scoreboard
    g_highlight_captain = gi.cvar("g_highlight_captain", "0", 0);

    // libcurl - IP address to use for libcurl requests
    g_http_bind = gi.cvar("g_http_bind", "", 0);

    // domain name or IP address for API calls
    g_http_domain = gi.cvar("g_http_domain", "opentdm.org", CVAR_LATCH);

    // libcurl - debug messages
    g_http_debug = gi.cvar("g_http_debug", "0", 0);

    // libcurl - enable for webconfigs
    g_http_enabled = gi.cvar("g_http_enabled", "1", 0);

    // include slash at beginning and end. ex: /api/
    g_http_path = gi.cvar("g_http_path", "/configs/", CVAR_LATCH);

    // libcurl - proxy address for http access
    g_http_proxy = gi.cvar("g_http_proxy", "", 0);

    // players idle this long will be switched to specs (in seconds)
    g_idle_time = gi.cvar("g_idle_time", "300", 0);

    // seconds intermission lasts at end of match
    g_intermission_time = gi.cvar("g_intermission_time", "5", 0);

    // instagib dm flags
    g_itdmflags = gi.cvar("g_itdmflags", "142427", 0);

    // disabled items bitmask
    g_itemflags = gi.cvar("g_itemflags", "0", 0);

    // whether team names can be changed or not
    g_locked_names = gi.cvar("g_locked_names", "0", 0);

    // whether team skins can be changed
    g_locked_skins = gi.cvar("g_locked_skins", "0", 0);

    // which file is the maplist
    g_maplistfile = gi.cvar("g_maplistfile", "", 0);

    // seconds from ready to start
    g_match_countdown = gi.cvar("g_match_countdown", "15", 0);

    // match length in seconds
    g_match_time = gi.cvar("g_match_time", "600", 0);

    // player limit
    g_max_players_per_team = gi.cvar("g_max_players_per_team", "4", 0);

    // max timeout when called via cmd
    g_max_timeout = gi.cvar("g_max_timeout", "300", 0);

    // message displayed upon entering the game
    g_motd_message = gi.cvar("g_motd_message", "" , 0);

    // seconds added if tied
    g_overtime = gi.cvar("g_overtime", "60", 0);

    // display super low pings around this value instead of their real ping
    g_ping_handicap = gi.cvar("g_ping_handicap", "0", 0);

    // allow downloading of player configs on connect
    g_playerconfig_enabled = gi.cvar("g_playerconfig_enabled", "1", CVAR_LATCH);

    // powerups removed (bitmask)
    g_powerupflags = gi.cvar("g_powerupflags", "0", 0);

    // whether server should record multi-view demo of matches
    g_record_mvd = gi.cvar("g_record_mvd", "0", 0);

    // stop warmup players from shooting until match start
    g_ready_attention = gi.cvar("g_ready_attention", "0", 0);

    // seconds until fragged player repawns automatically
    g_respawn_time = gi.cvar("g_respawn_time", "5", 0);

    // respawn seconds, very rare to need to override these defaults
    g_respawn_weapon = gi.cvar("g_respawn_weapon", "30", 0);
    g_respawn_ammo = gi.cvar("g_respawn_ammo", "30", 0);
    g_respawn_armor = gi.cvar("g_respawn_armor", "20", 0);
    g_respawn_quad = gi.cvar("g_respawn_quad", "60", 0);
    g_respawn_invuln = gi.cvar("g_respawn_invuln", "300", 0);
    g_respawn_backpack = gi.cvar("g_respawn_backpack", "180", 0);
    g_respawn_adren = gi.cvar("g_respawn_adren", "60", 0);
    g_respawn_ps = gi.cvar("g_respawn_ps", "60", 0);

    //
    g_tdm_allow_pick = gi.cvar("g_tdm_allow_pick", "0", 0);

    //
    g_tdm_spawn_mode = gi.cvar("g_tdm_spawn_mode", "1", 0);

    // dm flags for TDM mode
    g_tdmflags = gi.cvar("g_tdmflags", "1040", 0);

    // first team name
    g_team_a_name = gi.cvar("g_team_a_name", "Hometeam", 0);

    // first team skin
    g_team_a_skin = gi.cvar("g_team_a_skin", "male/grunt", 0);

    // second team name
    g_team_b_name = gi.cvar("g_team_b_name", "Visitors", 0);

    // second team skin
    g_team_b_skin = gi.cvar("g_team_b_skin", "female/jezebel", 0);

    // teleporter behavior (stop forward motion or not)
    g_teleporter_nofreeze = gi.cvar("g_teleporter_nofreeze", "0", 0);

    // what happens if tied (add time, sudden death, just end tied)
    g_tie_mode = gi.cvar("g_tie_mode", "1", 0);

    // max number of timeouts allowed per client (0 == unlimited)
    g_timeout_limit = gi.cvar("g_timeout_limit", "2", 0);

    // whether or not to allow only team captains to call timeouts
    g_timeout_captain = gi.cvar("g_timeout_captain", "0", 0);

    // play a sound when a vote is called
    g_vote_attention = gi.cvar("g_vote_attention", "0", 0);

    // sound file to play when g_vote_attention is enabled
    g_vote_attention_sound = gi.cvar("g_vote_attention_sound", "misc/pc_up.wav", CVAR_LATCH);

    // which commands are voteable (-1 enables all)
    g_vote_mask = gi.cvar ("g_vote_mask", "-1", 0);

    // seconds votes last
    g_vote_time = gi.cvar ("g_vote_time", "30", 0);

    // enable weapon/ammo counters in the hud
    g_weapon_hud = gi.cvar("g_weapon_hud", "2", CVAR_LATCH);

    // enables the weapontimer command
    g_weapon_timer = gi.cvar("g_weapon_timer", "1", 0);

    // ...
    sv_mvd_enable = gi.cvar("sv_mvd_enable", "", CVAR_LATCH);

    // items
    InitItems();

    // initialize all entities for this game
    game.maxentities = maxentities->value;
    g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
    globals.edicts = g_edicts;
    globals.max_edicts = game.maxentities;

    // initialize all clients for this game
    game.maxclients = (int)maxclients->value;
    game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
    globals.num_edicts = game.maxclients+1;

    // game/server feature support - export what we support and read what the server supports

    //ensure it has NOSET if it didn't exist
    g_features = gi.cvar ("g_features", "0", CVAR_NOSET);
    gi.cvar_forceset ("g_features", va("%d", GMF_CLIENTNUM | GMF_WANT_ALL_DISCONNECTS | GMF_PROPERINUSE | GMF_MVDSPEC | GMF_VARIABLE_FPS));

    //init server features
    sv_features = gi.cvar ("sv_features", NULL, 0);
    if (sv_features) {
        game.server_features = (int)sv_features->value;
    } else {
        game.server_features = 0;
    }

    TDM_Init ();
}

/*
============
WriteGame

This will be called whenever the game goes to a new level,
and when the user explicitly saves the game.

Game information include cross level data, like multi level
triggers, help computer info, and all client states.

A single player death will automatically restore from the
last save position.
============
*/
void DummyWrite (const char *filename, qboolean autosave)
{
}

void DummyRead (const char *filename)
{
}

//==========================================================

