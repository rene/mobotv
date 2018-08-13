/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     user_conf.h
 * Description:  Handle user configuration
 * Created:      Sex Jan  9 11:49:38 BRST 2009
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

#ifndef MOBOTV_USER_CONF_H

	#define MOBOTV_USER_CONF_H 1


	#include <glib.h>
	#include "conffile.h"
	#include "ui.h"


	#define GB_STR(x) ((x == TRUE) ? "TRUE" : "FALSE")
	#define MV_STR(x) ((x ==  0) ? "0" : "1")
	#define STR_GB(x) strncmp(x, "TRUE", 4) == 0 ? TRUE : FALSE
	#define STR_MV(x) strncmp(x, "0", 1) == 0 ? 0 : 1

	#define MOBOTV_CONF_FILE "~/.mobotv/mobotv.conf"
	#define MOBOTV_CONF_CHLIST "~/.mobotv/channels.xml"

	#define MOBOTV_VMODE_ICON	0
	#define MOBOTV_VMODE_LIST	1


	typedef struct _user_conf {
		char *conf_filename;
		gboolean ch_show;
		char view_mode;
		gboolean info_show;
		gboolean vbar_show;
		gboolean kap_ratio;
		char *ch_list_path;
		conffile_t *conf;
	} user_conf;


	user_conf *uconf_new(void);
	void uconf_destroy(user_conf *obj);

	void uconf_set_ch_show(user_conf *obj, gboolean ch_show);
	gboolean uconf_get_ch_show(user_conf *obj);

	void uconf_set_view_mode(user_conf *obj, char view_mode);
	char uconf_get_view_mode(user_conf *obj);

	void uconf_set_info_show(user_conf *obj, gboolean info_show);
	gboolean uconf_get_info_show(user_conf *obj);

	void uconf_set_vbar_show(user_conf *obj, gboolean vbar_show);
	gboolean uconf_get_vbar_show(user_conf *obj);

	void uconf_set_kap_ratio(user_conf *obj, gboolean kap_ratio);
	gboolean uconf_get_kap_ratio(user_conf *obj);

	void uconf_set_ch_list_path(user_conf *obj, char *ch_list_path);
	char *uconf_get_ch_list_path(user_conf *obj);

	void uconf_set_conf_filename(user_conf *obj, char *conf_filename);
	char *uconf_get_conf_filename(user_conf *obj);

	void uconf_set_default_conf(user_conf *obj);

	void uconf_read_conf(user_conf *obj);
	void uconf_write_conf(user_conf *obj);

#endif

