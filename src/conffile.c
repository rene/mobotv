/**
 * Copyright (C)2007 Renê de Souza Pinto	<rene@renesp.com.br>
 *
 * libconffile - Library to manage configuration files in a easy way
 *
 * File: conffile.c
 * Contents: Functions
 *
 * Created: 23/08/2008
 * Updated: 24/08/2008
 *
 *  This file is part of libconffile.
 * 
 *  libconffile is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation a, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  libconffile is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 * 
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with libconffile.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "conffile.h"

#define _REGEXP_KEY         "%s[ ]{0,}=[ ]{0,}(.*)"
#define _REGEXP_KEY_LENGTH  strlen(key) + 19


/**
 * new_conffile
 * Create a new conffile_t struct
 *
 * @param conf conffile_t struct
 * @return 0 on success, error flag ENOMEM otherwise
 */
int new_conffile(conffile_t **conf)
{
	conffile_t *uconf;

	uconf = (conffile_t*)malloc(sizeof(conffile_t));
	if(uconf == NULL) {
		return(ENOMEM);
	}

	uconf->filename = NULL;
	uconf->contents = NULL;
	uconf->nlines   = 0;
	uconf->str      = NULL;

	*conf = uconf;
	return(0);
}


/**
 * free_conffile
 * Frees memory used by conffile_t
 *
 * @param conf conffile_t struct
 */
void free_conffile(conffile_t **conf)
{
	conffile_t *uconf = *conf;
	long i, nlines;

	if(uconf != NULL) {
		nlines = uconf->nlines;
		if(uconf->contents != NULL) {
			for(i=0; i<nlines; i++) {
				if(uconf->contents[i] != NULL) {
					free(uconf->contents[i]);
				}
			}
			free(uconf->contents);
		}

		if(uconf->filename) {
			free(uconf->filename);
		}

		if(uconf->str) {
			free(uconf->str);
		}

		free(uconf);
	}
}


/**
 * load_from_file
 * Load all keys from configuration file
 *
 * @param filename File name
 * @param conf conffile_t struct
 * @return 0 on success, error flag otherwise (see errno.h)
 */
int load_from_file(char *filename, conffile_t **conf)
{
	FILE *fp;
	conffile_t *uconf = *conf;
	long fsize, pos_s, pos_e, lsize;
	long nlines, pline;
	char *buff;

	// Open file
	if((fp = fopen(filename, "r")) == NULL) {
		return(errno);
	}

	// Read whole file and split in lines
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buff = (char*)malloc(sizeof(char) * fsize);
	if(buff == NULL) {
		fclose(fp);
		return(ENOMEM);
	} else {
		fread(buff, sizeof(char), fsize, fp);
	}
	fclose(fp);

	pos_s = nlines = 0;
	while(pos_s < fsize) {
		if(buff[pos_s] == '\n') nlines++;
		pos_s++;
	}
	if(pos_s > 0) {
		uconf->contents = (char **)malloc(sizeof(char*) * nlines);
		if(uconf->contents == NULL) {
			free(buff);
			return(ENOMEM);
		} else {
			uconf->nlines = nlines;
		}
	} else {
		free(buff);
		return(-1);
	}

	pos_s = pos_e = pline = 0;
	while(pos_e < fsize && pline < nlines) {
		if(buff[pos_e] != '\n') {
			pos_e++;
		} else {
			lsize = pos_e - pos_s;
			if(lsize == 0) {
				uconf->contents[pline] = NULL;
			} else {
				uconf->contents[pline] = (char *)malloc(sizeof(char) * (lsize + 1));
				memcpy(uconf->contents[pline], &buff[pos_s], lsize);
				uconf->contents[pline][lsize] = '\0';
			}
			pos_s = pos_e + 1;
			pos_e++;
			pline++;
		}
	}
	free(buff);

	// Copy filename
	uconf->filename = (char *)malloc(sizeof(char *) * strlen(filename));
	if(uconf->filename != NULL) {
		strcpy(uconf->filename, filename);
	}

	return(0);
}


/**
 * save_to_file
 * Save all configuration into a file
 *
 * @param filename File name (NULL for use default file)
 * @param conf conffile_t struct
 * @return 0 on success, error flag otherwise (see errno.h)
 */
int save_to_file(char *filename, conffile_t *conf)
{
	conffile_t *uconf = conf;
	FILE *fp;
	char *fname;
	long i;

	// Open file
	if(filename == NULL) {
		if(uconf->filename != NULL) {
			fname = uconf->filename;
		} else {
			return(ENOENT);
		}
	} else {
		fname = filename;
	}
	if((fp = fopen(fname, "w+")) == NULL) {
		return(errno);
	}

	// Save contents
	for(i=0; i < uconf->nlines; i++) {
		if(uconf->contents[i] != NULL) {
			fprintf(fp, "%s\n", uconf->contents[i]);
		}
	}

	fclose(fp);
	return(0);
}


/**
 * set_key
 * Set or create a configuration key with a value
 *
 * @param key configuration key
 * @param value value for the key
 * @param conf conffile_t struct
 * @return 0 on success, -1 otherwise
 */
