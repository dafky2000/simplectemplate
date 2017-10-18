#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_exists1(void)
{
  /* arrange */
  const char* filename = "src/template_functions.h";
  int result = 0;

  /* act */
  result = exists(filename);

  /* assert */
  sp_assert_equal_i(0, result);
}

void spec_exists2(void)
{
  /* arrange */
  const char* filename = "invalid_file.c";
  int result = 0;

  /* act */
  result = exists(filename);

  /* assert */
  sp_assert_equal_i(-1, result);
}
