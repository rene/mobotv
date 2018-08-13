/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ch_list.h
 * Description:  Channel list
 * Created:      Sáb Jan 10 20:44:02 BRST 2009
 *
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MOBOTV_CH_LIST_H

	#define MOBOTV_CH_LIST_H 1


	#include <glib.h>
	#include <stdio.h>
	#include <string.h>
	#include <libxml/tree.h>
	#include <libxml/parser.h>
	#include "channel.h"


	typedef struct _ch_list {
		GSList *chlist;
	} ch_list;


	ch_list *chlist_new();
	void chlist_destroy(ch_list *obj);

	gboolean chlist_add_channel(ch_list *obj, channel *ch);
	gboolean chlist_del_channel(ch_list *obj, const char *chname);
	gboolean chlist_del_channel_byref(ch_list *obj, channel *ch);

	channel *chlist_get_channel_by_name(ch_list *obj, const char *chname);

	guint chlist_get_length(ch_list *obj);

	channel *chlist_get_channel_at(ch_list *obj, guint n);
	gint chlist_get_channel_index(ch_list *obj, channel *ch);

	gboolean chlist_read_from_file(ch_list *obj, const char *filename);
	gboolean chlist_write_to_file(ch_list *obj, const char *filename);

#endif

