#include "../speck/speck.h"
// Any other includes you need

void spec_example(void)
{
  /* arrange */
  char* filename = malloc(10 + 1);
  memset(filename, 0, 11);

  /* act */
  strcat(filename, "index");
  strcat(filename, ".html");

  /* assert */
  sp_assert_equal_s(filename, "index.html");

  free(filename);
}
