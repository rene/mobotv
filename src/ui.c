/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ui.c
 * Description:  Some interface and utilities functions
 * Created:      Qui Jan  8 19:30:52 BRST 2009
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

#include "ui.h"

/**
 * new_img_button
 * Create a image button
 */
GtkWidget *new_img_button(const gchar *icon_path)
{
	GtkWidget *img;
	GtkWidget *bt;

	bt    = gtk_button_new();
	img   = gtk_image_new_from_file(icon_path);
	gtk_button_set_image(GTK_BUTTON(bt), img);

	return(bt);
}


/**
 * new_img_toggle_button
 * Create a image toggle button
 */
GtkWidget *new_img_toggle_button(const gchar *icon_path)
{
	GtkWidget *img;
	GtkWidget *bt;

	bt    = gtk_toggle_button_new();
	img   = gtk_image_new_from_file(icon_path);
	gtk_button_set_image(GTK_BUTTON(bt), img);

	return(bt);
}


/**
 * new_img_menu_item
 * Create a image menu item
 */
GtkWidget *new_img_menu_item(const gchar *label, const gchar *icon_path)
{
	GtkWidget *img;
	GtkWidget *mnu_item;

	mnu_item = gtk_image_menu_item_new_with_label(label);
	img      = gtk_image_new_from_file(icon_path);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(mnu_item), img);

	return(mnu_item);
}


/**
 * Alloc new string with contents
 */
char *alloc_new_str(const char *str)
{
	char *tmp;
	size_t i;

	if(str == NULL) return(NULL);

	i = 0;
	while(str[i] != '\0') i++;

	tmp = (char*)g_malloc(sizeof(char) * (i + 1));
	if(tmp != NULL) {
		memcpy(tmp, str, i);
		tmp[i] = '\0';
	}
	return(tmp);
}


/**
 * expand_dir_path
 * Parse directory path expandind '~' in home directory
 * and creating directories as necessary
 *
 * Params:
 * 		path - Path to expand
 * 		mode - MOBOTV_EXPDIR_READ   : Just expand '~'
 * 		       MOBOTV_EXPDIR_CREATE : Expand '~' and try to create
 * 		                              directories as necessary
 *
 * Return:
 * 		complete path
 */
char *expand_dir_path(const char *path, char mode)
{
	char *home_dir, *nstr, *str, *tmp;
	const char *str2;
	size_t s1, s2;
	char found;
	DIR *dir;
	int p;

	nstr = NULL;
	if(path != NULL) {
		p = 0;
		while(path[p] == ' ' && path[p] != '\0') p++;

		// Check for home path
		if(path[p] == '~') {
			if( (home_dir = getenv("HOME")) != NULL ) {
				str2 = &path[++p];
				s1   = strlen(home_dir);
				s2   = strlen(str2);

				// Cat the strings
				nstr = (char*)g_malloc(sizeof(char) * (s1 + s2));
				if(nstr != NULL) {
					strcpy(nstr, home_dir);
					strcat(nstr, str2);
				}
			}
		} else {
			nstr = alloc_new_str(path);
		}
		if(mode == MOBOTV_EXPDIR_READ) {
			return(nstr);
		}


		// Validate path (create dir as necessary)
		tmp = alloc_new_str(nstr);
		str = alloc_new_str(tmp);

		p     = 0;
		s1    = strlen(tmp);
		found = 0;
		while(p < s1) {
			if(tmp[p] == '/') {
				if(found == 0) {
					found = 1;
				} else {
					// Check dir
					strncpy(str, tmp, p);
					str[p] = '\0';

					if( (dir = opendir(str)) == NULL ) {
						if(errno == ENOENT) {
							// Create dir
							if(mkdir(str, S_IRWXU) == -1)
								g_warning("error: Cannot create directory %s", str);
						} else {
							g_warning("error: Cannot read directory %s.", str);
						}
					} else {
						closedir(dir);
					}
				}
			}
			p++;
		}
	 g_free(tmp);
	 g_free(str);
	}
	return(nstr);
}


