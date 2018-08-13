/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     conf_win.c
 * Description:  User preferences window
 * Created:      Qui Jan  8 20:34:40 BRST 2009
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

#include "conf_win.h"


typedef struct _cwin_widgets {
	HildonWindow *cwin;
	application *app;
	GtkWidget *ck_chl;
	GtkWidget *rb_list;
	GtkWidget *rb_icon;
	GtkWidget *ck_info;
	GtkWidget *ck_vbar;
	GtkWidget *txt_cl;
	user_conf *prog_conf;
} cwin_widgets;


static void bt_ok_callback(GtkWidget *widget, gpointer data);
static void bt_cancel_callback(GtkWidget *widget, gpointer data);
static void ck_chl_callback(GtkWidget *widget, gpointer data);


/**
 * new_conf_win
 * Create new user preferences window
 */
GtkWidget *new_conf_win(application *app)
{
	cwin_widgets *cwidgets;
	user_conf *prog_conf;

	HildonWindow *cwin  = HILDON_WINDOW(hildon_window_new());
	GtkWidget *tabpanel = gtk_notebook_new();
	GtkWidget *ck_chl   = gtk_check_button_new_with_label("Show channel List");
	GtkWidget *lbl_view = gtk_label_new("View mode:");
	GtkWidget *rb_icon  = gtk_radio_button_new_with_label(NULL, "Icons");
	GtkWidget *rb_list  = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rb_icon), "List");
	GtkWidget *ck_info  = gtk_check_button_new_with_label("Channel information");
	GtkWidget *ck_vbar  = gtk_check_button_new_with_label("Video controls");
	GtkWidget *frame_vd = gtk_frame_new("Display");
	GtkWidget *frame_ch = gtk_frame_new("Channel List");
	GtkWidget *vbox1    = gtk_vbox_new(FALSE, 2);
	GtkWidget *vbox2    = gtk_vbox_new(FALSE, 2);
	GtkWidget *vbox3    = gtk_vbox_new(TRUE, 2);
	GtkWidget *vbox4    = gtk_vbox_new(FALSE, 2);
	GtkWidget *frame_cl = gtk_frame_new("Load channel list from:");
	GtkWidget *txt_cl   = gtk_entry_new();
	GtkWidget *hbox1    = gtk_hbox_new(TRUE, 2);
	GtkWidget *hbox2    = gtk_hbutton_box_new();
	GtkWidget *bt_ok    = gtk_button_new_with_mnemonic("_OK");
	GtkWidget *bt_canc  = gtk_button_new_with_mnemonic("_Cancel");


	// Tabs
	GtkWidget *tab_lbl1 = gtk_label_new("General");
	GtkWidget *tab_lbl2 = gtk_label_new("Sound and Video");
	gtk_notebook_append_page(GTK_NOTEBOOK(tabpanel), vbox1, tab_lbl1);
	//gtk_notebook_append_page(GTK_NOTEBOOK(tabpanel), vbox4, tab_lbl2);

	// Channel
	gtk_frame_set_shadow_type(GTK_FRAME(frame_ch), GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(frame_ch), vbox2);
	gtk_box_pack_start(GTK_BOX(vbox2), ck_chl, FALSE, TRUE, 2);
	gtk_box_pack_end(GTK_BOX(vbox2), hbox1, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox1), lbl_view, FALSE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(hbox1), rb_list, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox1), rb_icon, FALSE, TRUE, 2);

	// Info & control bar
	gtk_box_pack_start(GTK_BOX(vbox1), frame_ch, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox1), frame_vd, FALSE, TRUE, 2);
	gtk_container_add(GTK_CONTAINER(frame_vd), vbox3);
	gtk_box_pack_start(GTK_BOX(vbox3), ck_info, FALSE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox3), ck_vbar, FALSE, TRUE, 2);


	// Channel list path
	gtk_container_add(GTK_CONTAINER(frame_cl), txt_cl);
	gtk_box_pack_start(GTK_BOX(vbox1), frame_cl, FALSE, TRUE, 2);

	// Buttons
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox2), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbox2), 2);
	gtk_container_add(GTK_CONTAINER(hbox2), bt_canc);
	gtk_container_add(GTK_CONTAINER(hbox2), bt_ok);
	gtk_button_box_set_child_size(GTK_BUTTON_BOX(hbox2), -1, 32);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox2, FALSE, FALSE, 2);

	// events
	cwidgets  = (cwin_widgets*)g_malloc(sizeof(cwin_widgets));
	prog_conf = app_get_prog_conf(app);
	if(cwidgets != NULL) {
		cwidgets->app       = app;
		cwidgets->cwin      = cwin;
		cwidgets->ck_chl    = ck_chl;
		cwidgets->rb_list   = rb_list;
		cwidgets->rb_icon   = rb_icon;
		cwidgets->ck_info   = ck_info;
		cwidgets->ck_vbar   = ck_vbar;
		cwidgets->txt_cl    = txt_cl;
	}
	g_signal_connect(G_OBJECT(bt_ok), "clicked", G_CALLBACK(bt_ok_callback), cwidgets);
	g_signal_connect(G_OBJECT(bt_canc), "clicked", G_CALLBACK(bt_cancel_callback), cwin);
	g_signal_connect(G_OBJECT(ck_chl), "toggled", G_CALLBACK(ck_chl_callback), hbox1);


	// update with user configuration
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ck_chl), uconf_get_ch_show(prog_conf));

	switch( uconf_get_view_mode(prog_conf) ) {
		case MOBOTV_VMODE_LIST:
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_list), TRUE);
				break;

		case MOBOTV_VMODE_ICON:
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_icon), TRUE);
				break;
	}

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ck_info), uconf_get_info_show(prog_conf));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ck_vbar), uconf_get_vbar_show(prog_conf));
	gtk_entry_set_text(GTK_ENTRY(txt_cl), uconf_get_ch_list_path(prog_conf));


	// window
	gtk_container_add(GTK_CONTAINER(cwin), tabpanel);
	gtk_window_set_title(GTK_WINDOW(cwin), "Preferences");
	gtk_window_set_type_hint(GTK_WINDOW(cwin), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_resize(GTK_WINDOW(cwin), 400, 380);
	gtk_widget_set_size_request(frame_cl, -1, 80);
	gtk_widget_set_size_request(frame_vd, -1, 100);

	return((GtkWidget *)cwin);
}


