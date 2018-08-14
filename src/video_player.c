/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     video_player.c
 * Description:  Video player
 * Created:      Qua Jan 14 23:28:26 BRST 2009
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

#include "video_player.h"

#define MIN_BUFFER_START 10


static void create_frame(video_player *obj);
static void set_status_msg(video_player *obj, const char *msg);
static void start_gstreamer_plugin(video_player *obj);
static void vframe_callback(GtkWidget *widget, GdkEventButton *event, gpointer data);
static void mnu_aprate_callback(GtkWidget *menuitem, gpointer data);
static gboolean bus_player_callback(GstBus *bus, GstMessage *msg, gpointer data);


/**
 * vp_new
 * Instance new object video_player
 */
video_player *vp_new()
{
	video_player *obj;

	obj = (video_player*)g_malloc(sizeof(video_player));
	if(obj == NULL) {
		return(NULL);
	} else {
		obj->widget    = NULL;
		obj->sbar      = NULL;
		obj->socket    = NULL;
		obj->player    = NULL;
		obj->ch        = NULL;
		obj->state     = MOBOTV_VP_STATE_NULL;
		obj->kap_ratio = FALSE;

		// Create bar
		create_frame(obj);

		// Start gstreamer plugin
		start_gstreamer_plugin(obj);

		return(obj);
	}
}


/**
 * vp_destroy
 * Destroy a instanced object
 */
void vp_destroy(video_player *obj)
{
	if(obj == NULL) return;

	gst_element_set_state(obj->player, GST_STATE_NULL);
	gst_object_unref(G_OBJECT(obj->player));
	g_free(obj);
}


/**
 * Sets and Gets
 */
void vp_set_channel(video_player *obj, channel *ch)
{
	if(obj == NULL) return;

	obj->ch = ch;
}


channel *vp_get_channel(video_player *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->ch);
}


char vp_get_state(video_player *obj)
{
	if(obj == NULL) return(MOBOTV_VP_STATE_NULL);

	return(obj->state);
}


GtkWidget *vp_get_widget(video_player *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->widget);
}


gboolean vp_get_kap_ratio(video_player *obj)
{
	if(obj == NULL) return(FALSE);

	return(obj->kap_ratio);
}


void vp_set_kap_ratio(video_player *obj, gboolean state)
{
	if(obj == NULL) return;

	obj->kap_ratio = state;
	mnu_aprate_callback(NULL, obj);
}


/**
 * create_frame
 * Create video frame
 */
static void create_frame(video_player *obj)
{
	if(obj == NULL) return;

	GtkWidget *vbox   = gtk_vbox_new(FALSE, 1);
	GtkWidget *ebox1  = gtk_event_box_new();
	GtkWidget *sbar   = gtk_label_new("Initializing...");
	GtkWidget *ebox2  = gtk_event_box_new();
	GtkWidget *socket = gtk_socket_new();
	GdkColor c1, c2;

	// Video frame
	gdk_color_parse("black", &c2);
	gtk_widget_modify_bg(ebox2, GTK_STATE_NORMAL, &c2);
	gtk_container_add(GTK_CONTAINER(ebox2), socket);
	gdk_color_parse("black", &c1);
	gtk_widget_modify_bg(socket, GTK_STATE_NORMAL, &c1);
	gtk_widget_show(socket);
	gtk_widget_show_all(ebox2);

	// Status Bar
	gtk_misc_set_alignment(GTK_MISC(sbar), 0.01, 0);
	gtk_label_set_selectable(GTK_LABEL(sbar), FALSE);
	gdk_color_parse("white", &c1);
	gdk_color_parse("black", &c2);
	gtk_widget_modify_fg(sbar, GTK_STATE_NORMAL, &c1);
	gtk_widget_modify_bg(ebox1, GTK_STATE_NORMAL, &c2);
	gtk_container_add(GTK_CONTAINER(ebox1), sbar);
	gtk_widget_show_all(ebox1);

	// Frame
	gtk_box_pack_start(GTK_BOX(vbox), ebox2, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), ebox1, FALSE, TRUE, 0);
	gtk_widget_show_all(vbox);

	// Signal
	g_signal_connect(G_OBJECT(ebox2), "button_press_event", G_CALLBACK(vframe_callback), obj);


	obj->widget = vbox;
	obj->sbar   = sbar;
	obj->socket = socket;
}


