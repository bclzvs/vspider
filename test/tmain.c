#include "../include/unit_test.h"
#include <stdlib.h>

int main(int argc, const char **argv)
{
	Suite *s;
	SRunner *sr;
	s =make_util_http_suite();
	sr = srunner_create(s);
	srunner_set_fork_status(sr, CK_NOFORK);
	srunner_add_suite(sr, make_util_regex_suite());
	if(argc == 1){
		srunner_run_all(sr, CK_NORMAL);
	} else {
//		const char *sname = argv[1];
////		const char *tcname = argv[2];
//		srunner_run(sr, sname, tcname, CK_NORMAL);
	}
	int number_failed;
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE; 
}
