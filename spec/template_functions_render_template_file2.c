#include "../speck/speck.h"
#include "../src/template_functions.h"

// Tests are all covered by template_functions:render_template_file2
// Leaving as stub

void spec_render_template_file21(void)
{
  /* arrange */
  const char* data[] = {
    "ph1", "awesome",
    "ph2", " is running",
  };
  const char* filename = "spec/templates/basic.txt";

  /* act */
  char* rendered = render_template_file2(filename, 2, data);

  /* assert */
  sp_assert(strcmp(rendered, "My awesome test is running") == 0);

  if(rendered) free(rendered);
}

void spec_render_template_file22(void)
{
  /* arrange */
  const char* data[] = {
    "ph1", "awesome",
    "ph2", " is running",
  };
  const char* filename = "spec/templates/non-existent.txt";

  /* act */
  char* rendered = render_template_file2(filename, 2, data);

  /* assert */
  sp_assert(!rendered);

  if(rendered) free(rendered);
}
