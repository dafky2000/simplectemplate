#include "../speck/speck.h"

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

int multimap_init(multimap* mm);
int multimap_add(multimap* mm, const char* key, const char* value);
unsigned int multimap_get_count(multimap* mm, const char* key);
int multimap_free(multimap* mm);

void spec_multimap1(void)
{
	/* arrange */
	multimap* mm = malloc(sizeof(multimap));
	multimap_init(mm);

	/* act */
	multimap_add(mm, "test", "value1");
	multimap_add(mm, "test", "value2");

	/* assert */
	sp_assert_equal_s((char*)mm->key, "test");

	sp_assert_equal_i(mm->data->size, 2);
	sp_assert_equal_i(mm->data->capacity, 10);

	sp_assert_equal_s(*(mm->data->data) + 0 * sizeof(void*), "value1");
	sp_assert_equal_s(*(mm->data->data) + 1 * sizeof(void*), "value2");

	/* clean */
	multimap_free(mm);
}

void spec_multimap2(void)
{
	/* arrange */
	multimap* mm = malloc(sizeof(multimap));
	multimap_init(mm);

	/* act */
	sp_assert_equal_i(0, multimap_add(mm, "test", "value1"));
	sp_assert_equal_i(0, multimap_add(mm, "test", "value2"));
	sp_assert_equal_i(0, multimap_add(mm, "test", "value3"));

	sp_assert_equal_i(0, multimap_add(mm, "temp", "value1"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value2"));

	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value1"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value2"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value3"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value4"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value5"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value6"));
	// SWITCH
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value3"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value4"));
	// Ok just messing around
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value7"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value8"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value9"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value10"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value11"));
	// SWITCH
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value5"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value6"));
	// Ok just messing around again..

	/* assert */
	sp_assert_equal_i(3, multimap_get_count(mm, "test"));
	sp_assert_equal_i(6, multimap_get_count(mm, "temp"));
	sp_assert_equal_i(11, multimap_get_count(mm, "temp123"));

	sp_assert_equal_s(*(mm->data->data) + 0 * sizeof(void*), "value1");
	sp_assert_equal_s(*(mm->data->data) + 1 * sizeof(void*), "value2");
	sp_assert_equal_s(*(mm->data->data) + 2 * sizeof(void*), "value3");

	sp_assert_equal_s(*(mm->next->data->data) + 0 * sizeof(void*), "value1");
	sp_assert_equal_s(*(mm->next->data->data) + 1 * sizeof(void*), "value2");
	sp_assert_equal_s(*(mm->next->data->data) + 2 * sizeof(void*), "value3");
	sp_assert_equal_s(*(mm->next->data->data) + 3 * sizeof(void*), "value4");
	sp_assert_equal_s(*(mm->next->data->data) + 4 * sizeof(void*), "value5");
	sp_assert_equal_s(*(mm->next->data->data) + 5 * sizeof(void*), "value6");

	sp_assert_equal_s(*(mm->next->next->data->data) + 0 * sizeof(void*), "value1");
	sp_assert_equal_s(*(mm->next->next->data->data) + 1 * sizeof(void*), "value2");
	sp_assert_equal_s(*(mm->next->next->data->data) + 2 * sizeof(void*), "value3");
	sp_assert_equal_s(*(mm->next->next->data->data) + 3 * sizeof(void*), "value4");
	sp_assert_equal_s(*(mm->next->next->data->data) + 4 * sizeof(void*), "value5");
	sp_assert_equal_s(*(mm->next->next->data->data) + 5 * sizeof(void*), "value6");
	sp_assert_equal_s(*(mm->next->next->data->data) + 6 * sizeof(void*), "value7");
	sp_assert_equal_s(*(mm->next->next->data->data) + 7 * sizeof(void*), "value8");
	sp_assert_equal_s(*(mm->next->next->data->data) + 8 * sizeof(void*), "value9");
	sp_assert_equal_s(*(mm->next->next->data->data) + 9 * sizeof(void*), "value10");
	sp_assert_equal_s(*(mm->next->next->data->data) + 10 * sizeof(void*), "value11");

	/* clean */
	multimap_free(mm);
}
