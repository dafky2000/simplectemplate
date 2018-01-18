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

typedef struct RenderOptions {
	const char* placeholder_open;       // "{{"
	const char* placeholder_close;      // "}}"
	const char* placeholder_cond_separator;    // " " If a separator is defined than we assume the structure {{cond data}}
	const char* placeholder_cond_open_prefix;  // "#" If a prefix is defined than we assume this is {{#cond}} data {{/cond}} (mustache style)
	const char* placeholder_cond_close_prefix; // "/"
	const char* data_list_suffix; // "[]" To define a value as a list
	const char* data_object_suffix; // "." To define properties of a value
} renderoptions;

typedef struct Vector {
	void** data;
	size_t capacity; /* total capacity */
	size_t size; /* number of elements in vector */
} vector;

typedef struct MultiMap {
	const char* key;
	vector* data;
	struct MultiMap* first;
	struct MultiMap* next;
	struct MultiMap* previous;
} multimap;

STATIC int vector_grow(vector* v, size_t add_capacity) {
	unsigned int new_size = v->capacity + add_capacity;

	void** container = v->data;
	void** elems = *container;

	void* new_vector = realloc(elems, new_size * sizeof(void*));
	// Out of memory
	if(!new_vector) {
		return -1; /* LCOV_EXCL_LINE */
	}

	// Assign the new pointer
	*container = new_vector;

	// New capacity
	v->capacity = new_size;

	return 0;
}

STATIC int vector_init(vector* v, size_t init_capacity) {
	memset(v, 0, sizeof(vector));

	void** container = malloc(sizeof(void*));
	void** elems = malloc(init_capacity * sizeof(void*));

	// Out of memory
	if (!container || !elems) {
		return -1; /* LCOV_EXCL_LINE */
	}

	memset(elems, 0, init_capacity * sizeof(void*));

	*container = elems;

	v->data = container;
	v->size = 0;
	v->capacity = init_capacity;

	return 0; /* success */
}

STATIC int vector_push_back(vector* v, void* element) {
	if(v->size >= v->capacity) {
		if(vector_grow(v, 10) != 0) {
			return -1; /* LCOV_EXCL_LINE */
		}
	}

	// Add the element to the next memory location
	void** elems = *v->data;
	if(element) {
		elems[v->size++] = element;
	}

	return 0;
}

STATIC void* vector_get(vector* v, unsigned int index) {
	if(index < v->size) {
		void** container = v->data;
		void** elems = *container;
		return elems[index];
	}

	return NULL;
}

STATIC void vector_free(vector* v) {
	if(v) {
		void** container = v->data;

		if(container) {
			void** elems = *container;

			if(elems) {
				unsigned int i = 0;
				for(i = 0; i < v->size; ++i) {
					if(elems[i]) {
						free(elems[i]);
					}
				}

				free(elems);
			}

			free(container);
		}

		free(v);
	}
}

STATIC int multimap_init(multimap* mm) {
	memset(mm, 0, sizeof(multimap));

	mm->data = malloc(sizeof(vector));
	// Could not allocate the memory
	if(vector_init(mm->data, 10) != 0) {
		return -1; /* LCOV_EXCL_LINE */
	}

	mm->first = NULL;
	mm->next = NULL;
	mm->previous = NULL;

	return 0;
}

STATIC int multimap_add(multimap* mm, const char* key, const char* value) {
	char* value_copy = NULL;
	// Don't insert NULL since we simply return NULL
	if(!value) {
		value_copy = malloc(1);
		memset(value_copy, 0, 1);
	} else {
		value_copy = malloc(strlen(value) + 1);
		strcpy(value_copy, value);
	}

	multimap* current = mm->first;
	if(!current) {
		mm->key = key;

		// Add the value to the vector
		if(vector_push_back(mm->data, value_copy) != 0) {
			free(value_copy); /* LCOV_EXCL_LINE */
			return -1; /* LCOV_EXCL_LINE */
		}

		mm->first = mm;
		mm->next = NULL;
		mm->previous = NULL;
	} else {
		// Goto the end of the MultiMap or add this value if the key is found
		while(current) {
			if(strcmp(current->key, key) == 0) {
				// Add the value to the vector
				if(vector_push_back(current->data, value_copy) != 0) {
					free(value_copy); /* LCOV_EXCL_LINE */
					return -1; /* LCOV_EXCL_LINE */
				}

				return 0;
			}

			if(!current->next) break;

			current = current->next;
		}

		// Create a new MultiMap structure and put it at the end
		multimap* new_multimap = malloc(sizeof(multimap));
		// Out of memory
		if(multimap_init(new_multimap) != 0) {
			free(value_copy); /* LCOV_EXCL_LINE */
			return -1; /* LCOV_EXCL_LINE */
		}

		new_multimap->key = key;

		// Add the value to the vector
		if(vector_push_back(new_multimap->data, value_copy) != 0) {
			free(value_copy); /* LCOV_EXCL_LINE */
			return -1; /* LCOV_EXCL_LINE */
		}

		new_multimap->first = current->first;
		new_multimap->next = NULL;
		new_multimap->previous = current;

		current->next = new_multimap;
	}

	return 0;
}

