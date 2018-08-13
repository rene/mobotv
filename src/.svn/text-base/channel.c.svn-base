/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     channel.c
 * Description:  Channel class
 * Created:      Sex Jan  9 10:42:11 BRST 2009
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

#include "channel.h"


/**
 * channel_new
 * Instance new channel object
 */
channel *channel_new(void)
{
	channel *obj;

	obj = (channel*)g_malloc(sizeof(channel));
	if(obj != NULL) {
		obj->name      = NULL;
		obj->url       = NULL;
		obj->info      = NULL;
		obj->icon_path = NULL;
		obj->icon_img  = NULL;
		return(obj);
	} else {
		return(NULL);
	}
}


/**
 * channel_destroy
 * Destroy a instanced channel object
 */
void channel_destroy(channel *obj)
{
	if(obj == NULL) return;

	if(obj->name != NULL)      g_free(obj->name);
	if(obj->url != NULL)       g_free(obj->url);
	if(obj->info != NULL)      g_free(obj->info);
	if(obj->icon_path != NULL) g_free(obj->icon_path);

 g_free(obj);
}


/**
 * Sets and Gets
 */
void channel_set_name(channel *obj, const char *name)
{
	if(obj == NULL) return;
	obj->name = alloc_new_str(name);
}
char *channel_get_name(channel *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->name);
}


void channel_set_url(channel *obj, const char *url)
{
	if(obj == NULL) return;
	obj->url = alloc_new_str(url);
}
char *channel_get_url(channel *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->url);
}


void channel_set_info(channel *obj, const char *info)
{
	if(obj == NULL) return;
	obj->info = alloc_new_str(info);
}
char *channel_get_info(channel *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->info);
}


void channel_set_icon_path(channel *obj, const char *icon_path)
{
	if(obj == NULL) return;
	obj->icon_path = alloc_new_str(icon_path);
	obj->icon_img  = gtk_image_new_from_file(icon_path);
}
char *channel_get_icon_path(channel *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->icon_path);
}
GtkWidget *channel_get_icon_img(channel *obj)
{
	if(obj == NULL) return(NULL);
	return(obj->icon_img);
}


