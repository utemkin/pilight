/*
	Copyright (C) 2013 - 2014 CurlyMo

	This file is part of pilight.

	pilight is free software: you can redistribute it and/or modify it under the
	terms of the GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option) any later
	version.

	pilight is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with pilight. If not, see	<http://www.gnu.org/licenses/>
*/

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "../core/json.h"
#include "../core/config.h"

struct config_t *config_settings;

int settings_find_number(const char *name, int *out);
int settings_find_string(const char *name, char **out);
void settings_init(void);

/*
 * Rewrite backported functions
 */
#include "../core/pilight.h"
int settings_select_number(enum origin_t origin, char *id, double *out);
int settings_select_string(enum origin_t origin, char *id, char **out);

#endif
