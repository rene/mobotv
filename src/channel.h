/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     channel.h
 * Description:  Channel class
 * Created:      Sex Jan  9 10:43:08 BRST 2009
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

#ifndef MOBOTV_CHANNEL_H

	#define MOBOTV_CHANNEL_H 1


	#include <gtk/gtk.h>
	#include "ui.h"


	typedef struct _channel {
		char *name;
		char *url;
		char *info;
		char *icon_path;
		GtkWidget *icon_img;
	} channel;


	channel *channel_new(void);
	void channel_destroy(channel *obj);

	void channel_set_name(channel *obj, const char *name);
	void channel_set_url(channel *obj, const char *url);

	void channel_set_info(channel *obj, const char *info);
	char *channel_get_info(channel *obj);

	char *channel_get_name(channel *obj);
	char *channel_get_url(channel *obj);

	void channel_set_icon_path(channel *obj, const char *info_path);
	char *channel_get_icon_path(channel *obj);
	GtkWidget *channel_get_icon_img(channel *obj);

#endif

