/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     video_player.h
 * Description:  Video player
 * Created:      Qua Jan 14 23:29:11 BRST 2009
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

#ifndef MOBOTV_VIDEO_PLAYER_H

	#define MOBOTV_VIDEO_PLAYER_H 1


	#include <gtk/gtk.h>
	#include <gst/gst.h>
	#include <gst/interfaces/xoverlay.h>
	#include <glib/gprintf.h>
	#include "channel.h"
	#include "ui.h"


	#define MOBOTV_VP_STATE_NULL		GST_STATE_NULL
	#define MOBOTV_VP_STATE_PLAYING		GST_STATE_PLAYING
	#define MOBOTV_VP_STATE_PAUSED		GST_STATE_PAUSED
	#define MOBOTV_VP_STATE_READY		GST_STATE_READY


	typedef struct _video_player {
		GtkWidget *widget;
		GtkWidget *sbar;
		GtkWidget *socket;
		GstElement *player;
		channel *ch;
		char state;
		gboolean kap_ratio;
	} video_player;


	video_player *vp_new();
	void vp_destroy(video_player *obj);

	void vp_set_channel(video_player *obj, channel *ch);
	channel *vp_get_channel(video_player *obj);

	GtkWidget *vp_get_widget(video_player *obj);

	char vp_get_state(video_player *obj);
	void vp_set_kap_ratio(video_player *obj, gboolean state);
	gboolean vp_get_kap_ratio(video_player *obj);
	void vp_play(video_player *obj);
	void vp_stop(video_player *obj);

#endif


