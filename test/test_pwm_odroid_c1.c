/**
 * @file test_ref.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-06
 */

#include <check.h>

#include "pwm.h"
#include "pwm_internal.h"


START_TEST(test_odc1_duty_to_percent)
{
  int rc;
  float percent;

  rc = odc1_duty_cycle_to_percent(1023, &percent);
  fail_if(rc != USP_OK);
  fail_if(percent != 100.0f);

  rc = odc1_duty_cycle_to_percent(0, &percent);
  fail_if(rc != USP_OK);
  fail_if(percent != 0.0f);
}
END_TEST

START_TEST(test_odc1_percent_to_duty)
{
  int rc, duty_cycle;

  rc = odc1_percent_to_duty_cycle(100.0f, &duty_cycle);
  fail_if(rc != USP_OK);
  fail_if(duty_cycle != 1023);

  rc = odc1_percent_to_duty_cycle(0.0f, &duty_cycle);
  fail_if(rc != USP_OK);
  fail_if(duty_cycle != 0);
}
END_TEST

Suite *
suite_odc1_new()
{
  Suite *suite_odc1 = suite_create("suite_odroid_c1");
  TCase *case_odc1_conterters = tcase_create("test_converters");

  tcase_add_test(case_odc1_conterters, test_odc1_duty_to_percent);
  tcase_add_test(case_odc1_conterters, test_odc1_percent_to_duty);

  suite_add_tcase(suite_odc1, case_odc1_conterters);
  return suite_odc1;
}

int
main()
{
  int failed;
  Suite *suite_odc1 = suite_odc1_new();
  SRunner *runner_odc1 = srunner_create(suite_odc1);
  srunner_run_all(runner_odc1, CK_NORMAL);
  failed = srunner_ntests_failed(runner_odc1);
  srunner_free(runner_odc1);
  return failed;
}
