#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_render_template1(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
	};
	const char* template = "My {{data.ph1}} test";

	/* act */
	char* rendered = render_template(template, 1, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test");

	free(rendered);
}

void spec_render_template2(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{data.ph1}} test{{data.ph2}}";

	/* act */
	char* rendered = render_template(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running");

	free(rendered);
}

void spec_render_template3(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{data.ph1}} test{{data.ph2}}{{data.emptyph}}";

	/* act */
	char* rendered = render_template(template, 2, data);

	/* assert */
	printf("Actual: %s\n", rendered);
	sp_assert_equal_s(rendered, "My awesome test is running");

	free(rendered);
}

void spec_render_template4(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{data.ph1}} test{{data.ph2}}{{data.emptyph}}";

	/* act */
	char* rendered = render_template(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running");

	free(rendered);
}
