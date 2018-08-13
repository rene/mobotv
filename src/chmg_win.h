/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     chmg_win.h
 * Description:  Channel manager window
 * Created:      Sex Jan  9 02:20:23 BRST 2009
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

#ifndef MOBOTV_CHMG_WIN_H

	#define MOBOTV_CHMG_WIN_H 1


	#include <gtk/gtk.h>
	#include <hildon/hildon-program.h>
	#include <hildon/hildon-banner.h>
	#include "application.h"
	#include "input_dialog.h"
	#include "ui.h"


	GtkWidget *new_chmg_win(application *app);

#endif

