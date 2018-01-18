#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_render_template1(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
	};
	const char* template = "My {{ph1}} test";

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
	const char* template = "My {{ph1}} test{{ph2}}123\n\n";

	/* act */
	char* rendered = render_template(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running123\n\n");

	free(rendered);
}

void spec_render_template3(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* template = "My {{ph1}} test{{ph2}}{{emptyph}}";

	/* act */
	char* rendered = render_template(template, 2, data);

	/* assert */
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
	const char* template = "My {{ph1}} test{{ph2}}{{emptyph}}";

	/* act */
	char* rendered = render_template(template, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running");

	free(rendered);
}

void spec_render_template5(void)
{
	/* arrange */
	const char *data[] = {
		"ph1", "awesome",
		"ph2", " is running",
		"varwcondition", " ",
		"separator", "",
	};
	const char* template = "My {{ph1}} test{{ph2}}{{emptyph}} {{test123 testing 1 2 ummm 12}}{{separator bob loblaw}}{{#varwcondition}}Some datas{{/varwcondition}} abc123 test test test";

	/* act */
	char* rendered = render_template(template, 4, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running Some datas abc123 test test test");

	free(rendered);
}

void spec_render_template6(void)
{
	/* arrange */
	const char *data[] = {
		"noclosingbrace", "",
		"open", "true",
		"inner", " ---- ",
		"inneropen", " ",
	};
	const char* template = "Just a test for {{#noclosingbrace}}, lets see {{#open}}blah blah blah {{inner}} blah blah blah {{#inneropen}} bob loblaw {{/inneropen}}{{/open}}what {{#notpresent}} dsadas {{/notpresent}}happens!";

	/* act */
	char* rendered = render_template(template, 4, data);

	/* assert */
	sp_assert_equal_s(rendered, "Just a test for , lets see blah blah blah  ----  blah blah blah  bob loblaw what happens!");

	free(rendered);
}

void spec_render_template7(void)
{
	/* arrange */
	const char *data[] = {
		"noclosingbrace", "",
		"open", "true",
		"inner", " ---- ",
		"inneropen", NULL,
	};
	const char* template = "Just a test for {{#noclosingbrace}}, lets see {{#open}}blah blah blah {{inner}} blah blah blah {{#inneropen}} bob loblaw {{/inneropen}}{{/open}}what happens!";

	/* act */
	char* rendered = render_template(template, 4, data);

	/* assert */
	sp_assert_equal_s(rendered, "Just a test for , lets see blah blah blah  ----  blah blah blah what happens!");

	free(rendered);
}

// Basic section loop test
void spec_render_template8(void)
{
	/* arrange */
	const char *data[] = {
		"loop[]", "true",
		"loop[].data", "test1",
		"loop[].data", "test2",
		"loop[].data", "test3",
	};
	const char* template = "Looping: {{#loop}}Cur val: {{data}}\n{{/loop}}";

	/* printf("Actual value: '%s'", template); */

	/* act */
	char* rendered = render_template(template, 4, data);

	/* assert */
	/* sp_assert_equal_s(rendered, "Looping: Cur val: test1\nCur val: test2\nCur val: test3\n"); */

	free(rendered);
}
