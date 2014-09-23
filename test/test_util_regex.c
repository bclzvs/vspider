#include "../include/unit_test.h"
#include "../include/util_regex.h"

START_TEST(test_regex_getMatchVal)
{
	char *input = "<div class=\"must-yj\">";
	char *value = regex_getMatchValue(input, "class=\"must-yj\"");
	ck_assert_str_eq("class=\"must-yj\"", value);
}
END_TEST

Suite *make_util_regex_suite()
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("util_regex");
	tc_core = tcase_create("util_regex");
	tcase_add_test(tc_core, test_regex_getMatchVal);

	suite_add_tcase(s, tc_core);
	return s;
}
