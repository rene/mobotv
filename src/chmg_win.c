/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     chmg_win.c
 * Description:  Channel manager window
 * Created:      Sex Jan  9 02:21:17 BRST 2009
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

#include "chmg_win.h"


typedef struct _chmg_widgets {
	application  *app;
	GtkWidget *chwin;
	ch_list   *chlist;
	channel   *ch_active;
	GtkWidget *cbox_ch;
	GtkWidget *txt_name;
	GtkWidget *txt_url;
	GtkTextBuffer *info_buff;
	GtkWidget *txt_info;
	GtkWidget *icon;
	GtkWidget *bt_icon;
	GtkWidget *bt_del;
	GtkWidget *bt_save;
	char *icon_path;
	gboolean ch_updated;
	gint last_ch;
} chmg_widgets;


static gboolean save_modifications(chmg_widgets *data);
static void activate_bt_save_callback1(GtkWidget *widget, gint start_pos, gint end_post, gpointer data);
static void activate_bt_save_callback2(GtkWidget *widget, gchar *new_text, gint new_text_length, gint *position, gpointer data);
static void activate_bt_save_callback3(GtkWidget *widget, gpointer data);
static void bt_close_callback(GtkWidget *widget, gpointer data);
static void cbox_ch_callback(GtkWidget *widget, gpointer data);
static void bt_icon_callback(GtkWidget *widget, gpointer data);
static void bt_del_callback(GtkWidget *widget, gpointer data);
static void bt_add_callback(GtkWidget *widget, gpointer data);
static void bt_save_callback(GtkWidget *widget, gpointer data);


/**
 * new_conf_win
 * Create new user preferences window
 */
