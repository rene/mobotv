/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ch_view.h
 * Description:  Channels icon view widget
 * Created:      Dom Jan 11 03:44:35 BRST 2009
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

#ifndef MOBOTV_CH_VIEW_H

	#define MOBOTV_CH_VIEW_H 1


	#include <gtk/gtk.h>
	#include "user_conf.h"
	#include "ch_list.h"
	#include "ch_info.h"
	#include "video_player.h"
	#include "video_ctrl.h"
	#include "ui.h"


	typedef struct _ch_view {
		ch_list *chlist;
		ch_info *chinfo;
		video_player *vplayer;
		video_ctrl *vctrl;
		GtkWidget *widget;
		GtkWidget *ibox;
		GtkWidget *tree;
		GtkListStore *list_store;
		char view_mode;
		char *chname;
	} ch_view;


	ch_view *chview_new(ch_list *chlist, ch_info *chinfo, video_ctrl *vctrl, video_player *vplayer);
	void chview_destroy(ch_view *obj);

	GtkWidget *chview_get_widget(ch_view *obj);

	void chview_set_chlist(ch_view *obj, ch_list *chlist);

	void chview_set_view_mode(ch_view *obj, char view_mode);
	char chview_get_view_mode(ch_view *obj);

	char *chview_get_selected_channel(ch_view *obj);

	void chview_update(ch_view *obj);

#endif


