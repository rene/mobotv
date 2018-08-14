/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     main.c
 * Description:  Main function and interface
 * Created:      Qui Jan  8 02:25:35 BRST 2009
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

#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gdk/gdkkeysyms.h>
#include "application.h"
#include "video_ctrl.h"
#include "video_player.h"
#include "chmg_win.h"
#include "conf_win.h"
#include "about_win.h"
#include "ui.h"
#include "channel.h"
#include "ch_info.h"
#include "ch_view.h"
#include "ch_list.h"
#include "input_dialog.h"
#include "user_conf.h"


typedef struct _main_lapp {
	application *app;
	GMainLoop *mloop;
} main_lapp;

typedef struct _t_chlist {
	GtkWidget *dialog;
	ch_list *chlist;
	char *filename;
	gboolean res;
	gboolean done;
} t_chlist;


/** prototypes */
static void mobotv_quit(GtkWidget *widget, gpointer data);
static void mnu_open_callback(GtkWidget *widget, gpointer data);
static void smnu_url_callback(GtkWidget *widget, gpointer data);
static void mnu_mgt_ch_callback(GtkWidget *widget, gpointer data);
static void mnu_prefs_callback(GtkWidget *widget, gpointer data);
static void mnu_about_callback(GtkWidget *widget, gpointer data);
static void smnu_local_callback(GtkWidget *widget, gpointer data);
static void load_ch_list(char *filename, ch_list *chlist, ch_view *chview, GtkWidget *main_win);
static gpointer load_ch_list_thread(gpointer data);


/** 
 * main
 */
