/*
 * template_functions.c
 * https://github.com/dafky2000/simplectemplate
 * Provides HTML template functionality for https://github.com/andy5995/mhwkb
 *
 * Copyright 2017 Daniel Kelly <myself@danielkelly.me>
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#ifndef UNITTESTS
	#define STATIC static
#else
	#define STATIC
#endif

#ifndef TEMPLATE_FUNCTIONS_H
#include "template_functions.h"
#endif

/**
 * Read the contents of a file
 * filename: Relative path of the file to read
 * returns: null-terminated string of the file contents
 */
STATIC char* read_file_contents(const char* filename) {
	// Open ze file
	FILE* fp = fopen(filename, "r");
	if(fp == NULL) return NULL;

	// Get the content length
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Get the contents
	char* contents = malloc(length+1);
	memset(contents, 0, length+1);

	fread(contents, 1, length, fp);

	// No need to check if we fclose properly?
	// Even if the call fails, the stream passed as parameter will no longer be associated with the file nor its buffers.
	fclose(fp);

	if(!contents) return contents;

	// Remove the newline from the end but preserve a valid pointer so we can tell the difference between an "error" (when NULL) and simply an empty file.
	if(length) {
		char* removednl = malloc(length);
		memset(removednl, 0, length);
		memcpy(removednl, contents, length-1);
		free(contents);
		return removednl;
	}

	return contents;
}

// "Stolen" from https://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
/**
 * Replace all occurances of rep
 * orig: text to perfom search and replace on
 * rep: text to search for
 * with: text to replace with
 * returns: replaced text
 */
// You must free the result if result is non-NULL.
STATIC char* str_replace(char* orig, const char* rep, const char* with) {
	char* result;  // the return string
	char* ins;     // the next insert point
	char* tmp;     // varies
	unsigned long len_rep;   // length of rep (the string to remove)
	unsigned long len_with;  // length of with (the string to replace rep with)
	unsigned long count;     // number of replacements

	// sanity checks and initialization
	if(!orig || !rep) return NULL;

	len_rep = strlen(rep);
	if(len_rep == 0) return NULL; // empty rep causes infinite loop during count

	if(!with) with = "";
	len_with = strlen(with);

	// count the number of replacements needed
	ins = orig;     // the next insert point
	for(count = 0; (tmp = strstr(ins, rep)); ++count) {
		ins = tmp + len_rep;
	}

	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if(!result) return NULL;

	// first time through the loop, all the variable are set correctly from here on,
	//    tmp points to the end of the result string
	//    ins points to the next occurrence of rep in orig
	//    orig points to the remainder of orig after "end of rep"
	while(count--) {
			ins = strstr(orig, rep);
			unsigned long len_front = ins - orig;
			tmp = strncpy(tmp, orig, len_front) + len_front;
			tmp = strcpy(tmp, with) + len_with;
			orig += len_front + len_rep; // move to next "end of rep"
	}
	strcpy(tmp, orig);

	return result;
}

STATIC const char* get_last_token(const char* current, unsigned int token_count, const char* tokens[]) {
	unsigned long remaining_len = strlen(current);
	const char* next_token = &current[remaining_len];
	const char* last_delimiter = NULL;

	unsigned int i = 0;
	for(; i < token_count; ++i) {
		if(remaining_len >= strlen(tokens[i])) {
			char* subject_token = strstr(current, tokens[i]);
			if(subject_token != NULL && subject_token < next_token) {
				next_token = subject_token;
				last_delimiter = tokens[i];
			}
		}
	}

	return last_delimiter;
}

STATIC const char* my_strtok(const char* current, unsigned int token_count, const char* tokens[]) {
	unsigned long remaining_len = strlen(current);
	const char* next_token = &current[remaining_len];

	unsigned int i = 0;
	for(; i < token_count; ++i) {
		if(remaining_len >= strlen(tokens[i])) {
			char* subject_token = strstr(current, tokens[i]);

			if(subject_token != NULL && subject_token < next_token) {
				next_token = subject_token;
			}
		}
	}

	if(next_token < &current[remaining_len]) return next_token;

	return NULL;
}

STATIC unsigned int get_surrounded_with(const char* template, const char* open, const char* close, long* start, long* length) {
	*start = -1;
	*length = -1;

	const char* tokens[] = { open, close };

	char template_copy[strlen(template) + 1];
	strcpy(template_copy, template);

	const char* current = my_strtok(template_copy, 2, tokens);
	while(current != NULL) {
		const char* last_token = get_last_token(current, 2, tokens);

		if(last_token == open) *start = current - template_copy + strlen(open);
		else if(*start > -1 && last_token == close) *length = (current - template_copy) - *start;

		if(*start > -1 && *length > -1) {
			return 1;
		}

		current = my_strtok(&current[1], 2, tokens);
	}

	return 0;
}