GtkWidget *new_chmg_win(application *app)
{
	chmg_widgets *chmg_wg;
	guint lsize, p;
	ch_list *chlist;
	channel *ch;

	GtkWidget *chwin    = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *lbl_ch   = gtk_label_new("Channel:");
	GtkWidget *cbox_ch  = gtk_combo_box_new_text();
	GtkWidget *bt_del   = gtk_button_new_with_label(" Remove ");
	GtkWidget *bt_add   = gtk_button_new_with_label(" Add ");
	GtkWidget *lbl_name = gtk_label_new("Name:");
	GtkWidget *txt_name = gtk_entry_new();
	GtkWidget *lbl_url  = gtk_label_new("URL:");
	GtkWidget *txt_url  = gtk_entry_new();
	GtkWidget *lbl_info = gtk_label_new("Information:");
	GtkWidget *scr_info = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *txt_info = gtk_text_view_new();
	GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txt_info));
	GtkWidget *lbl_icon = gtk_label_new("Picture:");
	GtkWidget *frame_ic = gtk_frame_new(NULL);
	GtkWidget *img_icon = gtk_image_new();
	GtkWidget *bt_icon  = gtk_button_new_with_mnemonic(" _Browse... ");
	GtkWidget *vbox1    = gtk_vbox_new(FALSE, 2);
	GtkWidget *hbox1    = gtk_hbox_new(FALSE, 2);
	GtkWidget *hbox2    = gtk_hbox_new(FALSE, 2);
	GtkWidget *hbox3    = gtk_hbox_new(FALSE, 2);
	GtkWidget *hbox4    = gtk_hbox_new(FALSE, 2);
	GtkWidget *hbox5    = gtk_hbox_new(FALSE, 2);
	GtkWidget *hbox6    = gtk_hbutton_box_new();
	GtkWidget *bt_save  = gtk_button_new_with_mnemonic("_Save");
	GtkWidget *bt_close = gtk_button_new_with_mnemonic("_Close");


	if(app == NULL) {
		g_warning("new_chmg_win: ooops, this is not a nice place to stay :(");
		return(NULL);
	} else {
		chmg_wg = (chmg_widgets*)g_malloc(sizeof(chmg_widgets));
		if(chmg_wg == NULL) {
			g_warning("new_chmg_win: Failed to allocate memory :(");
			return(NULL);
		}

		chlist = app_get_chlist(app);

		chmg_wg->app        = app;
		chmg_wg->chwin      = chwin;
		chmg_wg->chlist     = chlist;
		chmg_wg->ch_active  = NULL;
		chmg_wg->cbox_ch    = cbox_ch;
		chmg_wg->txt_name   = txt_name;
		chmg_wg->txt_url    = txt_url;
		chmg_wg->txt_info   = txt_info;
		chmg_wg->info_buff  = buff;
		chmg_wg->icon       = img_icon;
		chmg_wg->bt_icon    = bt_icon;
		chmg_wg->bt_del     = bt_del;
		chmg_wg->bt_save    = bt_save;
		chmg_wg->icon_path  = NULL;
		chmg_wg->ch_updated = FALSE;
		chmg_wg->last_ch    = -1;
	}

	// Channel
	gtk_box_pack_start(GTK_BOX(hbox1), lbl_ch, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox1), cbox_ch, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox1), bt_del, TRUE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox1), bt_add, TRUE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(hbox2), lbl_name, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox2), txt_name, TRUE, TRUE, 2);

	gtk_box_pack_start(GTK_BOX(hbox3), lbl_url, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(hbox3), txt_url, TRUE, TRUE, 2);

	// Info
	gtk_box_pack_start(GTK_BOX(hbox4), lbl_info, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox4), scr_info, TRUE, TRUE, 2);
	gtk_container_add(GTK_CONTAINER(scr_info), txt_info);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scr_info), GTK_SHADOW_OUT);
	gtk_text_buffer_set_text(buff, "", -1);

	// Icon
	gtk_box_pack_start(GTK_BOX(hbox5), lbl_icon, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox5), frame_ic, FALSE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(hbox5), bt_icon, FALSE, FALSE, 5);
	gtk_container_add(GTK_CONTAINER(frame_ic), img_icon);


	// all
	gtk_box_pack_start(GTK_BOX(vbox1), hbox1, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox2, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox3, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox4, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox5, FALSE, TRUE, 2);


	// Buttons
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox6), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbox6), 2);
	gtk_container_add(GTK_CONTAINER(hbox6), bt_save);
	gtk_container_add(GTK_CONTAINER(hbox6), bt_close);
	gtk_button_box_set_child_size(GTK_BUTTON_BOX(hbox6), -1, 32);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox6, FALSE, FALSE, 2);
	gtk_widget_set_sensitive(bt_save, FALSE);

	// events
	g_signal_connect(G_OBJECT(cbox_ch), "changed", G_CALLBACK(cbox_ch_callback), chmg_wg);
	g_signal_connect(G_OBJECT(bt_icon), "clicked", G_CALLBACK(bt_icon_callback), chmg_wg);
	g_signal_connect(G_OBJECT(bt_del),  "clicked", G_CALLBACK(bt_del_callback), chmg_wg);
	g_signal_connect(G_OBJECT(bt_add),  "clicked", G_CALLBACK(bt_add_callback), chmg_wg);
	g_signal_connect(G_OBJECT(bt_save), "clicked", G_CALLBACK(bt_save_callback), chmg_wg);
	g_signal_connect(G_OBJECT(bt_close),"clicked", G_CALLBACK(bt_close_callback), chmg_wg);

	g_signal_connect(G_OBJECT(txt_name), "delete-text", G_CALLBACK(activate_bt_save_callback1), chmg_wg);
	g_signal_connect(G_OBJECT(txt_name), "insert-text", G_CALLBACK(activate_bt_save_callback2), chmg_wg);
	g_signal_connect(G_OBJECT(txt_url),  "delete-text", G_CALLBACK(activate_bt_save_callback1), chmg_wg);
	g_signal_connect(G_OBJECT(txt_url),  "insert-text", G_CALLBACK(activate_bt_save_callback2), chmg_wg);
	g_signal_connect(G_OBJECT(buff),     "changed",     G_CALLBACK(activate_bt_save_callback3), chmg_wg);

	// Add to combo box
	lsize = chlist_get_length(chlist);
	for(p = 0; p<lsize; p++) {
		if( (ch = chlist_get_channel_at(chlist, p)) != NULL )
			gtk_combo_box_append_text(GTK_COMBO_BOX(cbox_ch), channel_get_name(ch) );
	}

	// loading done
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbox_ch), 0);

	// window
	gtk_container_add(GTK_CONTAINER(chwin), vbox1);
	gtk_window_set_title(GTK_WINDOW(chwin), "Channel Manager");
	gtk_window_set_type_hint(GTK_WINDOW(chwin), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_resize(GTK_WINDOW(chwin), 400, 280);
	gtk_widget_set_size_request(frame_ic, 50, 50);

	return((GtkWidget *)chwin);
}


