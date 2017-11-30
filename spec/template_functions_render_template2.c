#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_render_template21(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
	};
	const char* template = "My {{data.ph1}} test";

	/* act */
	char* rendered = render_template2(template, 1, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test");

	free(rendered);
}

void spec_render_template22(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{data.ph1}} test{{data.ph2}}";

	/* act */
	char* rendered = render_template2(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running");

	free(rendered);
}

void spec_render_template23(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{data.ph1}} test{{data.ph2}}{{data.emptyph}}";

	/* act */
	char* rendered = render_template2(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running{{data.emptyph}}");

	free(rendered);
}

void spec_render_template24(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{data.ph1}} test{{data.ph2}}{{data.emptyph}}";

	/* act */
	char* rendered = render_template2(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running{{data.emptyph}}");

	free(rendered);
}
