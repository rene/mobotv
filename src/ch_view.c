/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ch_view.c
 * Description:  Channels icon view widget
 * Created:      Dom Jan 11 03:38:56 BRST 2009
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

#include "ch_view.h"


static void chview_set_selected_channel(ch_view *obj, const char *chname);
static void create_box(ch_view *obj);
static void tree_selection_callback(GtkTreeSelection *selection, gpointer data);
static void ibox_selection_callback(GtkWidget *widget, gpointer data);
static void tree_path_free(gpointer data, gpointer user_data);


/**
 * chview_new
 * Instance new object ch_view
 */
ch_view *chview_new(ch_list *chlist, ch_info *chinfo, video_ctrl *vctrl, video_player *vplayer)
{
	ch_view *obj;

	obj = (ch_view*)g_malloc(sizeof(ch_view));
	if(obj == NULL) {
		return(NULL);
	} else {
		obj->chlist     = chlist;
		obj->chinfo     = chinfo;
		obj->vplayer    = vplayer;
		obj->vctrl      = vctrl;
		obj->widget     = NULL;
		obj->ibox       = NULL;
		obj->list_store = NULL;
		obj->chlist     = NULL;
		obj->tree       = NULL;
		obj->chname     = NULL;
		obj->view_mode  = MOBOTV_VMODE_LIST;

		// Create icon box
		create_box(obj);
		return(obj);
	}
}


/**
 * chview_destroy
 * Destroy a instanced object
 */
void chview_destroy(ch_view *obj)
{
	if(obj == NULL) return;

	if(obj->chname != NULL) g_free(obj->chname);
 g_free(obj);
}


/**
 * Sets and Gets
 */
GtkWidget *chview_get_widget(ch_view *obj)
{
	if(obj != NULL) {
		return(obj->widget);
	} else {
		return(NULL);
	}
}


void chview_set_chlist(ch_view *obj, ch_list *chlist)
{
	if(obj == NULL) return;

	obj->chlist = chlist;
}


void chview_set_view_mode(ch_view *obj, char view_mode)
{
	if(obj == NULL) return;

	GtkWidget *swin  = chview_get_widget(obj);
	GList *childrens = gtk_container_get_children(GTK_CONTAINER(swin));
	GtkWidget *child = g_list_nth_data(childrens, 0);

	if(child != NULL)
		gtk_container_remove(GTK_CONTAINER(swin), child);


	if(view_mode == MOBOTV_VMODE_ICON) {

		// create icon box
		GtkWidget *ibox = gtk_icon_view_new_with_model(GTK_TREE_MODEL(obj->list_store));

		gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(ibox), 0);
		gtk_icon_view_set_text_column(GTK_ICON_VIEW(ibox), 1);
		gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(ibox), GTK_SELECTION_SINGLE);

		gtk_icon_view_set_columns(GTK_ICON_VIEW(ibox), -1);

		gtk_container_add(GTK_CONTAINER(swin), ibox);
		obj->view_mode = MOBOTV_VMODE_ICON;
		obj->ibox      = ibox;
		obj->tree      = NULL;

		g_signal_connect(G_OBJECT(ibox), "selection-changed", G_CALLBACK(ibox_selection_callback), obj);

	} else if(view_mode == MOBOTV_VMODE_LIST) {

		// create list box
		GtkWidget *tree          = gtk_tree_view_new_with_model(GTK_TREE_MODEL(obj->list_store));
		GtkTreeViewColumn *tcol1 = gtk_tree_view_column_new();
		GtkTreeViewColumn *tcol2 = gtk_tree_view_column_new();
		GtkTreeSelection  *selection;
		GtkCellRenderer *renderer;

		// create list box
		gtk_tree_view_column_set_title(tcol1, " Icon ");
		gtk_tree_view_column_set_title(tcol2, " Name ");
		gtk_tree_view_append_column(GTK_TREE_VIEW(tree), tcol1);
		gtk_tree_view_append_column(GTK_TREE_VIEW(tree), tcol2);

		renderer = gtk_cell_renderer_pixbuf_new();
		gtk_tree_view_column_pack_start(tcol1, renderer, TRUE);
		gtk_tree_view_column_add_attribute(tcol1, renderer, "pixbuf", 0);

		renderer = gtk_cell_renderer_text_new();
		gtk_tree_view_column_pack_start(tcol2, renderer, TRUE);
		gtk_tree_view_column_add_attribute(tcol2, renderer, "text", 1);
		gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);

		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

		gtk_container_add(GTK_CONTAINER(swin), tree);
		obj->view_mode = MOBOTV_VMODE_LIST;
		obj->tree      = tree;
		obj->ibox      = NULL;

		g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(tree_selection_callback), obj);
	}
	gtk_widget_show_all(swin);
	chview_update(obj);
}