/**
 * save_modifications
 * Check if channel information was changed and handle with modifications
 */
static gboolean save_modifications(chmg_widgets *data)
{
	chmg_widgets *chmg_wg = data;
	ch_list *chlist;
	ch_view *chview;
	channel *ch;
	application *app;
	GtkWidget *cbox_ch;
	GtkWidget *qdialog;
	GtkWidget *icon;
	GtkTextIter start_it, end_it;
	const char *chname, *txt;

	if(chmg_wg == NULL) {
		g_warning("save_modifications: ooops, this is not a nice place to stay :(");
		return(FALSE);
	} else {
		app     = chmg_wg->app;
		cbox_ch = chmg_wg->cbox_ch;
		chlist  = chmg_wg->chlist;
		icon    = chmg_wg->icon;
		ch      = chmg_wg->ch_active;
	}

	// Check for modifications unsaved
	if(chmg_wg->ch_updated == TRUE) {
		chmg_wg->ch_updated = FALSE;

		qdialog = gtk_message_dialog_new (GTK_WINDOW(chmg_wg->chwin), \
        				GTK_DIALOG_DESTROY_WITH_PARENT,                \
                        GTK_MESSAGE_QUESTION,                          \
                        GTK_BUTTONS_YES_NO, "Save modifications?");

		gtk_window_set_title(GTK_WINDOW(qdialog), "Save");
		if(gtk_dialog_run(GTK_DIALOG(qdialog)) == GTK_RESPONSE_YES) {

			// Update channel
			chname = (char*)gtk_entry_get_text(GTK_ENTRY(chmg_wg->txt_name));
			channel_set_name(ch, chname);

			txt = gtk_entry_get_text(GTK_ENTRY(chmg_wg->txt_url));
			channel_set_url(ch, txt);

			gtk_text_buffer_get_start_iter(chmg_wg->info_buff, &start_it);
			gtk_text_buffer_get_end_iter(chmg_wg->info_buff, &end_it);
			txt = gtk_text_buffer_get_text(chmg_wg->info_buff, &start_it, &end_it, FALSE);
			channel_set_info(ch, txt);

			channel_set_icon_path(ch, chmg_wg->icon_path);

			gtk_combo_box_insert_text(GTK_COMBO_BOX(chmg_wg->cbox_ch), chmg_wg->last_ch, chname);
			gtk_combo_box_remove_text(GTK_COMBO_BOX(chmg_wg->cbox_ch), (chmg_wg->last_ch + 1));

			gtk_widget_destroy(qdialog);

			// Update icon box
			chview = app_get_chview(app);
			chview_update(chview);
			return(TRUE);
		} else {
			gtk_widget_destroy(qdialog);
			return(FALSE);
		}
	} else {
		return(FALSE);
	}
}


/**
 * Callbacks
 */
static void activate_bt_save_callback1(GtkWidget *widget, gint start_pos, gint end_post, gpointer data)
{
	chmg_widgets *chmg_wg = (chmg_widgets*)data;

	if(chmg_wg == NULL) {
		g_warning("activate_bt_save_callback1: ooops, this is not a nice place to stay :(");
		return;
	} else {
		gtk_widget_set_sensitive(chmg_wg->bt_save, TRUE);
		chmg_wg->ch_updated = TRUE;
	}
}
static void activate_bt_save_callback2(GtkWidget *widget, gchar *new_text, gint new_text_length, gint *position, gpointer data)
{
	activate_bt_save_callback1(widget, 0, 0, data);
}
static void activate_bt_save_callback3(GtkWidget *widget, gpointer data)
{
	activate_bt_save_callback1(widget, 0, 0, data);
}


