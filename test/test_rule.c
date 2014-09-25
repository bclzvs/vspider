#include "../include/rule.h"
#include "../include/unit_test.h"

START_TEST(test_rule_load)
{
	xmlDocPtr doc = rule_load("test");
	ck_assert(doc != NULL);	
}
END_TEST

START_TEST(test_rule_first)
{
	xmlDocPtr doc = rule_load("test");
	rule_t *prule = rule_first(doc);
	ck_assert(prule != NULL);	
	ck_assert_int_eq(XML_ELEMENT_NODE, prule->node->type);
	ck_assert_str_eq("rule", prule->node->name);
}
END_TEST

START_TEST(test_rule_attr)
{
	xmlDocPtr doc = rule_load("test");
	rule_t *prule = rule_first(doc);
	ck_assert_str_eq("testp", prule->pattern);
	ck_assert_str_eq("single", prule->match);
	ck_assert_str_eq("menu", prule->name);
}
END_TEST

Suite *make_rule_suite()
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("rule");
	tc_core = tcase_create("rule");
	tcase_add_test(tc_core,test_rule_load);
	tcase_add_test(tc_core,test_rule_first);
	tcase_add_test(tc_core,test_rule_attr);

	suite_add_tcase(s, tc_core);
	return s;
}