int set_key(char *key, char *value, conffile_t **conf)
{
	conffile_t *uconf = *conf;
	regex_t rexp;
	char *regexp;
	char **nptr;
	long pline;
	long i, esize;

	// Sanity check
	if(strlen(key) == 0)
		return(-1);

	// Create regular expression to find the key
	esize  = _REGEXP_KEY_LENGTH + 1;
	regexp = (char *)malloc(sizeof(char) * esize);
	if(regexp == NULL) {
		return(-1);
	}
	sprintf(regexp, _REGEXP_KEY, key);
	if(regcomp(&rexp, regexp, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
		free(regexp);
		return(-1);
	}

	// Find and replace key with new value
	pline = 0;
	while(pline < uconf->nlines) {
		if(uconf->contents[pline] != NULL) {
			if(regexec(&rexp, uconf->contents[pline], 0, NULL, 0) == 0) {
				// Replace old value
				free(uconf->contents[pline]);

				esize = strlen(key) + strlen(value) + 2;
				uconf->contents[pline] = (char*)malloc(sizeof(char) * esize);
				if(uconf->contents[pline] == NULL) {
					return(-1);
				}
				sprintf(uconf->contents[pline], "%s=%s", key, value);
				break;
			}
		}
		pline++;
	}

	// Not found, add the new key
	if(pline == uconf->nlines) {
		// Search for a blank line
		for(i=0; i < uconf->nlines; i++) {
			if(uconf->contents[i] == NULL) break;
		}
		if(i == uconf->nlines) {
			// Reallocate memory
			nptr = (char **)realloc(uconf->contents, sizeof(char*) * (uconf->nlines + 1));
			if(nptr == NULL) {
				return(-1);
			}
			uconf->contents = nptr;
			uconf->nlines++;
		} else {
			pline = i;
		}

		// Add new key
		esize = strlen(key) + strlen(value) + 2;
		uconf->contents[pline] = (char*)malloc(sizeof(char) * esize);
		if(uconf->contents[pline] == NULL) {
			return(-1);
		}
		sprintf(uconf->contents[pline], "%s=%s", key, value);
	}

	free(regexp);
	return(0);
}


/**
 * get_value
 * Return the value of a key
 *
 * @param key key
 * @param conf conffile_t struct
 * @return value of the key or NULL if not found
 */
char *get_value(char *key, conffile_t **conf)
{
	conffile_t *uconf = *conf;
	regex_t rexp;
	char *regexp, *ptr, *ptr2;
	long pline;
	long esize;

	// Create regular expression to find the key
	esize  = _REGEXP_KEY_LENGTH + 1;
	regexp = (char *)malloc(sizeof(char) * esize);
	if(regexp == NULL) {
		return(NULL);
	}
	sprintf(regexp, _REGEXP_KEY, key);
	if(regcomp(&rexp, regexp, REG_EXTENDED | REG_NOSUB | REG_ICASE) != 0) {
		free(regexp);
		return(NULL);
	}

	// Find key
	pline = 0;
	while(pline != uconf->nlines) {
		if(uconf->contents[pline] != NULL) {
			if(regexec(&rexp, uconf->contents[pline], 0, NULL, 0) == 0) {

				if(uconf->str != NULL) free(uconf->str);

				esize      = strlen(uconf->contents[pline]);
				uconf->str = (char*)malloc(sizeof(char) * esize);
				if(uconf->str == NULL)
					return(NULL);

				// Get the value
				if( (ptr = index(uconf->contents[pline], '=')) != NULL ) {
					memcpy(uconf->str, ++ptr, esize);

					if( (ptr2 = index(uconf->str, '#')) != NULL) {
						ptr2[0] = '\0';
					} else {
						uconf->str[esize-1] = '\0';
					}

					free(regexp);
					return(uconf->str);
				}
				break;
			}
		}
		pline++;
	}
	free(regexp);
	return(NULL);
}


/**
 * remove_key
 * Remove a key from configuration
 *
 * @param key key
 * @param conf conffile_t struct
 * @return 0 on success, -1 otherwise
 */
int remove_key(char *key, conffile_t **conf)
{
	conffile_t *uconf = *conf;
	regex_t rexp;
	char *regexp;
	long pline;
	long esize;

	// Create regular expression to find the key
	esize = (strlen(key) * 2) + 21;
	regexp = (char *)malloc(sizeof(char) * esize);
	if(regexp == NULL) {
		return(-1);
	}
	sprintf(regexp, "%s[ ]{1,}?=[ ]{1,}?\\w+", key);
	if(regcomp(&rexp, regexp, REG_EXTENDED | REG_NOSUB | REG_ICASE) != 0) {
		free(regexp);
		return(-1);
	}

	// Find key
	pline = 0;
	while(pline != uconf->nlines) {
		if(uconf->contents[pline] != NULL) {
			if(regexec(&rexp, uconf->contents[pline], 0, NULL, 0) == 0) {
				// Remove
				free(uconf->contents[pline]);
				uconf->contents[pline] = NULL;
				break;
			}
		}
		pline++;
	}

	free(regexp);
	return(0);
}