int main(int argc, char **argv)
{
	GMainLoop *mloop;
	main_lapp mlapp;

	GtkWidget  *main_win;

	GtkWidget *pane1;
	GtkWidget *pane2;
	GtkWidget *frame_ch;
	GtkWidget *chview_wg;
	GtkWidget *frame_inf;
	GtkWidget *video_vb;
	GtkWidget *vp_wg;
	GtkWidget *vbar_wg;
	GtkWidget *chinfo_wg;

	GtkWidget *menu_bar;
	GtkWidget *menu;
	GtkWidget *root_menu;

	GtkWidget *mnu_open;
	GtkWidget *mnu_load_list;
	GtkWidget *smnu_load_list;
	GtkWidget *smnu_url;
	GtkWidget *smnu_local;
	GtkWidget *mnu_mgt_ch;
	GtkWidget *mnu_prefs;
	GtkWidget *mnu_about;
	GtkWidget *mnu_quit;

	GtkAccelGroup *accel_group = NULL;


	// instance gtk, main loop and hildon
	gtk_init(&argc, &argv);
	mloop    = g_main_loop_new(NULL, FALSE);
    //mobotv   = g_application_new("", G_APPLICATION_FLAGS_NONE);
    main_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// instance gstreamer
	gst_init(&argc, &argv);

	// instance objects
	application  *app       = app_new();
	user_conf    *prog_conf = uconf_new();
	video_player *vplayer   = vp_new();
	video_ctrl   *vbar      = vctrl_new(vplayer);
	ch_info      *chinfo    = chinfo_new();
	ch_list      *chlist    = chlist_new();
	ch_view      *chview    = chview_new(chlist, chinfo, vbar, vplayer);


	mlapp.app   = app;
	mlapp.mloop = mloop;

	pane1     = gtk_hpaned_new();
	pane2     = gtk_vpaned_new();
	frame_ch  = gtk_frame_new("Channels");
	frame_inf = gtk_frame_new("Channel information");
	video_vb  = gtk_vbox_new(FALSE, 2);
	vp_wg     = vp_get_widget(vplayer);
	vbar_wg   = vctrl_get_widget(vbar);
	chview_wg = chview_get_widget(chview);
	chinfo_wg = chinfo_get_widget(chinfo);

	if( app       == NULL || vplayer   == NULL || vbar   == NULL ||
		chinfo    == NULL || chlist    == NULL || chview == NULL ||
		prog_conf == NULL) {

			g_printerr("fatal error: free memory is not enough to continue.");
			return(EXIT_FAILURE);
	} else {
		app_set_main_win(app, main_win);
		app_set_vbar(app, vbar);
		app_set_vplayer(app, vplayer);
		app_set_chinfo(app, chinfo);
		app_set_frm_inf(app, frame_inf);
		app_set_chlist(app, chlist);
		app_set_chview(app, chview);
		app_set_frm_ch(app, frame_ch);
		app_set_prog_conf(app, prog_conf);

		chview_set_chlist(chview, chlist);
		vctrl_set_chlist(vbar, chlist);
	}

	// init program configuration
	uconf_set_conf_filename(prog_conf, MOBOTV_CONF_FILE);
	uconf_read_conf(prog_conf);
	chlist_read_from_file(chlist, uconf_get_ch_list_path(prog_conf) );

	// main win
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(main_win), accel_group);
	gtk_window_set_icon_from_file(GTK_WINDOW(main_win), MOBOTV_ICON, NULL);

	// menu
    menu = gtk_menu_new();

	mnu_open       = new_img_menu_item("Open url...", MOBOTV_ICON_OPEN);
	mnu_load_list  = gtk_menu_item_new_with_label("Load Channel's list from");
	smnu_load_list = gtk_menu_new();
	smnu_url       = gtk_menu_item_new_with_label("Url...");
	smnu_local     = gtk_menu_item_new_with_label("Local...");
	mnu_mgt_ch     = new_img_menu_item("Channel manager", MOBOTV_ICON_CHMG);
	mnu_prefs      = new_img_menu_item("Preferences", MOBOTV_ICON_PREFS);
	mnu_about      = new_img_menu_item("About", MOBOTV_ICON_ABOUT);
	mnu_quit       = new_img_menu_item("Quit", MOBOTV_ICON_QUIT);

	gtk_widget_add_accelerator(mnu_open, "activate", accel_group,
		      GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(mnu_prefs, "activate", accel_group,
		      GDK_p, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(mnu_quit, "activate", accel_group,
		      GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mnu_open);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mnu_load_list);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(mnu_load_list), smnu_load_list);
	gtk_menu_shell_append(GTK_MENU_SHELL(smnu_load_list), smnu_url);
	gtk_menu_shell_append(GTK_MENU_SHELL(smnu_load_list), smnu_local);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mnu_mgt_ch);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mnu_prefs);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), mnu_about);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mnu_quit);

	menu_bar  = gtk_menu_bar_new();
	root_menu = gtk_menu_item_new_with_label ("Menu");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(root_menu), menu);
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), root_menu);

	// main
	vctrl_set_ibutton_widget(vbar, frame_inf);
	gtk_frame_set_shadow_type(GTK_FRAME(frame_ch), GTK_SHADOW_ETCHED_IN);
	gtk_container_add(GTK_CONTAINER(frame_ch), chview_wg);

	GtkWidget *vb_mwin = gtk_vbox_new (FALSE, 0);
	gtk_container_add(GTK_CONTAINER (main_win), vb_mwin);
	gtk_box_pack_start(GTK_BOX(vb_mwin), menu_bar, FALSE, FALSE, 2);
	gtk_box_pack_end(GTK_BOX(vb_mwin), pane1, TRUE, TRUE, 2);
	gtk_widget_show(vb_mwin);
	gtk_widget_show(menu_bar);
	gtk_widget_show(menu);

	//gtk_container_add(GTK_CONTAINER(main_win), pane1);
	gtk_paned_pack1(GTK_PANED(pane1), frame_ch, TRUE, FALSE);
	gtk_widget_set_size_request(frame_ch, 180, -1);
	gtk_paned_pack2(GTK_PANED(pane1), pane2, TRUE, FALSE);

	gtk_paned_pack1(GTK_PANED(pane2), video_vb, TRUE, FALSE);
	gtk_widget_set_size_request(video_vb, 400, 200);
	gtk_paned_pack2(GTK_PANED(pane2), frame_inf, TRUE, FALSE);
	gtk_widget_set_size_request(frame_inf, -1, 100);
	gtk_container_add(GTK_CONTAINER(frame_inf), chinfo_wg);

	gtk_box_pack_start(GTK_BOX(video_vb), vp_wg, TRUE, TRUE, 2);
	gtk_box_pack_end(GTK_BOX(video_vb), vbar_wg, FALSE, FALSE, 2);

	gtk_window_set_default_size(GTK_WINDOW(main_win), 800, 450);

	// callbacks
	g_signal_connect(G_OBJECT(mnu_open),   "activate", G_CALLBACK(mnu_open_callback), app);
	g_signal_connect(G_OBJECT(smnu_url),   "activate", G_CALLBACK(smnu_url_callback), app);
	g_signal_connect(G_OBJECT(smnu_local), "activate", G_CALLBACK(smnu_local_callback), app);
	g_signal_connect(G_OBJECT(mnu_mgt_ch), "activate", G_CALLBACK(mnu_mgt_ch_callback), app);
	g_signal_connect(G_OBJECT(mnu_prefs),  "activate", G_CALLBACK(mnu_prefs_callback), app);
	g_signal_connect(G_OBJECT(mnu_quit),   "activate", G_CALLBACK(mobotv_quit), &mlapp);
	g_signal_connect(GTK_OBJECT(main_win), "destroy",  G_CALLBACK(mobotv_quit), &mlapp);
	g_signal_connect_swapped(G_OBJECT(mnu_about), "activate", G_CALLBACK(mnu_about_callback), NULL);

	// prepare and show main window
 	g_set_application_name("MoboTV");
	gtk_widget_show_all(main_win);

	// update interface
	if(uconf_get_ch_show(prog_conf) == TRUE) {
		gtk_widget_show(frame_ch);
	} else {
		gtk_widget_hide(frame_ch);
	}
	if(uconf_get_vbar_show(prog_conf) == TRUE) {
		gtk_widget_show(vbar_wg);
	} else {
		gtk_widget_hide(vbar_wg);
	}
	if(uconf_get_info_show(prog_conf) == TRUE) {
		gtk_widget_show(frame_inf);
	} else {
		gtk_widget_hide(frame_inf);
	}
	if(uconf_get_kap_ratio(prog_conf) == TRUE) {
		vp_set_kap_ratio(vplayer, FALSE);
	} else {
		vp_set_kap_ratio(vplayer, TRUE);
	}
	chview_set_view_mode(chview, uconf_get_view_mode(prog_conf));


	// main loop
	g_main_loop_run(mloop);
	return(EXIT_SUCCESS);
}


