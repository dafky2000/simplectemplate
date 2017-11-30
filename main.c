/*
 * main.c
 * https://github.com/dafky2000/simplectemplate
 * Some examples for using simplectemplate
 *
 * Copyright 2017 Daniel Kelly <myself@danielkelly.me>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include <stdio.h>
#include "src/template_functions.h"

int main () {
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	char* rendered = render_template_file2("spec/templates/basic.txt", 2, data);

	printf("%s", rendered);

	if(rendered) free(rendered);

	return 0;
}
