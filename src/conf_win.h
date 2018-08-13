/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     conf_win.h
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

#ifndef MOBOTV_CONF_WIN_H

	#define MOBOTV_CONF_WIN_H 1


	#include <gtk/gtk.h>
	#include <hildon/hildon-program.h>
	#include "application.h"
	#include "ui.h"
	#include "video_ctrl.h"
	#include "ch_view.h"
	#include "user_conf.h"


	GtkWidget *new_conf_win(application *app);

#endif