STATIC vector* multimap_get(multimap* mm, const char* key) {
	multimap* current = mm->first;

	while(current) {
		if(strcmp(current->key, key) == 0) {
			return current->data;
		}

		current = current->next;
	}

	return NULL;
}

STATIC unsigned int multimap_get_count(multimap* mm, const char* key) {
	vector* v = multimap_get(mm, key);

	if(v) {
		return v->size;
	}

	return 0;
}

STATIC void multimap_free(multimap* mm) {
	if(mm) {
		multimap* current = mm->first;

		while(current) {
			multimap* next = current->next;

			if(current != mm) {
				vector_free(current->data);
				free(current);
			}

			current = next;
		}

		vector_free(mm->data);
		free(mm);
	}
}

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

STATIC void set_default_renderoptions(renderoptions* options) {
	// Setup defaults
	if(options->placeholder_open == NULL)
		options->placeholder_open = "{{";
	if(options->placeholder_close == NULL)
		options->placeholder_close = "}}";
	if(options->placeholder_cond_separator == NULL)
		options->placeholder_cond_separator = " ";
	if(options->placeholder_cond_open_prefix == NULL)
		options->placeholder_cond_open_prefix = "#";
	if(options->placeholder_cond_close_prefix == NULL)
		options->placeholder_cond_close_prefix = "/";
	if(options->data_list_suffix == NULL)
		options->data_list_suffix = "[]";
	if(options->data_object_suffix == NULL)
		options->data_object_suffix = ".";
}

STATIC char* get_variable(const char* inside_placeholder, renderoptions options) {
	set_default_renderoptions(&options);

	const char* stop_tokens[] = {
		options.placeholder_close,
		options.placeholder_cond_separator,
		options.data_list_suffix,
		options.data_object_suffix,
	};

	// Build the current variable
	char* tmp_startofkey = (char*)inside_placeholder;
	char* tmp_token = NULL;
	char* actual_token = NULL;
	while((tmp_token = (char*)my_strtok(tmp_startofkey, 4, stop_tokens))) {
		const char* my_token = get_last_token(tmp_token, 4, stop_tokens);
		if(!my_token
				|| strcmp(my_token, options.placeholder_cond_separator) == 0
				|| strcmp(my_token, options.placeholder_close) == 0) {
			unsigned int variable_len = (tmp_token + strlen(my_token)) - tmp_startofkey;
			actual_token = malloc(variable_len + 1);
			memset(actual_token, 0, variable_len + 1);
			memcpy(actual_token, tmp_startofkey, variable_len);
			break;
		}

		tmp_startofkey = tmp_startofkey + 1;
	}

	// If the next token wasn't found, the actual token is the entire string!
	if(!actual_token) {
		actual_token = malloc(strlen(inside_placeholder) + 1);
		memset(actual_token, 0, strlen(inside_placeholder) + 1);
		memcpy(actual_token, inside_placeholder, strlen(inside_placeholder));
	}

	/* printf("Actual Token: %s\n", actual_token); */

	return actual_token;
}

