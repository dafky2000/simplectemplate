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
	vector* v = malloc(sizeof(vector));
	sp_assert_equal_i(0, vector_init(v, 10));
	sp_assert_equal_i(0, v->size);

	sp_assert_equal_i(0, vector_push_back(v, (char*)"test"));
	sp_assert_equal_i(1, v->size);

	sp_assert_equal_i(0, vector_push_back(v, (char*)"test2"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test3"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test4"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test5"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test6"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test7"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test8"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test9"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test10"));
	sp_assert_equal_i(10, v->size);

	sp_assert_equal_i(0, vector_push_back(v, (char*)"test11"));
	sp_assert_equal_i(11, v->size);

	vector_free(v);
}

void spec_vector2(void)
{
	vector* v = malloc(sizeof(vector));
	sp_assert_equal_i(0, vector_init(v, 10));

	sp_assert_equal_i(0, vector_push_back(v, (char*)"test01"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test02"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test03"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test04"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test05"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test06"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test07"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test08"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test09"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test10"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test11"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test12"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test13"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test14"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test15"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test16"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test17"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test18"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test19"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test20"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test21"));
	sp_assert_equal_i(21, v->size);

	unsigned int i = 0;
	for(; i < v->size; ++i) {
		char cmpto[7];
		memset(cmpto, 0, 7);
		sprintf(cmpto, "test%02d", i+1);

		char* cur = *(v->data) + i * sizeof(void*);
		sp_assert_equal_s(cmpto, cur);
	}

	vector_free(v);
}

void spec_vector3(void)
{
	vector* v = malloc(sizeof(vector));
	sp_assert_equal_i(0, vector_init(v, 10));
	sp_assert_equal_i(0, v->size);

	vector* v2 = malloc(sizeof(vector));
	sp_assert_equal_i(0, vector_init(v2, 10));
	sp_assert_equal_i(0, v2->size);

	sp_assert_equal_i(0, vector_push_back(v, (char*)"test"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test2"));
	sp_assert_equal_i(0, vector_push_back(v, (char*)"test3"));

	sp_assert_equal_i(0, vector_push_back(v2, (char*)"test4"));
	sp_assert_equal_i(0, vector_push_back(v2, (char*)"test5"));

	sp_assert_equal_i(3, v->size);
	sp_assert_equal_i(2, v2->size);

	sp_assert_equal_s(vector_get(v, 2), "test3");
	sp_assert_equal_s(vector_get(v2, 1), "test5");

	// Index out of bounds returns null
	sp_assert_equal_i((long)vector_get(v2, 3), (long)NULL);

	vector_free(v);
	vector_free(v2);
}
