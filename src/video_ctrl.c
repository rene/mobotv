/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     video_ctrl.c
 * Description:  Video control bar
 * Created:      Qui Jan  8 12:09:36 BRST 2009
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

#include "video_ctrl.h"


static void create_bar(video_ctrl *obj);
static void ibutton_callback(GtkWidget *widget, gpointer data);
static void ibutton_show_callback(GtkWidget *widget, gpointer data);
static void ibutton_hide_callback(GtkWidget *widget, gpointer data);
static void play_button_callback(GtkWidget *widget, gpointer data);
static void stop_button_callback(GtkWidget *widget, gpointer data);
static void bprev_button_callback(GtkWidget *widget, gpointer data);
static void bnext_button_callback(GtkWidget *widget, gpointer data);


/**
 * vctrl_new
 * Instance new object video_ctrl
 */
video_ctrl *vctrl_new(video_player *vplayer)
{
	video_ctrl *obj;

	obj = (video_ctrl*)g_malloc(sizeof(video_ctrl));
	if(obj == NULL) {
		return(NULL);
	} else {
		obj->ch      = NULL;
		obj->chlist  = NULL;
		obj->vplayer = vplayer;

		// Create bar
		create_bar(obj);
		return(obj);
	}
}


/**
 * vctrl_destroy
 * Destroy a instanced object
 */
void vctrl_destroy(video_ctrl *obj)
{
 g_free(obj);
}


/**
 * vctrl_get_widget
 * Return the video control widget
 */
GtkWidget *vctrl_get_widget(video_ctrl *obj)
{
	if(obj != NULL) {
		return(obj->widget);
	} else {
		return(NULL);
	}
}


/**
 * vctrl_set_ibutton_widget
 * Set widget to show/hide with info button
 */
void vctrl_set_ibutton_widget(video_ctrl *obj, GtkWidget *wd)
{
	obj->ibutton_wg = wd;
	g_signal_connect(G_OBJECT(wd), "show" , G_CALLBACK(ibutton_show_callback), obj->binfo);
	g_signal_connect(G_OBJECT(wd), "hide" , G_CALLBACK(ibutton_hide_callback), obj->binfo);
}


ch_list *vctrl_get_chlist(video_ctrl *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->chlist);
}


void vctrl_set_chlist(video_ctrl *obj, ch_list *chlist)
{
	if(obj == NULL) return;

	obj->chlist = chlist;
}


void vctrl_set_channel(video_ctrl *obj, channel *ch)
{
	if(obj == NULL) return;

	obj->ch = ch;
}


/**
 * create_bar
 * Create video control bar
 */
static void create_bar(video_ctrl *obj)
{
	if(obj == NULL) return;

	GtkWidget *hbbox = gtk_hbutton_box_new();
	GtkWidget *bprev = new_img_button(MOBOTV_ICON_PREVIOUS);
	GtkWidget *bplay = new_img_button(MOBOTV_ICON_PLAY);
	GtkWidget *bstop = new_img_button(MOBOTV_ICON_STOP);
	GtkWidget *bnext = new_img_button(MOBOTV_ICON_NEXT);
	GtkWidget *binfo = new_img_toggle_button(MOBOTV_ICON_INFO);

	gtk_container_add(GTK_CONTAINER(hbbox), bprev);
	gtk_container_add(GTK_CONTAINER(hbbox), bplay);
	gtk_container_add(GTK_CONTAINER(hbbox), bstop);
	gtk_container_add(GTK_CONTAINER(hbbox), bnext);
	gtk_container_add(GTK_CONTAINER(hbbox), binfo);

	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbbox), GTK_BUTTONBOX_START);
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbbox), 1);
	gtk_button_box_set_child_size(GTK_BUTTON_BOX(hbbox), 48, 48);

	// signals
	g_signal_connect(G_OBJECT(bplay), "clicked", G_CALLBACK(play_button_callback), obj);
	g_signal_connect(G_OBJECT(bstop), "clicked", G_CALLBACK(stop_button_callback), obj);
	g_signal_connect(G_OBJECT(bprev), "clicked", G_CALLBACK(bprev_button_callback), obj);
	g_signal_connect(G_OBJECT(bnext), "clicked", G_CALLBACK(bnext_button_callback), obj);
	g_signal_connect(G_OBJECT(binfo), "toggled", G_CALLBACK(ibutton_callback), obj);

	obj->binfo  = binfo;
	obj->widget = hbbox;
}


/**
 * Callbacks
 */
static void ibutton_callback(GtkWidget *widget, gpointer data)
{
	video_ctrl *vbar = (video_ctrl*)data;

	if ( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) )
    {
        gtk_widget_show(vbar->ibutton_wg);
    } else {
        gtk_widget_hide(vbar->ibutton_wg);
    }
}

static void ibutton_show_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget *bt = (GtkWidget *)data;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bt), TRUE);
}

static void ibutton_hide_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget *bt = (GtkWidget *)data;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bt), FALSE);
}


static void play_button_callback(GtkWidget *widget, gpointer data)
{
	video_ctrl *vctrl = (video_ctrl*)data;

	if(vctrl == NULL) {
		g_warning("play_button_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		vp_play(vctrl->vplayer);
	}
}

static void stop_button_callback(GtkWidget *widget, gpointer data)
{
	video_ctrl *vctrl = (video_ctrl*)data;

	if(vctrl == NULL) {
		g_warning("stop_button_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		vp_stop(vctrl->vplayer);
	}
}

static void bprev_button_callback(GtkWidget *widget, gpointer data)
{
	video_ctrl *vctrl = (video_ctrl*)data;
	gint len;
	gint ch_pos;
	channel *ch;

	if(vctrl == NULL) {
		g_warning("stop_button_callback: ooops, this is not a nice place to stay :(");
		return;
	}

	len    = chlist_get_length(vctrl->chlist);
	ch_pos = chlist_get_channel_index(vctrl->chlist, vctrl->ch);
	ch_pos--;
	if(ch_pos < 0)
		ch_pos = (len - 1);

	ch = chlist_get_channel_at(vctrl->chlist, ch_pos);
	if(ch != NULL) {
		vctrl_set_channel(vctrl, ch);
		vp_set_channel(vctrl->vplayer, vctrl->ch);
		vp_play(vctrl->vplayer);
	}
}

static void bnext_button_callback(GtkWidget *widget, gpointer data)
{
	video_ctrl *vctrl = (video_ctrl*)data;
	gint len;
	gint ch_pos;
	channel *ch;

	if(vctrl == NULL) {
		g_warning("stop_button_callback: ooops, this is not a nice place to stay :(");
		return;
	}

	len    = chlist_get_length(vctrl->chlist);
	ch_pos = chlist_get_channel_index(vctrl->chlist, vctrl->ch);
	ch_pos++;
	if(ch_pos >= len)
		ch_pos = 0;

	ch = chlist_get_channel_at(vctrl->chlist, ch_pos);
	if(ch != NULL) {
		vctrl_set_channel(vctrl, ch);
		vp_set_channel(vctrl->vplayer, vctrl->ch);
		vp_play(vctrl->vplayer);
	}
}


