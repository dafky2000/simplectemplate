#include "../speck/speck.h"
#include "../src/template_functions.h"

// Tests are all covered by template_functions:render_template_file2
// Leaving as stub

void spec_render_template_file1(void)
{
	/* arrange */
	const char* keys[] = { "ph1", "ph2", };
	const char* values[] = {
		"awesome",
		" is running",
	};
	const char* filename = "spec/templates/basic.txt";

	/* act */
	char* rendered = render_template_file(filename, 2, keys, values);

	/* assert */
	/* sp_assert_equal_s(rendered, "My awesome test is running"); */

	if(rendered) free(rendered);
}

void spec_render_template_file2(void)
{
	/* arrange */
	const char* keys[] = { "ph1", "ph2" };
	const char* values[] = { "data1", "data2", };
	const char* filename = "spec/templates/non-existent.txt";

	/* act */
	char* rendered = render_template_file(filename, 3, keys, values);

	/* assert */
	sp_assert(!rendered);

	if(rendered) free(rendered);
}
