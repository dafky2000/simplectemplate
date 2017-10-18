#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_str_replace1(void)
{
  /* arrange */
  char* contents = "testing Test test TEST 123 12 T";

  /* act */
  char* replaced = str_replace(contents, "test", "zzz");

  /* assert */
  sp_assert(strcmp(replaced, "zzzing Test zzz TEST 123 12 T") == 0);

  if(replaced) free(replaced);
}

void spec_str_replace2(void)
{
  /* arrange */
  char* contents = "";

  /* act */
  char* replaced = str_replace(contents, "test", "zzz");

  /* assert */
  sp_assert(strcmp(replaced, "") == 0);

  if(replaced) free(replaced);
}
