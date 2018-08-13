/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     about_win.c
 * Description:  About window
 * Created:      Sex Jan  9 00:52:47 BRST 2009
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

#include "about_win.h"


void bt_close_callback(GtkWidget *widget, gpointer data);


GtkWidget *new_about_win(void)
{
	GtkWidget *awin        = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *icon        = gtk_image_new_from_file(MOBOTV_ICON);
	GtkWidget *lbl_title   = gtk_label_new(NULL);
	GtkWidget *lbl_url     = gtk_link_button_new_with_label("http://code.google.com/p/mobotv", "http://code.google.com/p/mobotv");
	GtkWidget *lbl_author  = gtk_label_new("Developed by Renê de Souza Pinto");
	GtkWidget *lbl_email   = gtk_link_button_new_with_label("mailto:" PACKAGE_BUGREPORT, PACKAGE_BUGREPORT);
	GtkWidget *lbl_license = gtk_label_new("This program is free software, licensed under the terms of");
	GtkWidget *lbl_gpl     = gtk_link_button_new_with_label("http://www.gnu.org/licenses/old-licenses/gpl-2.0.html", "GPV v.2");
	GtkWidget *vbox1       = gtk_vbox_new(FALSE, 2);
	GtkWidget *vbox2	   = gtk_hbox_new(FALSE, 1);
	GtkWidget *hbox1	   = gtk_vbox_new(FALSE, 1);
	GtkWidget *hbox2	   = gtk_hbutton_box_new();
	GtkWidget *bt_close    = gtk_button_new_with_mnemonic("_Close");

	char *markup = g_markup_printf_escaped("<span size=\"large\" weight=\"bold\">MoboTV - Version %s</span>", PACKAGE_VERSION);
	gtk_label_set_markup(GTK_LABEL(lbl_title), markup);
	g_free(markup);

	gtk_box_pack_start(GTK_BOX(vbox1), icon, TRUE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), lbl_title, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(vbox1), lbl_url, FALSE, TRUE, 2);

	gtk_box_pack_start(GTK_BOX(hbox1), lbl_author, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(hbox1), lbl_email, TRUE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox1, FALSE, TRUE, 2);

	gtk_box_pack_start(GTK_BOX(vbox2), lbl_license, FALSE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(vbox2), lbl_gpl, FALSE, TRUE, 1);
	gtk_box_pack_start(GTK_BOX(vbox1), vbox2, FALSE, TRUE, 2);


	// Button
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox2), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbox2), 2);
	gtk_container_add(GTK_CONTAINER(hbox2), bt_close);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox2, FALSE, TRUE, 2);

	// events
	g_signal_connect(G_OBJECT(bt_close), "clicked", G_CALLBACK(bt_close_callback), awin);

	// window
	gtk_container_add(GTK_CONTAINER(awin), vbox1);
	gtk_window_set_title(GTK_WINDOW(awin), "About");
	gtk_window_set_type_hint(GTK_WINDOW(awin), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_resize(GTK_WINDOW(awin), 400, 200);
	gtk_widget_grab_focus(bt_close);

	return((GtkWidget*)awin);
}


/**
 * Callbacks
 */
void bt_close_callback(GtkWidget *widget, gpointer data)
{
	GtkWidget *awin = (GtkWidget*)data;
	gtk_widget_destroy(awin);
}