static void bt_close_callback(GtkWidget *widget, gpointer data)
{
	chmg_widgets *chmg_wg = (chmg_widgets*)data;

	if(chmg_wg == NULL) {
		g_warning("bt_close_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		GtkWidget *chwin = chmg_wg->chwin;
		gtk_widget_destroy(GTK_WIDGET(chwin));

		if(chmg_wg->icon_path != NULL) g_free(chmg_wg->icon_path);
	 g_free(chmg_wg);
	}
}


static void cbox_ch_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget    *cbox_ch = widget;
	chmg_widgets *chmg_wg = (chmg_widgets*)data;
	ch_list *chlist;
	channel *ch;
	GtkWidget *icon;
	GtkWidget *chicon;
	GdkPixbuf *img_pbuff;
	gint pos;
	const char *txt;

	if(chmg_wg == NULL) {
		g_warning("cbox_ch_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		chlist = chmg_wg->chlist;
		icon   = chmg_wg->icon;
		ch     = chmg_wg->ch_active;
	}

	// Check for modifications unsaved
	save_modifications(chmg_wg);

	// Load channel information
	pos = gtk_combo_box_get_active( GTK_COMBO_BOX(cbox_ch) );
	ch  = chlist_get_channel_at(chlist, pos);

	chmg_wg->ch_active = ch;

	if(ch != NULL) {
		gtk_entry_set_text(GTK_ENTRY(chmg_wg->txt_name), channel_get_name(ch));

		txt = channel_get_url(ch);
		if(txt != NULL) {
			gtk_entry_set_text(GTK_ENTRY(chmg_wg->txt_url), txt);
		} else {
			gtk_entry_set_text(GTK_ENTRY(chmg_wg->txt_url), "");
		}

		txt = channel_get_info(ch);
		if(txt != NULL) {
			gtk_text_buffer_set_text(chmg_wg->info_buff, txt, -1);
		} else {
			gtk_text_buffer_set_text(chmg_wg->info_buff, "", 0);
		}

		chicon = channel_get_icon_img(ch);
		if(chicon != NULL) {
			img_pbuff = gtk_image_get_pixbuf( GTK_IMAGE(chicon) );
			gtk_image_set_from_pixbuf(GTK_IMAGE(icon), img_pbuff);
		} else {
			gtk_image_clear(GTK_IMAGE(icon));
		}
		chmg_wg->icon_path = alloc_new_str( channel_get_icon_path(ch) );

		// Enable fields and buttons
		gtk_widget_set_sensitive(chmg_wg->txt_name, TRUE);
		gtk_widget_set_sensitive(chmg_wg->txt_url, TRUE);
		gtk_widget_set_sensitive(chmg_wg->txt_info, TRUE);
		gtk_widget_set_sensitive(chmg_wg->bt_del, TRUE);
		gtk_widget_set_sensitive(chmg_wg->bt_icon, TRUE);
	} else {
		gtk_entry_set_text(GTK_ENTRY(chmg_wg->txt_name), "");
		gtk_entry_set_text(GTK_ENTRY(chmg_wg->txt_url), "");
		gtk_text_buffer_set_text(chmg_wg->info_buff, "", 0);
		gtk_image_clear(GTK_IMAGE(icon));

		// Disable fields and buttons
		gtk_widget_set_sensitive(chmg_wg->txt_name, FALSE);
		gtk_widget_set_sensitive(chmg_wg->txt_url, FALSE);
		gtk_widget_set_sensitive(chmg_wg->txt_info, FALSE);
		gtk_widget_set_sensitive(chmg_wg->bt_del, FALSE);
		gtk_widget_set_sensitive(chmg_wg->bt_icon, FALSE);
	}

	gtk_widget_set_sensitive(chmg_wg->bt_save, FALSE);
	chmg_wg->ch_updated = FALSE;
	chmg_wg->last_ch    = pos;
}


static void bt_icon_callback(GtkWidget *widget, gpointer data)
{
	chmg_widgets *chmg_wg = (chmg_widgets*)data;
	char *filename;

	if(chmg_wg == NULL) {
		g_warning("bt_icon_callback: ooops, this is not a nice place to stay :(");
		return;
	}

	GtkWidget *fdialog = gtk_file_chooser_dialog_new("Open channel icon", \
								GTK_WINDOW(chmg_wg->chwin), GTK_FILE_CHOOSER_ACTION_OPEN, \
								GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, \
								GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_mime_type(filter, "image/jpeg");
	gtk_file_filter_add_mime_type(filter, "image/png");
	gtk_file_filter_set_name(filter, "Images (JPEG,PNG)");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fdialog), filter);


	if(gtk_dialog_run(GTK_DIALOG(fdialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fdialog));

		// Open icon
		gtk_image_set_from_file(GTK_IMAGE(chmg_wg->icon), filename);
		chmg_wg->icon_path = alloc_new_str(filename);

		g_free(filename);
	}

	gtk_widget_destroy(fdialog);
	gtk_widget_set_sensitive(chmg_wg->bt_save, TRUE);
	chmg_wg->ch_updated = TRUE;
}


