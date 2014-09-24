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
	for(;;){
		regmatch_t *match = regex_getMatches(input, pattern);
		if(match == NULL) break;
		int	i = 0;
		for(; i < count; i++){
			if(match[i].rm_so <= 0) break;
			char *value = regex_getValue(input, match + i);
			printf("match[%d]:%s\n", i, value);
	//		free(value);
		}
		input += match->rm_eo;
	}
//	free(match);		
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

	suite_add_tcase(s, tc_core);
	return s;
}
