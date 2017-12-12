#include "../speck/speck.h"
#include "../src/template_functions.h"

void spec_render_template_file1(void)
{
	/* arrange */
	const char* data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* filename = "spec/templates/basic.txt";

	/* act */
	char* rendered = render_template_file(filename, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "My awesome test is running");

	if(rendered) free(rendered);
}

void spec_render_template_file2(void)
{
	/* arrange */
	const char* data[] = {
		"ph1", "awesome",
		"ph2", " is running",
	};
	const char* filename = "spec/templates/non-existent.txt";

	/* act */
	char* rendered = render_template_file(filename, 2, data);

	/* assert */
	sp_assert(!rendered);

	if(rendered) free(rendered);
}

void spec_render_template_file3(void)
{
	/* arrange */
	const char* data[] = {
		"title", "title",
		"body", "test body",
	};
	const char* filename = "spec/templates/default.html";

	/* act */
	char* rendered = render_template_file(filename, 2, data);

	/* assert */
	sp_assert_equal_s(rendered, "<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<meta charset=\"UTF-8\">\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n\
		<title></title>\n\
	</head>\n\
	<body>\n\
		<h1></h1>\n\
		\n\
		<p>\n\
			Powered by <a href=\"https://github.com/dafky2000/simplectemplate\">simplectemplate</a>\n\
		</p>\n\
	</body>\n\
</html>");

	if(rendered) free(rendered);
}

void spec_render_template_file4(void)
{
	/* arrange */
	const char* data[] = {
		"title", "fancy title",
		"body", "test body",
	};
	const char* filename = "spec/templates/default.html";

	/* act */
	char* rendered = my_render_template_file(filename, 2, data,
		(struct RenderOptions){
			.placeholder_open="{{",
			.placeholder_close="}}",
			.data_open="data.",
		});

	/* assert */
	sp_assert_equal_s(rendered, "<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<meta charset=\"UTF-8\">\n\
		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n\
		<title>fancy title</title>\n\
	</head>\n\
	<body>\n\
		<h1>fancy title</h1>\n\
		test body\n\
		<p>\n\
			Powered by <a href=\"https://github.com/dafky2000/simplectemplate\">simplectemplate</a>\n\
		</p>\n\
	</body>\n\
</html>");

	if(rendered) free(rendered);
}
