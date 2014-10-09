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

START_TEST(test_pcre_getMatchCapture_noname_2)
{
	char *input = "<a href='test'>test page</a>"; 
	char *pattern = "href='([^']*)'>([^<]*)</a>";
	pcre_capture_t *pcapture = pcre_getMatchCapture(input, pattern, strlen(input));
	pcapture = pcapture->next;	// skip 0 capture;
	ck_assert(pcapture != NULL);
	ck_assert_str_eq("test", pcapture->value);
	pcapture = pcapture->next;
	ck_assert_str_eq("test page", pcapture->value);
	free_pcre_capture(pcapture);
}
END_TEST

START_TEST(test_pcre_getMatches)
{
	char *input = "<li><a href='test1'>test1 page</a></li><li><a href='test2'>test2 page</a></li><li><a href='test3'>test3 page</a></li>"; 
	char *pattern = "href='(?<href>[^']*)'>(?<name>[^<]*)</a>";
	pcre_match_t *match = pcre_getMatches(input, pattern, strlen(input));
	ck_assert_int_eq(1, match->success);
	int	i = 1;
	char	expect[20];
	for(; match != NULL; match = match->next, i++){
		pcre_capture_t *pcapture = match->pcapture;
		pcapture = pcapture->next;	// skip 0 capture;
		ck_assert_str_eq("href", pcapture->name);
		snprintf(expect, 20, "test%c", i + '0');
		ck_assert_str_eq(expect, pcapture->value);
		pcapture = pcapture->next;
		ck_assert_str_eq("name", pcapture->name);
		snprintf(expect, 20, "test%c page", i + '0');
		ck_assert_str_eq(expect, pcapture->value);
	}
	free_pcre_match(match);
}
END_TEST

START_TEST(test_pcre_capture_findByName)
{
	char *input = "<a href='test'>test page</a>"; 
	char *pattern = "href='(?<href>[^']*)'>(?<name>[^<]*)</a>";
	pcre_capture_t *pcapture = pcre_getMatchCapture(input, pattern, strlen(input));
	ck_assert(pcapture != NULL);
	pcre_capture_t *finded = pcre_capture_findByName(pcapture, "name");
	ck_assert(finded != NULL);
	ck_assert_str_eq("name",finded ->name);
	ck_assert_str_eq("test page",finded ->value);
	finded = pcre_capture_findByName(pcapture, "noname");
	ck_assert(finded == NULL);
}
END_TEST

START_TEST(test_pcre_capture_findByIndex)
{
	char *input = "<a href='test'>test page</a>"; 
	char *pattern = "href='(?<href>[^']*)'>(?<name>[^<]*)</a>";
	pcre_capture_t *pcapture = pcre_getMatchCapture(input, pattern, strlen(input));
	ck_assert(pcapture != NULL);
	pcre_capture_t *finded = pcre_capture_findByIndex(pcapture,2);
	ck_assert(finded != NULL);
	ck_assert_str_eq("name",finded ->name);
	ck_assert_str_eq("test page",finded ->value);
	finded = pcre_capture_findByIndex(pcapture,3);
	ck_assert(finded == NULL);
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
	tcase_add_test(tc_core, test_pcre_getMatchCapture_noname_2);
	tcase_add_test(tc_core, test_pcre_getMatches);
	tcase_add_test(tc_core, test_pcre_capture_findByName);
	tcase_add_test(tc_core, test_pcre_capture_findByIndex);


	suite_add_tcase(s, tc_core);
	return s;
}
