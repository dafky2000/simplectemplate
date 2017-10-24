#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_read_file_contents1(void)
{
  /* arrange */
  const char* filename = "spec/templates/basic.txt";

  /* act */
  char* contents = read_file_contents(filename);

  /* assert */
  sp_assert_equal_s(contents, "My {{data.ph1}} test{{data.ph2}}");

  if(contents) free(contents);
}

void spec_read_file_contents2(void)
{
  /* arrange */
  const char* filename = "spec/templates/basic_zyx.txt";

  /* act */
  char* contents = read_file_contents(filename);

  /* assert */
  sp_assert(!contents);

  if(contents) free(contents);
}

void spec_read_file_contents3(void)
{
  /* arrange */
  const char* filename = "spec/templates/empty.txt";

  /* act */
  char* contents = read_file_contents(filename);

  /* assert */
  sp_assert(contents != NULL);

  if(contents) free(contents);
}
