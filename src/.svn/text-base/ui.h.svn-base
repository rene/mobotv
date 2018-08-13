/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ui.h
 * Description:  Some interface functions
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

#ifndef MOBOTV_UI_H

	#define MOBOTV_UI_H 1

	#ifdef HAVE_CONFIG_H
		#include <config.h>
	#else
		#warning config.h not found. Using default path for pixmaps directory.
		#define PIXMAPS_DIR "/usr/share/pixmaps/"
	#endif


	#include <stdlib.h>
	#include <string.h>
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <errno.h>
	#include <gtk/gtk.h>

	#define MOBOTV_PIXMAP_FOLDER 	PIXMAPS_DIR "/mobotv"  //"./pixmaps"
	#define MOBOTV_ICON				MOBOTV_PIXMAP_FOLDER "/mobotv.png"
	#define MOBOTV_ICON_PREVIOUS	MOBOTV_PIXMAP_FOLDER "/media-skip-backward.png"
	#define MOBOTV_ICON_PLAY 		MOBOTV_PIXMAP_FOLDER "/media-playback-start.png"
	#define MOBOTV_ICON_STOP 		MOBOTV_PIXMAP_FOLDER "/media-playback-stop.png"
	#define MOBOTV_ICON_NEXT 		MOBOTV_PIXMAP_FOLDER "/media-skip-forward.png"
	#define MOBOTV_ICON_OPEN 		MOBOTV_PIXMAP_FOLDER "/folder-open.png"
	#define MOBOTV_ICON_CHMG		MOBOTV_PIXMAP_FOLDER "/stock_bookmark.png"
	#define MOBOTV_ICON_INFO 		MOBOTV_PIXMAP_FOLDER "/document-properties.png"
	#define MOBOTV_ICON_PREFS 		MOBOTV_PIXMAP_FOLDER "/preferences-system.png"
	#define MOBOTV_ICON_ABOUT 		MOBOTV_PIXMAP_FOLDER "/bookmark-new.png"
	#define MOBOTV_ICON_QUIT		MOBOTV_PIXMAP_FOLDER "/system-log-out.png"
	#define MOBOTV_ICON_OK			MOBOTV_PIXMAP_FOLDER "/dialog-apply.png"
	#define MOBOTV_ICON_CANCEL		MOBOTV_PIXMAP_FOLDER "/application-exit.png"

	#define MOBOTV_EXPDIR_READ      0
	#define MOBOTV_EXPDIR_CREATE	1

	GtkWidget *new_img_button(const gchar *icon_path);
	GtkWidget *new_img_toggle_button(const gchar *icon_path);
	GtkWidget *new_img_menu_item(const gchar *label, const gchar *icon_path);
	char *alloc_new_str(const char *str);
	char *expand_dir_path(const char *path, char mode);

#endif