/**
 * mobotv_quit
 * Exit from MoboTV
 */
static void mobotv_quit(GtkWidget *widget, gpointer data)
{
	main_lapp *lapp  = (main_lapp*)data;
	application *app;
	user_conf *prog_conf;
	ch_list *chlist;
	video_player *vplayer;

	if(lapp == NULL) {
		g_warning("mobotv_quit BUG: ooops, this is not a nice place to stay :(");
		exit(0);
		return;
	} else {
		app       = lapp->app;
		prog_conf = app_get_prog_conf(app);
		chlist    = app_get_chlist(app);
		vplayer   = app_get_vplayer(app);
	}

	// write config and channels list
	uconf_set_kap_ratio(prog_conf, vp_get_kap_ratio(vplayer));
	uconf_write_conf(prog_conf);
	chlist_write_to_file(chlist, uconf_get_ch_list_path(prog_conf));

	// Good Bye!
	vctrl_destroy( app_get_vbar(app) );
	chlist_destroy( app_get_chlist(app) );
	chview_destroy( app_get_chview(app) );
	chinfo_destroy( app_get_chinfo(app) );
	vp_destroy(vplayer);
	uconf_destroy(prog_conf);
	app_destroy(app);
	g_main_loop_quit(lapp->mloop);
}


/**
 * mnu_open_callback
 */
static void mnu_open_callback(GtkWidget *widget, gpointer data)
{
	application *app = (application*)data;
	video_player *vplayer;
	channel *ch;
	char *txturl;

	if(app == NULL) {
		return;
	} else {
		vplayer = app_get_vplayer(app);
	}

	GtkWidget *udialog = input_dialog_new("Load URL", GTK_WINDOW( app_get_main_win(app) ), \
								MOBOTV_INPDIAG_URL);

	if(gtk_dialog_run(GTK_DIALOG(udialog)) == GTK_RESPONSE_OK) {

		// Open channel
		txturl = input_dialog_get_text(udialog);
		ch     = channel_new();
		if(ch == NULL) {
			g_warning("mnu_open_callback: Cannot create new channel");
		} else {
			channel_set_name(ch, "Web stream");
			channel_set_url(ch, txturl);

			// Play
			vp_set_channel(vplayer, ch);
			vp_play(vplayer);
		}
	}
	gtk_widget_destroy(udialog);
}


/**
 * mnu_mgt_ch_callback
 */
static void mnu_mgt_ch_callback(GtkWidget *widget, gpointer data)
{
	application *app = (application*)data;
	GtkWidget *chwin = new_chmg_win(app);
	gtk_window_set_modal(GTK_WINDOW(chwin), TRUE);
	gtk_widget_show_all((GtkWidget *)chwin);
}


