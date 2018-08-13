/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ch_info.c
 * Description:  Show channel information
 * Created:      Sáb Jan 10 14:31:12 BRST 2009
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

#include "ch_info.h"


static void chinfo_set_info(ch_info *obj, char *info);
static void create_box(ch_info *obj);


/**
 * chinfo_new
 * Instance new ch_info object
 */
ch_info *chinfo_new()
{
	ch_info *obj;

	obj = (ch_info*)g_malloc(sizeof(ch_info));
	if(obj == NULL) {
		return(NULL);
	} else {
		obj->widget     = NULL;
		obj->txt_widget = NULL;
		obj->ch         = NULL;
		obj->info       = NULL;
		// Create box
		create_box(obj);
		return(obj);
	}
}


/**
 * chinfo_destroy
 * Destroy a instanced object
 */
void chinfo_destroy(ch_info *obj)
{
	if(obj == NULL) return;

	if(obj->info != NULL) g_free(obj->info);
 g_free(obj);
}


/**
 * Sets and Gets
 */
GtkWidget *chinfo_get_widget(ch_info *obj)
{
	if(obj != NULL) {
		return(obj->widget);
	} else {
		return(NULL);
	}
}


void chinfo_set_widget(ch_info *obj, GtkWidget *wd)
{
	if(obj == NULL) return;

	obj->widget = wd;
}


GtkWidget *chinfo_get_txt_widget(ch_info *obj)
{
	if(obj != NULL) {
		return(obj->txt_widget);
	} else {
		return(NULL);
	}
}


void chinfo_set_txt_widget(ch_info *obj, GtkWidget *wd)
{
	if(obj == NULL) return;

	obj->txt_widget = wd;
}


void chinfo_set_channel(ch_info *obj, channel *ch)
{
	if(obj == NULL) return;

	obj->ch = ch;

	// Update information
	if(obj->ch != NULL) {
		GtkTextBuffer *buffer;
		GtkWidget *txt_info = chinfo_get_txt_widget(obj);
		if(txt_info != NULL) {
			buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txt_info));
			if(buffer != NULL) {
				chinfo_set_info(obj, ch->info);
				gtk_text_buffer_set_text(buffer, ch->info, -1);
			}
		}
	}
}


channel *ch_info_get_channel(ch_info *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->ch);
}


static void chinfo_set_info(ch_info *obj, char *info)
{
	if(obj == NULL) return;

	if(obj->info != NULL) g_free(obj->info);
	obj->info = alloc_new_str(info);
}


char *chinfo_get_info(ch_info *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->info);
}


/**
 * create_box
 * Create channel information box
 */
static void create_box(ch_info *obj)
{
	if(obj == NULL) return;

	GtkWidget *swin       = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *txt_info   = gtk_text_view_new();
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txt_info));

	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(swin), GTK_SHADOW_OUT);
	gtk_container_add(GTK_CONTAINER(swin), txt_info);

	gtk_text_view_set_editable(GTK_TEXT_VIEW(txt_info), FALSE);

	gtk_text_buffer_set_text(buffer, "", -1);

	chinfo_set_widget(obj, swin);
	chinfo_set_txt_widget(obj, txt_info);
}


