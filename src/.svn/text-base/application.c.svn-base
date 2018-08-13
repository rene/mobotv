/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     application.c
 * Description:  Application class
 * Created:      Sáb Jan 10 16:55:52 BRST 2009
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

#include "application.h"

/**
 * app_new
 * Instance new application object
 */
application *app_new(void)
{
	application *obj;

	obj = (application*)g_malloc(sizeof(application));
	if(obj == NULL) {
		return(NULL);
	} else {
		obj->chinfo    = NULL;
		obj->frm_inf   = NULL;
		obj->chlist    = NULL;
		obj->frm_ch    = NULL;
		obj->chview    = NULL;
		obj->vbar      = NULL;
		obj->vplayer   = NULL;
		obj->prog_conf = NULL;
		return(obj);
	}
}


/**
 * app_destroy
 * Destroy a instanced object
 */
void app_destroy(application *obj)
{
	if(obj == NULL) return;

 g_free(obj);
}


/**
 * Sets and Gets
 */
void app_set_main_win(application *obj, HildonWindow *main_win)
{
	if(obj == NULL) return;

	obj->main_win = main_win;
}


HildonWindow *app_get_main_win(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->main_win);
}


void app_set_chinfo(application *obj, ch_info *chinfo)
{
	if(obj == NULL) return;

	if(obj->chinfo != NULL) chinfo_destroy(obj->chinfo);
	obj->chinfo = chinfo;
}


ch_info *app_get_chinfo(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->chinfo);
}


void app_set_frm_inf(application *obj, GtkWidget *frm_inf)
{
	if(obj == NULL) return;

	obj->frm_inf = frm_inf;
}


GtkWidget *app_get_frm_inf(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->frm_inf);
}


void app_set_chlist(application *obj, ch_list *chlist)
{
	if(obj == NULL) return;

	if(obj->chlist != NULL) chlist_destroy(obj->chlist);
	obj->chlist = chlist;
}


ch_list *app_get_chlist(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->chlist);
}


void app_set_frm_ch(application *obj, GtkWidget *frm_ch)
{
	if(obj == NULL) return;

	obj->frm_ch = frm_ch;
}


GtkWidget *app_get_frm_ch(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->frm_ch);
}


void app_set_chview(application *obj, ch_view *chview)
{
	if(obj == NULL) return;

	if(obj->chview != NULL) chview_destroy(obj->chview);
	obj->chview = chview;
}


ch_view *app_get_chview(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->chview);
}


void app_set_vbar(application *obj, video_ctrl *vbar)
{
	if(obj == NULL) return;

	if(obj->vbar != NULL) vctrl_destroy(obj->vbar);
	obj->vbar = vbar;
}


video_ctrl *app_get_vbar(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->vbar);
}


void app_set_vplayer(application *obj, video_player *vplayer)
{
	if(obj == NULL) return;

	if(obj->vplayer != NULL) vp_destroy(obj->vplayer);
	obj->vplayer = vplayer;
}


video_player *app_get_vplayer(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->vplayer);
}


void app_set_prog_conf(application *obj, user_conf *prog_conf)
{
	if(obj == NULL) return;

	if(obj->prog_conf != NULL) uconf_destroy(obj->prog_conf);
	obj->prog_conf = prog_conf;
}


user_conf *app_get_prog_conf(application *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->prog_conf);
}


