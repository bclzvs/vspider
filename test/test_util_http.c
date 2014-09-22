#include "../include/util_http.h"
#include <check.h>
static char * headbuf = "HTTP/1.1 200 OK\r\nTransfer-Encoding: chuncked\r\n";
START_TEST(test_getheader)
{
	ck_assert_str_eq("chuncked", http_getHeaderVal(headbuf, "Transfer-Encoding"));
}
END_TEST

START_TEST(test_getheader_null)
{
	fail_unless(http_getHeaderVal(headbuf,"Nokey") == NULL, "ok");
}
END_TEST

START_TEST(test_getstatus)
{
	ck_assert_int_eq(200, http_getStatus(headbuf));
}
END_TEST

Suite *make_util_http_suite()
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("util_http");
	tc_core = tcase_create("util_http");
	tcase_add_test(tc_core,test_getheader);
	tcase_add_test(tc_core,test_getheader_null);
	tcase_add_test(tc_core,test_getstatus);

	suite_add_tcase(s, tc_core);
	return s;
}

