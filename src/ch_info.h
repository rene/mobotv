/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ch_info.h
 * Description:  Show channel information
 * Created:      Sáb Jan 10 14:31:12 BRST 2009
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

#ifndef MOBOTV_CH_INFO_H

	#define MOBOTV_CH_INFO_H 1


	#include <gtk/gtk.h>
	#include <hildon/hildon-program.h>
	#include "channel.h"
	#include "ui.h"


	typedef struct _ch_info {
		GtkWidget *widget;
		GtkWidget *txt_widget;
		channel *ch;
		char *info;
	} ch_info;


	ch_info *chinfo_new();
	void chinfo_destroy(ch_info *obj);

	void chinfo_set_widget(ch_info *obj, GtkWidget *wd);
	GtkWidget *chinfo_get_widget(ch_info *obj);

	void chinfo_set_txt_widget(ch_info *obj, GtkWidget *wd);
	GtkWidget *chinfo_get_txt_widget(ch_info *obj);

	void chinfo_set_channel(ch_info *obj, channel *ch);
	channel *ch_info_get_channel(ch_info *obj);

	char *chinfo_get_info(ch_info *obj);

#endif

