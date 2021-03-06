/*
	Copyright (C) 2013 - 2016 CurlyMo

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _LUA_CONFIG_H_
#define _LUA_CONFIG_H_

#include "lua.h"
#include "../datatypes/stack.h"

typedef struct plua_device_t {
	char *name;
	void *data;
} plua_device_t;

extern int plua_config_device(struct lua_State *L);
extern int plua_config_setting(struct lua_State *L);

static const luaL_Reg pilight_config_lib[] = {
	{"device", plua_config_device},
	{"setting", plua_config_setting},
	{NULL, NULL}
};

#endif