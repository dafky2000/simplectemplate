#include <speck.h>
#include <template_functions.h>

void spec_exists(void)
{
    /* arrange */
    const char *filename = "index.html";
    int result = 0;

    /* act */
    result = exists(filename);

    /* assert */
    sp_assert_equal_i(-1, result);
}
