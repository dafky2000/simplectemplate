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

void* vector_get(vector* v, unsigned int index);

int multimap_init(multimap* mm);
int multimap_add(multimap* mm, const char* key, const char* value);
vector* multimap_get(multimap* mm, const char* key);
unsigned int multimap_get_count(multimap* mm, const char* key);
int multimap_free(multimap* mm);

// Basically just an init memory leak test
void spec_multimap1(void)
{
	/* arrange */
	multimap* mm = malloc(sizeof(multimap));
	sp_assert_equal_i(0, multimap_init(mm));

	/* clean */
	multimap_free(mm);
}

void spec_multimap2(void)
{
	/* arrange */
	multimap* mm = malloc(sizeof(multimap));
	sp_assert_equal_i(0, multimap_init(mm));

	/* act */
	sp_assert_equal_i(0, multimap_add(mm, "test", "value1"));
	sp_assert_equal_i(0, multimap_add(mm, "test", "value2"));

	/* assert */
	sp_assert_equal_s((char*)mm->key, "test");

	sp_assert_equal_i(mm->data->size, 2);
	sp_assert_equal_i(mm->data->capacity, 10);

	sp_assert_equal_s(vector_get(mm->data, 0), "value1");
	sp_assert_equal_s(vector_get(mm->data, 1), "value2");

	/* clean */
	multimap_free(mm);
}

void spec_multimap3(void)
{
	/* arrange */
	multimap* mm = malloc(sizeof(multimap));
	sp_assert_equal_i(0, multimap_init(mm));

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
	// Inserting >10 of the previous map to help check for memory violations
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value3"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value4"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value5"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value6"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value7"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value8"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value9"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value10"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value11"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value12"));
	// Done insert >10 of previous
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value7"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value8"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value9"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value10"));
	sp_assert_equal_i(0, multimap_add(mm, "temp123", "value11"));

	// Insert a couple more for fun
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value13 unlucky 13..... just need some extra chars to test for buffer overflows"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value14"));

	/* /1* assert *1/ */
	sp_assert_equal_i(3, multimap_get_count(mm, "test"));
	sp_assert_equal_i(14, multimap_get_count(mm, "temp"));
	sp_assert_equal_i(11, multimap_get_count(mm, "temp123"));

	vector* v1 = multimap_get(mm, "test");
	sp_assert_equal_s(vector_get(v1, 0), "value1");
	sp_assert_equal_s(vector_get(v1, 1), "value2");
	sp_assert_equal_s(vector_get(v1, 2), "value3");

	vector* v2 = multimap_get(mm, "temp");
	sp_assert_equal_s(vector_get(v2, 0), "value1");
	sp_assert_equal_s(vector_get(v2, 1), "value2");
	sp_assert_equal_s(vector_get(v2, 2), "value3");
	sp_assert_equal_s(vector_get(v2, 3), "value4");
	sp_assert_equal_s(vector_get(v2, 4), "value5");
	sp_assert_equal_s(vector_get(v2, 5), "value6");
	sp_assert_equal_s(vector_get(v2, 6), "value7");
	sp_assert_equal_s(vector_get(v2, 7), "value8");
	sp_assert_equal_s(vector_get(v2, 8), "value9");
	sp_assert_equal_s(vector_get(v2, 9), "value10");
	sp_assert_equal_s(vector_get(v2, 10), "value11");
	sp_assert_equal_s(vector_get(v2, 11), "value12");
	sp_assert_equal_s(vector_get(v2, 12), "value13 unlucky 13..... just need some extra chars to test for buffer overflows");
	sp_assert_equal_s(vector_get(v2, 13), "value14");

	vector* v3 = multimap_get(mm, "temp123");
	sp_assert_equal_s(vector_get(v3, 0), "value1");
	sp_assert_equal_s(vector_get(v3, 1), "value2");
	sp_assert_equal_s(vector_get(v3, 2), "value3");
	sp_assert_equal_s(vector_get(v3, 3), "value4");
	sp_assert_equal_s(vector_get(v3, 4), "value5");
	sp_assert_equal_s(vector_get(v3, 5), "value6");
	sp_assert_equal_s(vector_get(v3, 6), "value7");
	sp_assert_equal_s(vector_get(v3, 7), "value8");
	sp_assert_equal_s(vector_get(v3, 8), "value9");
	sp_assert_equal_s(vector_get(v3, 9), "value10");
	sp_assert_equal_s(vector_get(v3, 10), "value11");

	// Index out of bounds returns null
	sp_assert_equal_i((long)multimap_get(mm, "notfound"), (long)NULL);
	sp_assert_equal_i(multimap_get_count(mm, "notfound"), 0);

	/* clean */
	multimap_free(mm);
}

void spec_multimap4(void)
{
	/* arrange */
	multimap* mm = malloc(sizeof(multimap));
	sp_assert_equal_i(0, multimap_init(mm));

	/* act */
	sp_assert_equal_i(0, multimap_add(mm, "temp", "value1"));
	sp_assert_equal_i(0, multimap_add(mm, "temp", NULL));

	vector* v1 = multimap_get(mm, "temp");
	sp_assert_equal_s(vector_get(v1, 0), "value1");
	sp_assert_equal_s(vector_get(v1, 1), "");

	/* clean */
	multimap_free(mm);
}