char chview_get_view_mode(ch_view *obj)
{
	if(obj == NULL) return(MOBOTV_VMODE_LIST);

	return(obj->view_mode);
}


char *chview_get_selected_channel(ch_view *obj)
{
	if(obj == NULL) return(NULL);

	return(obj->chname);
}


static void chview_set_selected_channel(ch_view *obj, const char *chname)
{
	if(obj == NULL) return;

	if(obj->chname != NULL) g_free(obj->chname);

	obj->chname = alloc_new_str(chname);
}


/**
 * chview_update
 * Update with channels list
 */
void chview_update(ch_view *obj)
{
	if(obj == NULL) return;
	if(obj->chlist == NULL || obj->list_store == NULL) return;

	GtkTreeIter iter;
	GdkPixbuf *icon;
	channel *ch;
	const char *chname;
	guint lsize, i;

	gtk_list_store_clear(obj->list_store);

	lsize = chlist_get_length(obj->chlist);
	for(i=0; i<lsize; i++) {
		ch = chlist_get_channel_at(obj->chlist, i);

		if(ch != NULL) {
			chname = channel_get_name(ch);
			icon   = gtk_image_get_pixbuf( GTK_IMAGE(channel_get_icon_img(ch)) );

			gtk_list_store_append(obj->list_store, &iter);
			gtk_list_store_set(obj->list_store, &iter, 0, icon, 1, chname, -1);
		}
	}

	gtk_widget_show_all( chview_get_widget(obj) );
}


/**
 * create_box
 * Create icon box
 */
static void create_box(ch_view *obj)
{
	if(obj == NULL) return;

	GtkWidget *swin          = gtk_scrolled_window_new(NULL, NULL);
	GtkListStore *list_store = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);

	// update object
	obj->list_store = list_store;
	obj->widget     = swin;
}


/**
 * Callbacks
 */
static void tree_selection_callback(GtkTreeSelection *selection, gpointer data)
{
	ch_view *chview = (ch_view*)data;
	ch_info *chinfo;
	ch_list *chlist;
	channel *ch;
	video_ctrl *vctrl;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *chname;


	if(chview == NULL) {
		return;
	} else {
		chinfo = chview->chinfo;
		chlist = chview->chlist;
		vctrl  = chview->vctrl;
	}

	if( gtk_tree_selection_get_selected(selection, &model, &iter) ) {

		gtk_tree_model_get(model, &iter, 1, &chname, -1);

		// Set selected channel
		ch = chlist_get_channel_by_name(chlist, chname);
		if(ch != NULL) {
			chview_set_selected_channel(chview, chname);

			// Info
			chinfo_set_channel(chinfo, ch);
			vctrl_set_channel(vctrl, ch);

			// Play
			vp_set_channel(chview->vplayer, ch);
			vp_play(chview->vplayer);
		}
		g_free(chname);
	}
}


static void ibox_selection_callback(GtkWidget *widget, gpointer data)
{
	GList *list     = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(widget));
	ch_view *chview = (ch_view*)data;
	ch_info *chinfo;
	ch_list *chlist;
	channel *ch;
	video_ctrl *vctrl;
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkTreePath *path;
	gchar *chname;


	if(chview == NULL || list == NULL) {
		return;
	} else {
		chinfo = chview->chinfo;
		chlist = chview->chlist;
		vctrl  = chview->vctrl;
	}

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(widget));
	path  = g_list_nth_data(list, 0);
	gtk_tree_model_get_iter(model, &iter, path);

	gtk_tree_model_get(model, &iter, 1, &chname, -1);

	// Set selected channel
	ch = chlist_get_channel_by_name(chlist, chname);
	if(ch != NULL) {
		chview_set_selected_channel(chview, chname);

		// Info
		chinfo_set_channel(chinfo, ch);
		vctrl_set_channel(vctrl, ch);

		// Play
		vp_set_channel(chview->vplayer, ch);
		vp_play(chview->vplayer);
	}
	g_list_foreach(list, tree_path_free, NULL);
	g_list_free(list);
	g_free(chname);
}
static void tree_path_free(gpointer data, gpointer user_data)
{
	gtk_tree_path_free(data);
}

