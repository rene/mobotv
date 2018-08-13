/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     application.h
 * Description:  Application class
 * Created:      Sáb Jan 10 16:56:45 BRST 2009
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

#ifndef MOBOTV_APPLICATION_H

	#define MOBOTV_APPLICATION_H

	#include <gtk/gtk.h>
	#include <hildon/hildon-program.h>
	#include "video_ctrl.h"
	#include "video_player.h"
	#include "ch_info.h"
	#include "ch_list.h"
	#include "ch_view.h"
	#include "user_conf.h"


	typedef struct _application {
		HildonWindow *main_win;
		ch_info      *chinfo;
		GtkWidget    *frm_inf;
		ch_list      *chlist;
		GtkWidget    *frm_ch;
		ch_view      *chview;
		video_ctrl   *vbar;
		video_player *vplayer;
		user_conf    *prog_conf;
	} application;


	application *app_new(void);
	void app_destroy(application *obj);

	void app_set_main_win(application *obj, HildonWindow *main_win);
	HildonWindow *app_get_main_win(application *obj);

	void app_set_chinfo(application *obj, ch_info *chinfo);
	ch_info *app_get_chinfo(application *obj);

	void app_set_frm_inf(application *obj, GtkWidget *frm_inf);
	GtkWidget *app_get_frm_inf(application *obj);

	void app_set_chlist(application *obj, ch_list *chlist);
	ch_list *app_get_chlist(application *obj);

	void app_set_chview(application *obj, ch_view *chview);
	ch_view *app_get_chview(application *obj);

	void app_set_frm_ch(application *obj, GtkWidget *frm_ch);
	GtkWidget *app_get_frm_ch(application *obj);

	void app_set_vbar(application *obj, video_ctrl *vbar);
	video_ctrl *app_get_vbar(application *obj);

	void app_set_vplayer(application *obj, video_player *vplayer);
	video_player *app_get_vplayer(application *obj);

	void app_set_prog_conf(application *obj, user_conf *prog_conf);
	user_conf *app_get_prog_conf(application *obj);

#endif