/**
 * mnu_prefs_callback
 */
static void mnu_prefs_callback(GtkWidget *widget, gpointer data)
{
	application *app  = (application*)data;
	GtkWidget   *cwin = new_conf_win(app);
	gtk_window_set_modal(GTK_WINDOW(cwin), TRUE);
	gtk_widget_show_all((GtkWidget *)cwin);
}


/**
 * mnu_about_callback
 */
static void mnu_about_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget *awin = new_about_win();
	gtk_window_set_modal(GTK_WINDOW(awin), TRUE);
	gtk_widget_show_all((GtkWidget *)awin);
}


/**
 * smnu_url_callback
 */
static void smnu_url_callback(GtkWidget *widget, gpointer data)
{
	application *app = (application*)data;
	GtkWidget *main_win;
	ch_list *chlist;
	ch_view *chview;
	char *txturl;

	if(app == NULL) {
		return;
	} else {
		main_win = app_get_main_win(app);
		chlist   = app_get_chlist(app);
		chview   = app_get_chview(app);
	}

	GtkWidget *udialog = input_dialog_new("Load URL", GTK_WINDOW( app_get_main_win(app) ), \
								MOBOTV_INPDIAG_URL);

	if(gtk_dialog_run(GTK_DIALOG(udialog)) == GTK_RESPONSE_OK) {

		// Open list
		txturl = alloc_new_str( input_dialog_get_text(udialog) );
		gtk_widget_destroy(udialog);

		load_ch_list(txturl, chlist, chview, main_win);

	 g_free(txturl);
	} else {
		gtk_widget_destroy(udialog);
	}
}


/**
 * smnu_local_callback
 */
static void smnu_local_callback(GtkWidget *widget, gpointer data)
{
	application *app = (application*)data;
	GtkWidget *main_win;
	ch_list *chlist;
	ch_view *chview;
	char *filename;

	if(app == NULL) {
		return;
	} else {
		main_win = app_get_main_win(app);
		chlist   = app_get_chlist(app);
		chview   = app_get_chview(app);
	}

	GtkWidget *fdialog = gtk_file_chooser_dialog_new("Open channels list", \
								GTK_WINDOW(main_win), GTK_FILE_CHOOSER_ACTION_OPEN, \
								GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, \
								GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_mime_type(filter, "text/xml");
	gtk_file_filter_set_name(filter, "XML text file (XML)");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fdialog), filter);


	if(gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));

		gtk_widget_destroy(fdialog);

		load_ch_list(filename, chlist, chview, main_win);

		g_free(filename);
	} else {
		gtk_widget_destroy(fdialog);
	}
}


/**
 * load_ch_list
 * Load channels list and show message on error
 */
static void load_ch_list(char *filename, ch_list *chlist, ch_view *chview, GtkWidget *main_win)
{
	t_chlist tch;
	GtkWidget *dialog;

	// Show info dialog
	dialog = gtk_message_dialog_new(GTK_WINDOW(main_win),             \
					GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, \
					GTK_BUTTONS_NONE, "Loading, please wait...");

	// Create a thread to not busy main window
	tch.dialog   = dialog;
	tch.chlist   = chlist;
	tch.filename = filename;
	tch.res      = FALSE;
	tch.done     = FALSE;

	g_thread_create(load_ch_list_thread, &tch, FALSE, NULL);

	// Wait for thread finish
	gtk_widget_show_all(dialog);
	while(tch.done == FALSE) {
		gtk_main_iteration();
		g_thread_yield();
	}
	gtk_widget_destroy(dialog);

	// Load list
	if(tch.res == FALSE) {
		// Error
		dialog = gtk_message_dialog_new(GTK_WINDOW(main_win),          \
					GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, \
					GTK_BUTTONS_OK, "Failed to load list!");

		// Show error
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
	chview_update(chview);
}
static gpointer load_ch_list_thread(gpointer data)
{
	t_chlist *tch = (t_chlist*)data;
	ch_list *chlist;
	GtkWidget *dialog;
	char *filename;

	if(tch == NULL) {
		return(NULL);
	} else {
		dialog   = tch->dialog;
		chlist   =  tch->chlist;
		filename = tch->filename;
	}

	tch->res = chlist_read_from_file(chlist, filename);

	// We are done
	tch->done = TRUE;
	return(NULL);
}


