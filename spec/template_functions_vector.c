#include "../speck/speck.h"

typedef struct Vector {
	void** data;
	size_t capacity; /* total capacity */
	size_t size; /* number of elements in vector */
} vector;

int vector_init(vector* v, size_t init_capacity);
int vector_grow(vector* v, size_t add_capacity);
int vector_push_back(vector* v, void* element);
void* vector_get(vector* v, unsigned int index);
void vector_free(vector* v);

void spec_vector1(void)
{
	unsigned int capacity = 10;
	unsigned int size = capacity * 30 + 1;

	vector* v = malloc(sizeof(vector));
	sp_assert_equal_i(0, vector_init(v, 10));
	sp_assert_equal_i(0, v->size);

	const char* static_text = "text and more text so we can check for buffer overflow ";
	unsigned int text_len = strlen(static_text);

	unsigned int i = 0;
	for(i = 0; i < size; ++i) {
		char* text = malloc(text_len + 3 + 1);
		sprintf(text, "%s%03d", static_text, i);
		sp_assert_equal_i(0, vector_push_back(v, text));
	}

	sp_assert_equal_i(v->size, size);
	sp_assert_equal_i(v->capacity, size/10*10+10);

	for(i = 0; i < v->size; ++i) {
		char* text = malloc(text_len + 3 + 1);
		sprintf(text, "%s%03d", static_text, i);

		sp_assert_equal_s(vector_get(v, i), text);

		free(text);
	}

	vector_free(v);
}

void spec_my_test_function(void)
{
	/* unsigned int capacity = 3; */

	/* void** container = malloc(sizeof(void*)); */
	/* void** elems = malloc(capacity * sizeof(void*)); */
	/* memset(elems, 0, capacity * sizeof(void*)); */

	/* *container = elems; */

	/* void* new_elems = realloc(elems, (capacity + 1) * sizeof(void*)); */
	/* *container = new_elems; */
	/* capacity++; */

	/* elems = *container; */

	/* const char* static_text = "text and more text so we can check for buffer overflow "; */
	/* unsigned int text_len = strlen(static_text); */
	/* void* next_loc = NULL; */
	/* unsigned int i = 0; */
	/* for(i = 0; i < capacity; ++i) { */
	/* 	next_loc = &elems[i]; */

	/* 	/1* printf("========================\n"); *1/ */
	/* 	/1* printf("=== CURRENT INDEX %02d ===\n", i); *1/ */
	/* 	/1* printf("========================\n"); *1/ */
	/* 	/1* printf("container = %p\n", container); *1/ */
	/* 	/1* printf("*container = %p\n", *container); *1/ */
	/* 	/1* printf("elems = %p\n", elems); *1/ */
	/* 	/1* printf("next_loc = %p\n", next_loc); *1/ */
	/* 	/1* printf("*next_loc = %lu\n", *next_loc); *1/ */
	/* 	/1* printf("elems[%d] = %lu\n", i, elems[i]); *1/ */

	/* 	char* text = malloc(text_len + 2 + 1); */
	/* 	memset(text, 0, text_len + 2 + 1); */
	/* 	sprintf(text, "%s%02d", static_text, i); */
	/* 	printf("text = %p\n", text); */
	/* 	printf("next_loc = %p\n", next_loc); */
	/* 	printf("elems[%d] = %p\n", i, &elems[i]); */
	/* 	elems[i] = text; */

	/* 	printf("=======================\n"); */
	/* 	/1* printf("text: %s\n", text); *1/ */
	/* 	printf("next_loc = %p\n", next_loc); */
	/* 	printf("elems[%d] = %p\n", i, &elems[i]); */
	/* } */

	/* printf("=======================\n"); */
	/* printf("=======================\n"); */
	/* printf("=======================\n"); */
	/* for(i = 0; i < capacity; ++i) { */
	/* 	next_loc = elems[i]; */
	/* 	printf("text: %s\n", (char*)next_loc); */
	/* 	free(next_loc); */
	/* } */

	/* free(*container); */
	/* free(container); */
	/* /1* free(elems); *1/ */
	/* /1* free(text1); *1/ */
}