char* my_render_template(const char* template_data, unsigned long len, const char* data[], renderoptions options) {
	unsigned long i;

	multimap* mm = malloc(sizeof(multimap));
	multimap_init(mm);
	for(i = 0; i < len; i++) {
		multimap_add(mm, data[i*2], data[i*2+1]);
	}

	// Get the options and set defaults if they aren't set.
	set_default_renderoptions(&options);

	// Create a copy of the template to work with
	char* output = malloc(strlen(template_data) + 1);
	strcpy(output, template_data);

	long start = 0, match_len = 0;
	while(get_surrounded_with(output, options.placeholder_open, options.placeholder_close, &start, &match_len)) {
		char* matched_start = output + start;

		// Get the match, inside the placeholder_open and placeholder_close braces
		char matched_copy[match_len+1];
		memset(matched_copy, 0, match_len + 1);
		strncpy(matched_copy, matched_start, match_len);

		/* int is_condition_sep = 0; */
		int is_condition_pre = 0;
		char* data = NULL;

		// If we have the separator
		char* data_separated = strstr(matched_copy, options.placeholder_cond_separator);
		if(data_separated != NULL) {
			data_separated += strlen(options.placeholder_cond_separator);

			// Copy the separated data to the data
			data = malloc(strlen(data_separated) + 1);
			strcpy(data, data_separated);
		}

		// If we start with a condition
		if(strstr(matched_copy, options.placeholder_cond_open_prefix) == matched_copy) {
			is_condition_pre = 1;

			// 1) Get the closing element {{/data}}
			unsigned long closing_len =
				strlen(options.placeholder_open) +
				strlen(options.placeholder_cond_close_prefix) +
				strlen(matched_copy) + strlen(options.placeholder_cond_open_prefix) +
				strlen(options.placeholder_close);

			char closing_text[closing_len + 1];
			sprintf((char*)closing_text, "%s%s%s%s",
				options.placeholder_open,
				options.placeholder_cond_close_prefix,
				matched_copy + strlen(options.placeholder_cond_open_prefix),
				options.placeholder_close);

			char* closing_instance = strstr(matched_start, closing_text);
			if(closing_instance != NULL) {
				// 2) Get the data the placeholders
				char* data_inside_start = matched_start + strlen(matched_copy) + strlen(options.placeholder_open);
				unsigned long data_inside_len = (unsigned long)closing_instance - (unsigned long)data_inside_start;
				char data_inside[data_inside_len + 1];
				memset(data_inside, 0, data_inside_len + 1);
				strncpy(data_inside, data_inside_start, data_inside_len);

				// 3) Set the data to the replace inner text
				// Append to the existing data
				unsigned long new_data_len = data_inside_len;
				if(data) new_data_len += strlen(data);

				// Concat this data to the current data array in case it already had data from the separator
				char* new_data = malloc(new_data_len + 1);
				memset(new_data, 0, new_data_len + 1);
				if(data) strcpy(new_data, data);
				strcat(new_data, data_inside);
				if(data) free(data);
				data = new_data;

				// 4) Set match_len so we have the whole match
				match_len = ((unsigned long)closing_instance + closing_len) - (unsigned long)matched_start - strlen(options.placeholder_open) - strlen(options.placeholder_close);
			}
		}

		// Reassemble the entire placeholder and replace it
		char toreplace[strlen(options.placeholder_open) + match_len + strlen(options.placeholder_close) + 1];
		sprintf(toreplace, "%s%.*s%s",
			options.placeholder_open,
			(unsigned int)match_len, matched_start,
			options.placeholder_close);

		char* startofkey = matched_copy;
		if(is_condition_pre) {
			startofkey += strlen(options.placeholder_cond_open_prefix);
		}

		char* actual_variable = get_variable(startofkey, options);

		// Do the replacement
		char* replaced = NULL;
		vector* v = multimap_get(mm, actual_variable);
		if(v && v->size) {
			char* value = vector_get(v, 0);

			if(value && strlen(value)) {
				if(data) {
					replaced = str_replace(output, toreplace, data);
				} else {
					replaced = str_replace(output, toreplace, value);
				}
			}
		}

		if(!replaced) {
			replaced = str_replace(output, toreplace, "");
		}

		if(replaced) {
			free(output);
			output = replaced;
		}

		if(data) free(data);
		if(actual_variable) free(actual_variable);
	}

	multimap_free(mm);

	return output;
}

char* render_template(const char* template_data, unsigned long len, const char* data[]) {
	return my_render_template(template_data, len, data, (renderoptions){});
}

char* my_render_template_file(const char* filename, unsigned long len, const char* data[], renderoptions options) {
	char* contents = read_file_contents(filename);
	if(!contents) return NULL;

	char* rendered = my_render_template(contents, len, data, options);
	free(contents);

	return rendered;
}

char* render_template_file(const char* filename, unsigned long len, const char* data[]) {
	return my_render_template_file(filename, len, data, (renderoptions){});
}
