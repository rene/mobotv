/**
 * Copyright (C)2007 Renê de Souza Pinto	<rene@renesp.com.br>
 *
 * libconffile - Library to manage configuration files in a easy way
 *
 * File: conffile.h
 * Contents: Prototypes 
 *
 * Created: 23/08/2008
 * Updated: 24/08/2008
 *
 *  This file is part of libconffile.
 * 
 *  libconffile is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation a, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  libconffile is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 * 
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with libconffile.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CONFFILE_H_

	#define _CONFFILE_H_ 1

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/types.h>
	#include <regex.h>
	#include <errno.h>


 #ifdef __cplusplus
 	extern "C" {
 #endif

	typedef struct _CONFFILE_ {
		char *filename;
		char **contents;
		long nlines;
		char *str;
	} conffile_t;

	int new_conffile(conffile_t **conf);
	void free_conffile(conffile_t **conf);
	int load_from_file(char *filename, conffile_t **conf);
	int save_to_file(char *filename, conffile_t *conf);
	int set_key(char *key, char *value, conffile_t **conf);
	char *get_value(char *key, conffile_t **conf);
	int remove_key(char *key, conffile_t **conf);

 #ifdef __cplusplus
 	}
 #endif

#endif /* _CONFFILE_H_ */

