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

#ifndef MOBOTV_ABOUT_WIN_H

	#define MOBOTV_ABOUT_WIN_H 1


	#include <gtk/gtk.h>
	#include <hildon/hildon-program.h>
	#include "ui.h"

	#ifdef HAVE_CONFIG_H
		#include <config.h>
	#else
		#warning config.h not found. Using extreme generic definitions !
		#define PACKAGE_BUGREPORT "rene@renesp.com.br"
		#define PACKAGE_VERSION "X.XX"
	#endif


	GtkWidget *new_about_win(void);

#endif