/**
 * Callbacks
 */
static void bt_ok_callback(GtkWidget *widget, gpointer data)
{
	cwin_widgets *cwidgets = (cwin_widgets*)data;
	application  *app;
	GtkWidget    *cfrm;
	GtkWidget    *vbar;
	GtkWidget    *chinf;
	ch_view      *chview;
	user_conf    *prog_conf;

	if(cwidgets == NULL) {
		g_warning("bt_ok_callback (no memory): ooops, this is not a nice place to stay :(");
		return;
	} else {
		app = cwidgets->app;

		if(app != NULL) {
			cfrm      = app_get_frm_ch(app);
			vbar      = vctrl_get_widget( app_get_vbar(app) );
			chinf     = app_get_frm_inf(app);
			chview    = app_get_chview(app);
			prog_conf = app_get_prog_conf(app);
		} else {
			return;
		}
	}

	// update configuration
	uconf_set_ch_show(prog_conf, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cwidgets->ck_chl)));

	if( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cwidgets->rb_list)) == TRUE ) {
		uconf_set_view_mode(prog_conf, MOBOTV_VMODE_LIST);
	} else {
		uconf_set_view_mode(prog_conf, MOBOTV_VMODE_ICON);
	}

 	uconf_set_info_show(prog_conf, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cwidgets->ck_info)));
	uconf_set_vbar_show(prog_conf, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cwidgets->ck_vbar)));
	uconf_set_ch_list_path(prog_conf, (char*)gtk_entry_get_text(GTK_ENTRY(cwidgets->txt_cl)));

	// update interface
	if(uconf_get_ch_show(prog_conf) == TRUE) {
		gtk_widget_show(cfrm);
	} else {
		gtk_widget_hide(cfrm);
	}
	if(uconf_get_vbar_show(prog_conf) == TRUE) {
		gtk_widget_show(vbar);
	} else {
		gtk_widget_hide(vbar);
	}
	if(uconf_get_info_show(prog_conf) == TRUE) {
		gtk_widget_show(chinf);
	} else {
		gtk_widget_hide(chinf);
	}
	chview_set_view_mode(chview, uconf_get_view_mode(prog_conf));

	// close window
	gtk_widget_destroy(GTK_WIDGET(cwidgets->cwin));
 g_free(cwidgets);
}

static void bt_cancel_callback(GtkWidget *widget, gpointer data)
{
	HildonWindow *cwin = HILDON_WINDOW(data);
	gtk_widget_destroy(GTK_WIDGET(cwin));
}

static void ck_chl_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget *hbox1 = (GtkWidget*)data;

	if(hbox1 != NULL) {
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) == TRUE) {
			gtk_widget_set_sensitive(hbox1, TRUE);
		} else {
			gtk_widget_set_sensitive(hbox1, FALSE);
		}
	}
}