/**
 * Set the message on status bar
 */
static void set_status_msg(video_player *obj, const char *msg)
{
	video_player *vplayer = obj;

	if(vplayer == NULL) {
		g_warning("set_status_msg: ooops, this is not a nice place to stay :(");
		return;
	}

	gtk_label_set_text(GTK_LABEL(vplayer->sbar), msg);
}


/**
 * GStreamer plugin initialization
 */
static void start_gstreamer_plugin(video_player *obj)
{
	video_player *vplayer = obj;
	GstBus *bus;
	GstElement *videosink;

	if(vplayer == NULL) {
		g_printerr("BUG: start_gstreamer_plugin(): obj = NULL");
		return;
	}

	vplayer->player = gst_element_factory_make("playbin", "player");
	if(!vplayer->player) {
		set_status_msg(vplayer, "GStreamer plugin could not be initialized.");
		g_warning("GStreamer plugin could not be initialized.");
		return;
	}

	videosink = gst_element_factory_make("ximagesink", "videosink");
	if(!videosink) {
		set_status_msg(vplayer, "GStreamer ximagesink plugin could not be initialized.");
		g_warning("GStreamer ximagesink plugin could not be initialized.");
	}
	g_object_set(vplayer->player, "video-sink", videosink, NULL);

	bus = gst_pipeline_get_bus(GST_PIPELINE(vplayer->player));
	gst_bus_add_watch(bus, bus_player_callback, obj);
	gst_object_unref(bus);

	gst_element_set_state(vplayer->player, GST_STATE_READY);
	set_status_msg(vplayer, "Ready.");
}


/**
 * Video commands
 */

/**
 * Play
 */
void vp_play(video_player *obj)
{
	video_player *vplayer = obj;
	GstElement *player;

	if(vplayer == NULL) {
		g_warning("vp_play: ooops, this is not a nice place to stay :(");
		return;
	} else {
		player = vplayer->player;

		if(vplayer->ch == NULL) {
			set_status_msg(vplayer, "No channel selected.");
			return;
		} else {
			set_status_msg(vplayer, "Please, wait...");
			gst_element_set_state(vplayer->player, GST_STATE_READY);
			g_object_set(G_OBJECT(vplayer->player), "uri", channel_get_url(vplayer->ch), NULL);
			set_status_msg(vplayer, "Starting...");

			gst_element_set_state(vplayer->player, GST_STATE_PLAYING);
		}
	}
}


/**
 * Stop
 */
void vp_stop(video_player *obj)
{
	video_player *vplayer = obj;
	GstElement *player;

	if(vplayer == NULL) {
		g_warning("vp_play: ooops, this is not a nice place to stay :(");
		return;
	} else {
		player = vplayer->player;

		if(vplayer->ch == NULL) {
			set_status_msg(vplayer, "No channel playing.");
			return;
		}
	}

	set_status_msg(vplayer, "Stopped.");
	gst_element_set_state(vplayer->player, GST_STATE_PAUSED);
	gst_element_set_state(vplayer->player, GST_STATE_READY);
}


/**
 * Callback for video frame pop-up menu
 */
static void vframe_callback(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	video_player *vplayer = (video_player*)data;
	GtkWidget *vmenu;
	GtkWidget *mnu_aprate;

	if(vplayer == NULL) {
		g_warning("vframe_callback: ooops, this is not a nice place to stay :(");
		return;
	}

	if(event->button == 3) {
		vmenu      = gtk_menu_new();
		mnu_aprate = gtk_check_menu_item_new_with_label("Keep aspect ratio");

		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(mnu_aprate), vplayer->kap_ratio);
		gtk_menu_shell_append(GTK_MENU_SHELL(vmenu), mnu_aprate);
	} else {
		return;
	}

	g_signal_connect(G_OBJECT(mnu_aprate), "activate", G_CALLBACK(mnu_aprate_callback), vplayer);
	gtk_menu_popup(GTK_MENU(vmenu), NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time());
	gtk_widget_show_all(vmenu);
}

/**
 * Callback for keep aspect rate item
 */
