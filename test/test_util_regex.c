#include "../include/unit_test.h"
#include "../include/util_regex.h"
#include <stdio.h>
/* patter \r\n */
START_TEST(test_regex_getMatchVal_rn)
{
	char *input = "<div class=\042must-yj\042>\r\n</div><ul></ul>"; // \042 is "
	char *pattern = "class=\"must-yj\"[[:print:]\r\n]*?</div>";
	char *value = regex_getMatchValue(input, pattern);
	ck_assert_str_eq("class=\"must-yj\">\r\n</div>", value);
	//ck_assert_str_eq("",value);
	//fail_unless(value =="");
}
END_TEST

Suite *make_util_regex_suite()
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("util_regex");
	tc_core = tcase_create("util_regex");
	tcase_add_test(tc_core, test_regex_getMatchVal_rn);

	suite_add_tcase(s, tc_core);
	return s;
}