/**
 * bt_del_callback
 * Remove a Channel from the list
 */
static void bt_del_callback(GtkWidget *widget, gpointer data)
{
	chmg_widgets *chmg_wg = (chmg_widgets*)data;
	ch_view *chview;
	ch_list *chlist;
	channel *ch;
	gint chpos;

	if(chmg_wg == NULL) {
		g_warning("bt_del_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		chview = app_get_chview(chmg_wg->app);
		chlist = chmg_wg->chlist;
		ch     = chmg_wg->ch_active;
	}

	if(ch != NULL) {
		// Confirm exclusion
		GtkWidget *res_dialog;
		GtkWidget *qdialog = gtk_message_dialog_new (GTK_WINDOW(chmg_wg->chwin), \
                                  GTK_DIALOG_DESTROY_WITH_PARENT,                \
                                  GTK_MESSAGE_QUESTION,                          \
                                  GTK_BUTTONS_YES_NO,                            \
                                  "Are you sure you want to remove %s channel?", \
								  channel_get_name(ch));

		gtk_window_set_title(GTK_WINDOW(qdialog), "Confirm");
		if(gtk_dialog_run(GTK_DIALOG(qdialog)) == GTK_RESPONSE_YES) {
			if(chlist_del_channel_byref(chlist, ch) == TRUE) {

				// Update combox box
				chmg_wg->ch_updated = FALSE;
				chpos = gtk_combo_box_get_active( GTK_COMBO_BOX(chmg_wg->cbox_ch) );
				gtk_combo_box_remove_text(GTK_COMBO_BOX(chmg_wg->cbox_ch), chpos);
				gtk_combo_box_set_active(GTK_COMBO_BOX(chmg_wg->cbox_ch), 0);

				// Update icon box
				chview_update(chview);

			} else {

				// Error
				res_dialog = gtk_message_dialog_new(GTK_WINDOW(chmg_wg->chwin),   \
								GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, \
								GTK_BUTTONS_OK, "Error removing channel!");

				// Show error
				gtk_dialog_run(GTK_DIALOG(res_dialog));
				gtk_widget_destroy(res_dialog);
			}
			gtk_widget_destroy(qdialog);

		} else {
			gtk_widget_destroy(qdialog);
		}
	}
}


/**
 * bt_add_callback
 * Add new Channel to the list
 */
static void bt_add_callback(GtkWidget *widget, gpointer data)
{
	chmg_widgets *chmg_wg = (chmg_widgets*)data;
	ch_list *chlist;
	channel *ch;
	guint lsize;
	GtkWidget *idialog;
	GtkWidget *udialog;
	char *chname;
	gboolean chname_ok;

	if(chmg_wg == NULL) {
		g_warning("bt_add_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		chlist = chmg_wg->chlist;
		ch     = chmg_wg->ch_active;
	}

	// Check for modifications
	if(save_modifications(chmg_wg) == TRUE)
		gtk_combo_box_set_active(GTK_COMBO_BOX(chmg_wg->cbox_ch), 0);


	// Read and validate channel name
	chname_ok = FALSE;
	while(chname_ok == FALSE) {
		udialog = input_dialog_new("Channel name", GTK_WINDOW(chmg_wg->chwin), \
								MOBOTV_INPDIAG_TEXT);

		if(gtk_dialog_run(GTK_DIALOG(udialog)) == GTK_RESPONSE_CANCEL) {
			gtk_widget_destroy(udialog);
			return;
		}

		chname = alloc_new_str( input_dialog_get_text(udialog) );
		gtk_widget_destroy(udialog);

		// Check channel name
		ch = chlist_get_channel_by_name(chlist, chname);
		if(ch != NULL) {
			idialog = gtk_message_dialog_new(GTK_WINDOW(chmg_wg->chwin),       \
								GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, \
								GTK_BUTTONS_OK, "Channel %s already exist. Please, choose another name.", chname);

			gtk_window_set_title(GTK_WINDOW(idialog), "New channel name");
			gtk_dialog_run(GTK_DIALOG(idialog));
			gtk_widget_destroy(idialog);
		 g_free(chname);
		} else {
			gboolean blank = FALSE;
			size_t p, strl;

			strl = strlen(chname);

			if(strl == 0) {
				continue;
			} else {
				// Check for blank name
				p = 0;
				while(chname[p] == ' ') p++;
				if(p >= strl)
					blank = TRUE;

				if(blank == TRUE) {
					idialog = gtk_message_dialog_new(GTK_WINDOW(chmg_wg->chwin),       \
								GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, \
								GTK_BUTTONS_OK, "This is not a valid channel name.");

					gtk_window_set_title(GTK_WINDOW(idialog), "New channel name");
					gtk_dialog_run(GTK_DIALOG(idialog));
					gtk_widget_destroy(idialog);

					chname_ok = FALSE;
				} else {
					chname_ok = TRUE;
				}
			}
		}
	}

	// Add new Channel
	ch = channel_new();
	channel_set_name(ch, chname);
	chlist_add_channel(chlist, ch);

	// Update combo box
	gtk_combo_box_append_text(GTK_COMBO_BOX(chmg_wg->cbox_ch), chname);
	lsize = chlist_get_length(chlist);
	gtk_combo_box_set_active(GTK_COMBO_BOX(chmg_wg->cbox_ch), (lsize - 1));
	gtk_widget_grab_focus(chmg_wg->txt_url);

	gtk_widget_set_sensitive(chmg_wg->bt_save, TRUE);
	chmg_wg->ch_updated = TRUE;

 g_free(chname);
}


/**
 * bt_save_callback
 * Update current channel
 */
static void bt_save_callback(GtkWidget *widget, gpointer data)
{
	chmg_widgets *chmg_wg = (chmg_widgets*)data;
	application *app;
	ch_view *chview;
	ch_list *chlist;
	channel *ch;
	GtkTextIter start_it, end_it;
	guint chpos;
	const char *chname, *txt;

	if(chmg_wg == NULL) {
		g_warning("bt_save_callback: ooops, this is not a nice place to stay :(");
		return;
	} else {
		app    = chmg_wg->app;
		chlist = chmg_wg->chlist;
		ch     = chmg_wg->ch_active;
	}

	chmg_wg->ch_updated = FALSE;

	if(ch != NULL) {
		// Update channel
		chname = gtk_entry_get_text(GTK_ENTRY(chmg_wg->txt_name));
		chpos  = gtk_combo_box_get_active( GTK_COMBO_BOX(chmg_wg->cbox_ch) );
		channel_set_name(ch, chname);

		txt = gtk_entry_get_text(GTK_ENTRY(chmg_wg->txt_url));
		channel_set_url(ch, txt);

		gtk_text_buffer_get_start_iter(chmg_wg->info_buff, &start_it);
		gtk_text_buffer_get_end_iter(chmg_wg->info_buff, &end_it);
		txt = gtk_text_buffer_get_text(chmg_wg->info_buff, &start_it, &end_it, FALSE);
		channel_set_info(ch, txt);

		channel_set_icon_path(ch, chmg_wg->icon_path);

		// Update combo box
		gtk_combo_box_insert_text(GTK_COMBO_BOX(chmg_wg->cbox_ch), chpos, chname);
		gtk_combo_box_remove_text(GTK_COMBO_BOX(chmg_wg->cbox_ch), (chpos + 1));
		gtk_combo_box_set_active(GTK_COMBO_BOX(chmg_wg->cbox_ch), chpos);

		gtk_widget_set_sensitive(widget, FALSE);

		// Update icons
		chview = app_get_chview(app);
		chview_update(chview);
	}
}


