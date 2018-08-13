/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     video_ctrl.h
 * Description:  Video control bar
 * Created:      Qui Jan  8 12:17:13 BRST 2009
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

#ifndef MOBOTV_VIDEO_CTRL_H

	#define MOBOTV_VIDEO_CTRL_H 1


	#include <gtk/gtk.h>
	#include "video_player.h"
	#include "ch_list.h"
	#include "channel.h"
	#include "ui.h"


	typedef struct _video_ctrl {
		GtkWidget *widget;
		GtkWidget *ibutton_wg;
		GtkWidget *binfo;
		video_player *vplayer;
		ch_list *chlist;
		channel *ch;
	} video_ctrl;

	// prototypes
	video_ctrl *vctrl_new(video_player *vplayer);
	void vctrl_destroy(video_ctrl *obj);

	GtkWidget *vctrl_get_widget(video_ctrl *obj);

	ch_list *vctrl_get_chlist(video_ctrl *obj);
	void vctrl_set_chlist(video_ctrl *obj, ch_list *chlist);

	void vctrl_set_channel(video_ctrl *obj, channel *ch);

	void vctrl_set_ibutton_widget(video_ctrl *obj, GtkWidget *wd);

#endif


