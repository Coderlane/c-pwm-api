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

struct us_pwm_test_t {
  const char *uspt_id;
  int uspt_should_free;
};

void
pwm_free(void *arg)
{
  struct us_pwm_t *pwm = arg;
  struct us_pwm_test_t *pwm_test = pwm->uspwm_ctx;

  fail_if(pwm_test->uspt_should_free == 0,
          "ref freed, when I shouldn't have been.");
}

START_TEST(test_list_order)
{
  struct us_pwm_t pwm_a, pwm_b, pwm_c;
  struct us_pwm_list_t *list;
  list = us_pwm_list_new();
  usp_ref_init(&pwm_a, pwm_free);
  usp_ref_init(&pwm_b, pwm_free);
  usp_ref_init(&pwm_c, pwm_free);


  us_pwm_list_unref(list);
}
END_TEST

START_TEST(test_list_post_unref_ref) {}
END_TEST

Suite *
suite_ref_new()
{
  Suite *suite_ref = suite_create("suite_ref");
  TCase *case_ref_sf = tcase_create("test_ref_should_free");
  TCase *case_ref_snf = tcase_create("test_ref_should_not_free");
  tcase_add_test(case_ref_sf, test_list_order);
  tcase_add_test(case_ref_snf, test_list_post_unref_ref);
  suite_add_tcase(suite_ref, case_ref_sf);
  suite_add_tcase(suite_ref, case_ref_snf);
  return suite_ref;
}

int
main()
{
  int failed;
  Suite *suite_ref = suite_ref_new();
  SRunner *runner_ref = srunner_create(suite_ref);
  srunner_run_all(runner_ref, CK_NORMAL);
  failed = srunner_ntests_failed(runner_ref);
  srunner_free(runner_ref);
  return failed;
}
