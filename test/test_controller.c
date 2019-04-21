/**
 * @file test_controller.c
 * @author Travis Lane
 * @version 0.0.1
 * @date 2019-04-21
 */

#include <check.h>

#include "pwm.h"
#include "pwm_internal.h"

START_TEST(test_controller_basic)
{
  struct usp_controller_t *ctrl;
  struct usp_pwm_list_t *lst;

  ctrl = usp_controller_new();
  fail_if(ctrl == NULL, "Failed to create controller.");

  lst = usp_controller_get_pwms(ctrl);
  fail_if(lst == NULL, "Failed to get pwm list.");

  usp_pwm_list_unref(lst);

  usp_controller_delete(ctrl);
}
END_TEST

static Suite *
suite_controller_new()
{
  Suite *suite_controller = suite_create("suite_controller");
  TCase *case_controller = tcase_create("test_controller");

  tcase_add_test(case_controller, test_controller_basic);

  suite_add_tcase(suite_controller, case_controller);
  return suite_controller;
}

int
main()
{
  int failed;
  Suite *suite_controller = suite_controller_new();
  SRunner *runner_controller = srunner_create(suite_controller);
  srunner_run_all(runner_controller, CK_NORMAL);
  failed = srunner_ntests_failed(runner_controller);
  srunner_free(runner_controller);
  return failed;
}
