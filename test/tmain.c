#include <check.h>
#include "../include/unit_test.h"
#include <stdlib.h>

int main(int argc, const char **argv)
{
	Suite *s;
	SRunner *sr;
	s =make_util_http_suite();
	sr = srunner_create(s);
	srunner_set_fork_status(sr, CK_NOFORK);
//	srunner_add_suite
	srunner_run_all(sr, CK_NORMAL);
	int number_failed;
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}