char* my_render_template(const char* template_data, unsigned long len, const char* data[], struct RenderOptions options) {
	const char* keys[len];
	const char* values[len];
	unsigned long i;
	for(i = 0; i < len; i++) {
		keys[i] = (char *)data[i*2];
		values[i] = (char *)data[i*2+1];
	}

	// Get the options and set defaults if they aren't set.
	const char* open = options.placeholder_open;
	const char* close = options.placeholder_close;
	const char* data_cond_separator = options.data_cond_separator;
	const char* data_cond_open_prefix = options.data_cond_open_prefix;
	const char* data_cond_close_prefix = options.data_cond_close_prefix;

	// Setup defaults
	if(open == NULL) open = "{{";
	if(close == NULL) close = "}}";
	if(data_cond_separator == NULL) data_cond_separator = " ";
	if(data_cond_open_prefix == NULL) data_cond_open_prefix = "#";
	if(data_cond_close_prefix == NULL) data_cond_close_prefix = "/";

	// Create a copy of the template to work with
	unsigned long template_length = strlen(template_data) + 1;
	char* output = malloc(template_length);
	strcpy(output, template_data);

	long start = 0, match_len = 0;
	while(get_surrounded_with(output, open, close, &start, &match_len)) {
		char* matched_start = output + start;

		// Get the match, inside the open and close braces
		char matched_copy[match_len+1];
		memset(matched_copy, 0, match_len + 1);
		strncpy(matched_copy, matched_start, match_len);

		/* int is_condition_sep = 0; */
		int is_condition_pre = 0;
		char* data = NULL;

		// If we have the separator
		char* data_separated = strstr(matched_copy, data_cond_separator);
		if(data_separated != NULL) {
			/* is_condition_sep = 1; */
			data_separated += strlen(data_cond_separator);

			// Copy the separated data to the data
			data = malloc(strlen(data_separated) + 1);
			memset(data, 0, strlen(data_separated) + 1);
			strcpy(data, data_separated);

			/* printf("Element '%s' has condition separator, value = '%s'\n", matched_copy, data); */
		}

		// If we start with a condition
		if(strstr(matched_copy, data_cond_open_prefix) == matched_copy) {
			is_condition_pre = 1;

			/* printf("Element '%s' has condition prefix at (%u)\n", matched_copy, (unsigned)matched_start); */

			// 1) Get the closing element {{/data}}
			unsigned long closing_len =
				strlen(open) +
				strlen(data_cond_close_prefix) +
				strlen(matched_copy) + strlen(data_cond_open_prefix) +
				strlen(close);

			char closing_text[closing_len + 1];
			memset(closing_text, 0, closing_len + 1);
			strcpy(closing_text, open);
			strcat(closing_text, data_cond_close_prefix);
			strcat(closing_text, matched_copy + strlen(data_cond_open_prefix));
			strcat(closing_text, close);

			char* closing_instance = strstr(matched_start, closing_text);
			// TODO: Add support for placeholder with no end
			if(closing_instance != NULL) {
				// 2) Get the data the placeholders
				char* data_inside_start = matched_start + strlen(matched_copy) + strlen(open);
				unsigned long data_inside_len = (unsigned long)closing_instance - (unsigned long)data_inside_start;
				char data_inside[data_inside_len + 1];
				memset(data_inside, 0, data_inside_len + 1);
				strncpy(data_inside, data_inside_start, data_inside_len);

				// 3) Set the data to the replace inner text
				// Append to the existing data
				unsigned long new_data_len = data_inside_len;
				if(data) new_data_len += strlen(data);

				char* new_data = malloc(new_data_len + 1);
				memset(new_data, 0, new_data_len + 1);
				if(data) strcpy(new_data, data);
				strcat(new_data, data_inside);
				if(data) free(data);
				data = new_data;

				// 4) Set match_len so we have the whole match
				match_len = ((unsigned long)closing_instance + closing_len) - (unsigned long)matched_start - strlen(open) - strlen(close);

			}
		}

		/* if(data) printf("Data: '%s'\n", data); */

		// Reassemble the entire placeholder and replace it
		/* printf("match_len: '%u'\n", match_len); */
		char toreplace[strlen(open) + match_len + strlen(close) + 1];
		strcpy(toreplace, open);
		strncat(toreplace, matched_start, match_len);
		strcat(toreplace, close);
		/* printf("toreplace: '%s'\n", toreplace); */

		// TODO: Need to compare to the actual end of the string or a var "cat" would also match for "cats_ass"
		char* startofkey = matched_copy;
		if(is_condition_pre) {
			startofkey += strlen(data_cond_open_prefix);
		}

		/* printf("startokey: '%s'\n", startofkey); */

		// Get the token value from the input data
		unsigned int i = 0;
		for(; i < len; ++i) {
			unsigned int keylen = strlen(keys[i]);
			char key[keylen + 1];
			strcpy(key, keys[i]);
			/* printf("key: '%s'\n", key); */

			int cmp_res = strncmp(startofkey, key, keylen);
			if(cmp_res == 0) {
				/* printf("FOUND\n"); */
				char* replaced;
				if(data) {
					/* printf("HAVE DATA\n"); */
					if(values[i] && strlen(values[i])) {
						/* printf("TRUE VALUE\n"); */
						replaced = str_replace(output, toreplace, data);
					} else {
						/* printf("FALSE VALUE\n"); */
						replaced = str_replace(output, toreplace, "");
					}
				} else {
					/* printf("SUB VALUE\n"); */
					replaced = str_replace(output, toreplace, values[i]);
				}

				free(output);
				output = replaced;
				break;
			}
		}

		if(data) free(data);

		// If the key wasn't found
		if(i >= len) {
			char* replaced = str_replace(output, toreplace, "");
			free(output);
			output = replaced;
		}
	}

	return output;
}

char* render_template(const char* template_data, unsigned long len, const char* data[]) {
	return my_render_template(template_data, len, data, (struct RenderOptions){});
}

char* my_render_template_file(const char* filename, unsigned long len, const char* data[], struct RenderOptions options) {
	char* contents = read_file_contents(filename);
	if(!contents) return NULL;

	char* rendered = my_render_template(contents, len, data, options);
	free(contents);

	return rendered;
}

char* render_template_file(const char* filename, unsigned long len, const char* data[]) {
	return my_render_template_file(filename, len, data, (struct RenderOptions){});
}
