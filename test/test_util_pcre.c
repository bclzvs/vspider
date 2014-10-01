#include "../include/unit_test.h"
#include "../include/util_pcre.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

START_TEST(test_pcre_getMatchVal)
{
	char *input = "<div class=\042must-yj\042>\r\n</div><ul></ul><div>other</div>"; // \042 is "
	char *pattern = "class=\"must-yj\"[\\s\\S]*?</div>";
	// or char *pattern = "class=\"must-yj\"[[:print:][:space:]]*?</div>";
	// or char *pattern = "class=\"must-yj\".*?</div>";
	char *value = pcre_getMatchVal(input, pattern, strlen(input));
	ck_assert_str_eq("class=\"must-yj\">\r\n</div>", value);
	free(value);
}
END_TEST

START_TEST(test_pcre_getMatchCapture_name_1)
{
	char *input = "<div class=\042must-yj\042>\r\n</div><ul></ul><div>other</div>"; // \042 is "
	char *pattern = "class=\"(?<class>must-yj)\"[\\s\\S]*?</div>";
	pcre_capture_t *pcapture = pcre_getMatchCapture(input, pattern, strlen(input));
	ck_assert_str_eq("class=\042must-yj\042>\r\n</div>", pcapture->value);
	pcapture = pcapture->next;
	ck_assert_str_eq("class", pcapture->name);
	ck_assert_str_eq("must-yj", pcapture->value);
}
END_TEST

START_TEST(test_pcre_getMatchCapture_name_2)
{
	char *input = "<a href='test'>test page</a>"; 
	char *pattern = "href='(?<href>[^']*)'>(?<name>[^<]*)</a>";
	pcre_capture_t *pcapture = pcre_getMatchCapture(input, pattern, strlen(input));
	pcapture = pcapture->next;	// skip 0 capture;
	ck_assert_str_eq("href", pcapture->name);
	ck_assert_str_eq("test", pcapture->value);
	pcapture = pcapture->next;
	ck_assert_str_eq("name", pcapture->name);
	ck_assert_str_eq("test page", pcapture->value);
}
END_TEST

Suite *make_util_pcre_suite()
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("util_pcre");
	tc_core = tcase_create("util_pcre");
	tcase_add_test(tc_core, test_pcre_getMatchVal);
	tcase_add_test(tc_core, test_pcre_getMatchCapture_name_1);
	tcase_add_test(tc_core, test_pcre_getMatchCapture_name_2);
	//tcase_add_test(tc_core, test_pcre_getListMatch);

	suite_add_tcase(s, tc_core);
	return s;
}
