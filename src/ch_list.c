/**
 * Copyright (C) 2008 Renê de Souza Pinto <rene@renesp.com.br>
 *
 * mobotv - Watch TV on mobile devices
 *
 * Filename:     ch_list.c
 * Description:  Channel list
 * Created:      Dom Jan 11 02:25:55 BRST 2009
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

#include "ch_list.h"


/** DTD for channel list XML file */
#define MOBOTV_XML_DTD  "<!DOCTYPE channel_list [\n"                     \
					    "\t<!ELEMENT channel_list (channel*)>\n"         \
		        		"\t<!ELEMENT channel (name,url,info,picture)>\n" \
				    	"\t<!ELEMENT name    (#PCDATA)>\n"               \
					    "\t<!ELEMENT url     (#PCDATA)>\n"               \
						"\t<!ELEMENT info    (#PCDATA)>\n"               \
						"\t<!ELEMENT picture (#PCDATA)>\n]>"



/** private methods */
static gint compare_by_name(gconstpointer ch, gconstpointer name);
static int nodecmp(const xmlChar *nodename, const char *name);
static void create_channel_node(gpointer data, gpointer user_data);


/**
 * chlist_new
 * Instance new ch_list object
 */
ch_list *chlist_new()
{
	ch_list *obj;

	obj = (ch_list*)g_malloc(sizeof(ch_list));
	if(obj == NULL) {
		return(NULL);
	} else {
		obj->chlist = NULL;

		/** libxml checking */
		LIBXML_TEST_VERSION
		xmlCheckVersion(LIBXML_VERSION);

		return(obj);
	}
}


/**
 * chlist_destroy
 * Destroy a instanced object
 */
void chlist_destroy(ch_list *obj)
{
	if(obj == NULL) return;

	if(obj->chlist != NULL) {
		g_slist_foreach(obj->chlist, (GFunc)(channel_destroy), NULL);
		g_slist_free(obj->chlist);
	}
 g_free(obj);
}


/**
 * chlist_add_channel
 * Add new channel to the list
 */
gboolean chlist_add_channel(ch_list *obj, channel *ch)
{
	if(obj == NULL || ch == NULL) return(FALSE);

	// check if channel is already inserted
	channel *tmp = chlist_get_channel_by_name(obj, channel_get_name(ch));

	if(tmp == NULL) {
		// Add channel
		obj->chlist = g_slist_append(obj->chlist, ch);
		return(TRUE);
	} else {
		return(FALSE);
	}
}


/**
 * chlist_del_channel
 * Delete an channel from the list
 */
gboolean chlist_del_channel(ch_list *obj, const char *chname)
{
	if(obj == NULL) return(FALSE);

	channel *ch = chlist_get_channel_by_name(obj, chname);

	if(ch == NULL) {
		return(FALSE);
	} else {
		// Remove from list
		obj->chlist = g_slist_remove(obj->chlist, ch);
	}

	return(TRUE);
}


/**
 * chlist_del_channel_byref
 * Delete an channel from the list passing the channel itself
 */
gboolean chlist_del_channel_byref(ch_list *obj, channel *ch)
{
	if(obj == NULL) return(FALSE);

	obj->chlist = g_slist_remove(obj->chlist, ch);
	return(TRUE);
}


/**
 * chlist_get_channel_by_name
 * Search and return the channel which have chname from list.
 * 
 * Return:
 * 	channel - The channel object
 * 	NULL    - Channel not found
 */
channel *chlist_get_channel_by_name(ch_list *obj, const char *chname)
{
	if(obj == NULL) return(NULL);

	channel *chfound = NULL;
	GSList  *found   = g_slist_find_custom(obj->chlist, chname, (GCompareFunc)compare_by_name);

	if(found != NULL) {
		chfound = found->data;
	}

	return(chfound);
}


/**
 * chlist_get_length
 * Return the number of channels in the list
 */
guint chlist_get_length(ch_list *obj)
{
	if(obj == NULL) return(0);

	return( g_slist_length(obj->chlist) );
}


/**
 * chlist_get_channel_at
 * Return the channel at the given position in list
 */
channel *chlist_get_channel_at(ch_list *obj, guint n)
{
	if(obj == NULL) return(NULL);

	channel *ch = g_slist_nth_data(obj->chlist, n);
	if(ch != NULL) {
		return(ch);
	} else {
		return(NULL);
	}
}


/**
 * chlist_get_channel_index
 * Return the index of a channel in the list
 */
gint chlist_get_channel_index(ch_list *obj, channel *ch)
{
	if(obj == NULL) return(-1);

	return( g_slist_index(obj->chlist, ch) );
}


/**
 * chlist_read_from_file
 * Load channels list from XML file
 *
 * Params:
 * 	filename - Should be a file path or URL
 */
