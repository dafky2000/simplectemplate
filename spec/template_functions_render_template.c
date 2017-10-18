#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_render_template1(void)
{
  /* arrange */
  const char *keys[] = { "ph1", };
  const char *values[] = {
    "awesome",
  };
  const char* template = "My {{data.ph1}} test";

  /* act */
  char* rendered = render_template(template, 1, keys, values);

  /* assert */
  sp_assert(strcmp(rendered, "My awesome test") == 0);

  free(rendered);
}

void spec_render_template2(void)
{
  /* arrange */
  const char *keys[] = { "ph1", "ph2", };
  const char *values[] = {
    "awesome",
    " is running",
  };
  const char* template = "My {{data.ph1}} test{{data.ph2}}";

  /* act */
  char* rendered = render_template(template, 2, keys, values);

  /* assert */
  sp_assert(strcmp(rendered, "My awesome test is running") == 0);

  free(rendered);
}

void spec_render_template3(void)
{
  /* arrange */
  const char *keys[] = { "ph1", "ph2", };
  const char *values[] = {
    "awesome",
    " is running",
  };
  const char* template = "My {{data.ph1}} test{{data.ph2}}{{data.emptyph}}";

  /* act */
  char* rendered = render_template(template, 2, keys, values);

  /* assert */
  sp_assert(strcmp(rendered, "My awesome test is running{{data.emptyph}}") == 0);

  free(rendered);
}

void spec_render_template4(void)
{
  /* arrange */
  const char *keys[] = { "ph1", "ph2", };
  const char *values[] = {
    "awesome",
    " is running",
  };
  const char* template = "My {{data.ph1}} test{{data.ph2}}{{data.emptyph}}";

  /* act */
  char* rendered = render_template(template, 2, keys, values);

  /* assert */
  sp_assert(strcmp(rendered, "My awesome test is running{{data.emptyph}}") == 0);

  free(rendered);
}
