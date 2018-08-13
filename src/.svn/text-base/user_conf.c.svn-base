/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     user_conf.c
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

#include "user_conf.h"


/**
 * uconf_new
 * Instance new user_conf object
 */
user_conf *uconf_new(void)
{
	user_conf *obj;

	obj = (user_conf*)g_malloc(sizeof(user_conf));
	if(obj != NULL) {
		obj->conf_filename = NULL;
		obj->ch_list_path  = NULL;

		if(new_conffile(&obj->conf) != 0) {
		 g_free(obj);
			return(NULL);
		}

		return(obj);
	} else {
		return(NULL);
	}
}


/**
 * uconf_destroy
 * Destroy a instanced user_conf object
 */
void uconf_destroy(user_conf *obj)
{
	if(obj == NULL) return;

	if(obj->conf_filename != NULL) g_free(obj->conf_filename);
	if(obj->ch_list_path != NULL) g_free(obj->ch_list_path);

	free_conffile(&obj->conf);
 g_free(obj);
}


/**
 * Sets and Gets
 */
void uconf_set_ch_show(user_conf *obj, gboolean ch_show)
{
	if(obj == NULL) return;
	obj->ch_show = ch_show;
}
gboolean uconf_get_ch_show(user_conf *obj)
{
	if(obj == NULL) return(FALSE);
	return(obj->ch_show);
}


void uconf_set_view_mode(user_conf *obj, char view_mode)
{
	if(obj == NULL) return;
	switch(view_mode) {
		case MOBOTV_VMODE_ICON:
		case MOBOTV_VMODE_LIST:
				obj->view_mode = view_mode;
				break;
	}
}
char uconf_get_view_mode(user_conf *obj)
{
	if(obj == NULL) return(0);
	return(obj->view_mode);
}


void uconf_set_info_show(user_conf *obj, gboolean info_show)
{
	if(obj == NULL) return;
	obj->info_show = info_show;
}
gboolean uconf_get_info_show(user_conf *obj)
{
	if(obj == NULL) return(FALSE);
	return(obj->info_show);
}


void uconf_set_vbar_show(user_conf *obj, gboolean vbar_show)
{
	if(obj == NULL) return;
	obj->vbar_show = vbar_show;
}
gboolean uconf_get_vbar_show(user_conf *obj)
{
	if(obj == NULL) return(FALSE);
	return(obj->vbar_show);
}


void uconf_set_kap_ratio(user_conf *obj, gboolean kap_ratio)
{
	if(obj == NULL) return;
	obj->kap_ratio = kap_ratio;
}
gboolean uconf_get_kap_ratio(user_conf *obj)
{
	if(obj == NULL) return(FALSE);
	return(obj->kap_ratio);
}


void uconf_set_ch_list_path(user_conf *obj, char *ch_list_path)
{
	if(obj == NULL) return;
	obj->ch_list_path = alloc_new_str(ch_list_path);
}
char *uconf_get_ch_list_path(user_conf *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->ch_list_path);
}


void uconf_set_conf_filename(user_conf *obj, char *conf_filename)
{
	if(obj == NULL) return;

	if(obj->conf_filename != NULL) g_free(obj->conf_filename);

	obj->conf_filename = expand_dir_path(conf_filename, MOBOTV_EXPDIR_CREATE);
}
char *uconf_get_conf_filename(user_conf *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->conf_filename);
}


/**
 * uconf_set_default_conf
 * Set default configuration
 */
void uconf_set_default_conf(user_conf *obj)
{
	if(obj == NULL) return;

	uconf_set_conf_filename(obj, MOBOTV_CONF_FILE);
	uconf_set_ch_show(obj, TRUE);
	uconf_set_view_mode(obj, MOBOTV_VMODE_ICON);
	uconf_set_info_show(obj, FALSE);
	uconf_set_vbar_show(obj, TRUE);
	uconf_set_kap_ratio(obj, TRUE);
	uconf_set_ch_list_path(obj, MOBOTV_CONF_CHLIST);
}


/**
 * uconf_read_conf
 * Read configuration from file (pointed by conf_filename)
 */
void uconf_read_conf(user_conf *obj)
{
	char *ptr;

	if(obj == NULL) return;
	if(load_from_file(uconf_get_conf_filename(obj), &obj->conf) != 0) {
		uconf_set_default_conf(obj);
		return;
	}

	if( (ptr = get_value("ch_show", &obj->conf)) != NULL )
		uconf_set_ch_show(obj, STR_GB(ptr));

	if( (ptr = get_value("view_mode", &obj->conf)) != NULL )
		uconf_set_view_mode(obj, STR_MV(ptr));

	if( (ptr = get_value("info_show", &obj->conf)) != NULL )
		uconf_set_info_show(obj, STR_GB(ptr));

	if( (ptr = get_value("vbar_show", &obj->conf)) != NULL )
		uconf_set_vbar_show(obj, STR_GB(ptr));

	if( (ptr = get_value("kap_ratio", &obj->conf)) != NULL )
		uconf_set_kap_ratio(obj, STR_GB(ptr));

	if( (ptr = get_value("ch_list_path", &obj->conf)) != NULL )
		uconf_set_ch_list_path(obj, ptr);
}


/**
 * uconf_write_conf
 * Write configuration to file
 */
void uconf_write_conf(user_conf *obj)
{
	if(obj == NULL) return;

	char *fname = uconf_get_conf_filename(obj);

	set_key("ch_show", GB_STR(obj->ch_show), &obj->conf);
	set_key("view_mode", MV_STR(obj->view_mode), &obj->conf);
	set_key("info_show", GB_STR(obj->info_show), &obj->conf);
	set_key("vbar_show", GB_STR(obj->vbar_show), &obj->conf);
	set_key("kap_ratio", GB_STR(obj->kap_ratio), &obj->conf);
	set_key("ch_list_path", uconf_get_ch_list_path(obj), &obj->conf);

	if(fname != NULL)
		save_to_file(fname, obj->conf);
}


