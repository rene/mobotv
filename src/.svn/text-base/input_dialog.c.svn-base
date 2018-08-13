/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     input_dialog.c
 * Description:  Dialog to input text or URL
 * Created:      Ter Jan 13 00:30:37 BRST 2009
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

#include "input_dialog.h"


/**
 * input_dialog_new
 * Create new input dialog
 */
GtkWidget *input_dialog_new(const gchar *title, GtkWindow *parent, char type)
{
	GtkWidget *udiag     = gtk_dialog_new_with_buttons(title, parent, GTK_DIALOG_DESTROY_WITH_PARENT, \
								GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	GtkWidget *txt_entry = gtk_entry_new();
	GtkWidget *content_area;

	if(udiag == NULL) {
		return(NULL);
	} else {
		content_area = GTK_DIALOG(udiag)->vbox;
	}

	gtk_container_add(GTK_CONTAINER(content_area), txt_entry);
	gtk_widget_set_size_request(udiag, 390, -1);
	gtk_widget_show_all(udiag);
	return(udiag);
}


/**
 * input_dialog_get_text
 * Return the text typed
 */
gchar *input_dialog_get_text(GtkWidget *idiag)
{
	if(idiag == NULL) return(NULL);

	GtkWidget *content_area = GTK_DIALOG(idiag)->vbox;
	GList *childrens        = gtk_container_get_children( GTK_CONTAINER(content_area) );
	GtkWidget *txt_entry    = g_list_nth_data(childrens, 0);

	return( (gchar*)gtk_entry_get_text(GTK_ENTRY(txt_entry)) );
}


