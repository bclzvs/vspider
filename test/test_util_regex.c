#include "../include/unit_test.h"
#include "../include/util_regex.h"
#include <stdlib.h>
#include <stdio.h>
/* patter \r\n */
START_TEST(test_regex_getMatchVal_rn)
{
	char *input = "<div class=\042must-yj\042>\r\n</div><ul></ul>"; // \042 is "
	char *pattern = "class=\"must-yj\"[[:print:]\r\n]*?</div>";
	char *value = regex_getMatchValue(input, pattern);
	ck_assert_str_eq("class=\"must-yj\">\r\n</div>", value);
}
END_TEST

START_TEST(test_regex_getMatches)
{
	char *input = 
"<div>\r\n\
<li><a href='1.html'>1</a></li>\r\n\
<li><a href='2.html'>2</a></li>\r\n\
</div>";
	char *pattern = "<a href='([^']+)'>([^<]+)</a>";
	int	count = 9;
	regmatch_t *match = regex_getMatches(input, pattern);
	int	i = 0;
	for(; i < count; i++){
		if(match[i].rm_so <= 0) break;
	}
	ck_assert_int_eq(3, i);
	ck_assert_str_eq("1.html", regex_getValue(input, match+1));
	ck_assert_str_eq("1", regex_getValue(input, match+2));

}
END_TEST

START_TEST(test_regex_getListMatch)
{
	char *input = 
"<div>\r\n\
<li><a href='1.html'>1</a></li>\r\n\
<li><a href='2.html'>2</a></li>\r\n\
</div>";
	char *pattern = "<a href='([^']+)'>([^<]+)</a>";
	int	count;
	list_rmatch_t *lmatch = regex_getListMatch(input, pattern, &count);
	ck_assert_int_eq(2, count);	

	regmatch_t *pmatch = lmatch->pmatch;
	char	*tmp;
	tmp = regex_getValue(input, pmatch+1);
	ck_assert_str_eq("1.html", tmp);
	free(tmp);

	tmp =regex_getValue(input, pmatch+2);
	ck_assert_str_eq("1", tmp);
	free(tmp);
	
	lmatch = lmatch->next;
	pmatch = lmatch->pmatch;	
	tmp = regex_getValue(input, pmatch+1);
	ck_assert_str_eq("2.html", tmp);
	free(tmp);

	tmp =regex_getValue(input, pmatch+2);
	ck_assert_str_eq("2", tmp);
	free(tmp);
}
END_TEST

Suite *make_util_regex_suite()
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("util_regex");
	tc_core = tcase_create("util_regex");
	tcase_add_test(tc_core, test_regex_getMatchVal_rn);
	tcase_add_test(tc_core, test_regex_getMatches);
	tcase_add_test(tc_core, test_regex_getListMatch);

	suite_add_tcase(s, tc_core);
	return s;
}