gboolean chlist_read_from_file(ch_list *obj, const char *filename)
{
	xmlDocPtr doc    = NULL;
	xmlNode *root    = NULL;
	xmlNode *ch_node = NULL;
	xmlNode *s_node  = NULL;
	xmlParserCtxtPtr ctxt;
	channel *new_ch;
	char *fname;

	fname = expand_dir_path(filename, MOBOTV_EXPDIR_READ);

	// create a parser content
	ctxt = xmlNewParserCtxt();
	if(ctxt == NULL) {
		g_warning("chlist_read_from_file: Failed to create a parser content.");
		return(FALSE);
	}

	// parse and validate the file
	doc = xmlCtxtReadFile(ctxt, fname, NULL, XML_PARSE_DTDVALID);
	if(doc == NULL && ctxt != NULL) {
		g_warning("chlist_read_from_file: Failed to parse file: %s.", filename);
		xmlFreeParserCtxt(ctxt);
		return(FALSE);
	}
	if(ctxt->valid == 0) {
		g_warning("chlist_read_from_file: Failed to validate file: %s.", filename);
		xmlFreeDoc(doc);
		return(FALSE);
	}

	// clear list and get data
	chlist_destroy(obj);
	obj  = chlist_new();
	root = xmlDocGetRootElement(doc);

	if(root->type == XML_ELEMENT_NODE && nodecmp(root->name, "channel_list") == 0) {

		// Channels
		for(ch_node = root->children; ch_node; ch_node = ch_node->next) {

			if(ch_node->type == XML_ELEMENT_NODE) {
				if(nodecmp(ch_node->name, "channel") == 0) {

					// New channel
					new_ch = channel_new();

					for(s_node = ch_node->children; s_node; s_node = s_node->next) {
						if(s_node->type == XML_ELEMENT_NODE) {

							// Name
							if(nodecmp(s_node->name, "name") == 0) {
								channel_set_name(new_ch, (char*)xmlNodeGetContent(s_node));
							}

							// Url
							if(nodecmp(s_node->name, "url") == 0) {
								channel_set_url(new_ch, (char*)xmlNodeGetContent(s_node));
							}

							// Info
							if(nodecmp(s_node->name, "info") == 0) {
								channel_set_info(new_ch, (char*)xmlNodeGetContent(s_node));
							}

							// Picture
							if(nodecmp(s_node->name, "picture") == 0) {
								channel_set_icon_path(new_ch, (char*)xmlNodeGetContent(s_node));
							}
						}
					}

					// Add to list
					chlist_add_channel(obj, new_ch);
				} else {
					g_warning("chlist_read_from_file: Unknown element (%s) in XML file.", ch_node->name);
				}
			}
		}

	} else {
		g_warning("chlist_read_from_file: Unknown element (%s) in XML file.", ch_node->name);
	}

	xmlFreeDoc(doc);
	xmlFreeParserCtxt(ctxt);
	if(fname != NULL) g_free(fname);

	return(TRUE);
}


/**
 * chlit_write_to_file
 * Write channels list to XML file
 */
gboolean chlist_write_to_file(ch_list *obj, const char *filename)
{
	xmlDocPtr  doc     = NULL;
	xmlNodePtr root    = NULL;
	xmlChar *xmlbuffer;
	FILE *fp;
	char *fname;
	int buff_size, p;

	if(obj == NULL) return(FALSE);


	// Open/Create file
	fname = expand_dir_path(filename, MOBOTV_EXPDIR_READ);
	if((fp = fopen(fname, "w+")) == NULL) {
		if(fname != NULL) g_free(fname);
		return(FALSE);
	}

	// Generate XML
	doc  = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "channel_list");
	xmlDocSetRootElement(doc, root);

	g_slist_foreach(obj->chlist, (GFunc)create_channel_node, root);

	xmlDocDumpFormatMemory(doc, &xmlbuffer, &buff_size, 1);

	if(xmlbuffer == NULL) {
	 g_free(fname);
		xmlFreeDoc(doc);
		return(FALSE);
	}

	// Save to file
	p = 0;
	while(xmlbuffer[p] != '\n') {
		fprintf(fp, "%c", (char)xmlbuffer[p]);
		p++;
	}
	fprintf(fp, "\n");
	fprintf(fp, "%s", MOBOTV_XML_DTD);
	fprintf(fp, "%s\n", (char*)&xmlbuffer[p]);
	fclose(fp);

 g_free(fname);
	xmlFree(xmlbuffer);
	xmlFreeDoc(doc);
	return(TRUE);
}


/** Private methods  */

/**
 * compare_by_name
 * Compare the name of with an given channel
 *
 * Params:
 * 		ch   - Channel to compare
 * 		name - Name
 */
static gint compare_by_name(gconstpointer ch, gconstpointer name)
{
	channel *u_ch = (channel*)ch;
	char *u_name  = (char*)name;
	char *cname   = NULL;

	if(u_ch == NULL) {
		return(-1);
	} else {
		cname = channel_get_name(u_ch);
	}
	if(u_name == NULL) {
		return(1);
	}

	if(cname == NULL) {
		return(-1);
	} else {
		return( strcmp(cname, u_name) );
	}
}


/**
 * nodecmp
 * Compare a xmlChar* node name with an given name
 */
static int nodecmp(const xmlChar *nodename, const char *name)
{
	int i, j, k, p;

	if(nodename == NULL || name == NULL) return(-1);

	i = 0;
	while(nodename[i] != '\0') i++;
	j = 0;
	while(name[j] != '\0') j++;

	if(i <= j) {
		p = i;
	} else {
		p = j;
	}

	for(k=0; k<p; k++) {
		if(nodename[k] < name[k]) {
			return(-1);
		}
		if(nodename[k] > name[k]) {
			return(1);
		}
	}

	return(0);
}


/**
 * create_channel_node
 * Create XML nodes for a channel
 */
static void create_channel_node(gpointer data, gpointer user_data)
{
	channel    *ch   = (channel*)data;
	xmlNodePtr root  = (xmlNodePtr)user_data;
	xmlNodePtr chxml = NULL;

	// Channel node
	chxml = xmlNewChild(root, NULL, BAD_CAST "channel", NULL);

	// Childrens
	xmlNewChild(chxml, NULL, BAD_CAST "name",    BAD_CAST channel_get_name(ch));
	xmlNewChild(chxml, NULL, BAD_CAST "url",     BAD_CAST channel_get_url(ch));
	xmlNewChild(chxml, NULL, BAD_CAST "info",    BAD_CAST channel_get_info(ch));
	xmlNewChild(chxml, NULL, BAD_CAST "picture", BAD_CAST channel_get_icon_path(ch));
}