static void mnu_aprate_callback(GtkWidget *menuitem, gpointer data)
{
	video_player *vplayer = (video_player*)data;
	GstElement *videosink;
	gboolean aprate;

	if(vplayer == NULL) {
		g_warning("mnu_aprate_callback 1: ooops, this is not a nice place to stay :(");
		return;
	}

	if(vplayer->player != NULL) {
		g_object_get(vplayer->player, "video-sink", &videosink, NULL);
		if(!videosink) {
			g_warning("mnu_aprate_callback 2: ooops, this is not a nice place to stay :(");
			return;
		}

		if(vplayer->kap_ratio == TRUE) {
			aprate = FALSE;
		} else {
			aprate = TRUE;
		}
		vplayer->kap_ratio = aprate;

		// Set aspect ratio
		g_object_set(videosink, "force-aspect-ratio", aprate, NULL);
		g_object_unref(videosink);

		gst_element_set_state(vplayer->player, GST_STATE_READY);
		if(vplayer->state == MOBOTV_VP_STATE_PLAYING)
			gst_element_set_state(vplayer->player, GST_STATE_PLAYING);
	}
}


/**
 * Bus messages
 */
static gboolean bus_player_callback(GstBus *bus, GstMessage *msg, gpointer data)
{
	video_player *vplayer = (video_player*)data;
	GString *msgbuf;
	GstElement *videosink;

	if(vplayer == NULL) {
		g_warning("bus_player_callback: ooops, this is not a nice place to stay :(");
		return(TRUE);
	}

	switch(GST_MESSAGE_TYPE(msg)) {

		case GST_MESSAGE_ELEMENT:
			/* Grab video to GtkWindow */
			if ( gst_is_video_overlay_prepare_window_handle_message(msg) ) {
				g_object_get(vplayer->player, "video-sink", &videosink, NULL);
				gulong xid = gtk_socket_get_id(GTK_SOCKET(vplayer->socket));
				gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY(GST_MESSAGE_SRC(msg)), xid);
				g_object_unref(videosink);
			}
			break;

		case GST_MESSAGE_EOS:
			set_status_msg(vplayer, "End of stream.");
			break;

		case GST_MESSAGE_BUFFERING: {
			gint percent = 0;

			gst_message_parse_buffering(msg, &percent);

			// Check buffer percentage
			if(percent < MIN_BUFFER_START) {
				gst_element_set_state(vplayer->player, GST_STATE_PAUSED);
				vplayer->state = MOBOTV_VP_STATE_PAUSED;
			} else {
				if(vplayer->state == MOBOTV_VP_STATE_PAUSED) {
					gst_element_set_state(vplayer->player, GST_STATE_PLAYING);
					vplayer->state = MOBOTV_VP_STATE_PLAYING;
				}
			}

			// Show message
			msgbuf = g_string_new(" ");
			if(msgbuf != NULL) {
				g_string_printf(msgbuf, "Buffering... %d%%", percent);
				set_status_msg(vplayer, msgbuf->str);
				g_string_free(msgbuf, TRUE);
			}
			break;
		}

		case GST_MESSAGE_STATE_CHANGED: {
			GstState state;

			switch( gst_element_get_state(vplayer->player, &state, NULL, 3000) ) {

				case GST_STATE_CHANGE_SUCCESS: {
					vplayer->state = state;

					switch( state ) {
						case GST_STATE_READY || GST_STATE_NULL:
							set_status_msg(vplayer, "Ready.");
							break;

						case GST_STATE_PAUSED:
							set_status_msg(vplayer, "Paused.");
							break;

						case GST_STATE_PLAYING:
							set_status_msg(vplayer, "Playing");
							break;

						default:
							break;
					}
					break;
				}

				default:
					break;
			}
			break;
		}

		case GST_MESSAGE_ERROR: {
			GError *error;
			gchar *debug;

			msgbuf = g_string_new("Error: ");
			if(msgbuf != NULL) {
				gst_message_parse_error(msg, &error, &debug);
				g_free(debug);

				g_string_append(msgbuf, error->message);
				set_status_msg(vplayer, msgbuf->str);

				g_string_free(msgbuf, TRUE);
				g_error_free(error);
			}
			vplayer->state = MOBOTV_VP_STATE_NULL;
			break;
		}

		default:
			break;
	}

	return(TRUE);
}


