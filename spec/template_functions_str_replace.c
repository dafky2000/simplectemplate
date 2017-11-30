#include "../speck/speck.h"

char* str_replace(char* orig, const char* rep, const char* with);

void spec_str_replace1(void)
{
	/* arrange */
	char* contents = "testing Test test TEST 123 12 T";

	/* act */
	char* replaced = str_replace(contents, "test", "zzz");

	/* assert */
	sp_assert_equal_s(replaced, "zzzing Test zzz TEST 123 12 T");

	if(replaced) free(replaced);
}

void spec_str_replace2(void)
{
	/* arrange */
	char* contents = "";

	/* act */
	char* replaced = str_replace(contents, "test", "zzz");

	/* assert */
	sp_assert_equal_s(replaced, "");

	if(replaced) free(replaced);
}
