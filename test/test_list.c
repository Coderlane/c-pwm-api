/**
 * @file test_ref.c
 * @brief
 * @author Travis Lane
 * @version 0.0.2
 * @date 2015-09-06
 */

#include <check.h>

#include "pwm.h"
#include "pwm_internal.h"

struct usp_pwm_test_t {
  const char *uspt_id;
  int uspt_should_free;
  int uspt_freed;
};

void pwm_test_init(struct usp_pwm_test_t *pwm_test, struct usp_pwm_t *pwm,
                   const char *id, int should_free);
void pwm_test_free(void *arg);

void
pwm_test_init(struct usp_pwm_test_t *pwm_test, struct usp_pwm_t *pwm,
              const char *id, int should_free)
{
  memset(pwm, 0, sizeof(struct usp_pwm_t));
  pwm->uspwm_ctx = pwm_test;
  pwm_test->uspt_id = id;
  pwm_test->uspt_should_free = should_free;
  pwm_test->uspt_freed = 0;

  usp_ref_init(pwm, pwm_test_free);
}

void
pwm_test_free(void *arg)
{
  struct usp_pwm_t *pwm = arg;
  struct usp_pwm_test_t *pwm_test = pwm->uspwm_ctx;

  fail_if(pwm_test->uspt_freed == 1, "pwm_test already freed.");
  pwm_test->uspt_freed = 1;
  fail_if(pwm_test->uspt_should_free == 0,
          "ref freed, when I shouldn't have been.");
}

START_TEST(test_list_order)
{
  struct usp_pwm_t pwm_a, pwm_b, pwm_c;
  struct usp_pwm_test_t pwm_test_a, pwm_test_b, pwm_test_c;
  struct usp_pwm_list_t *list;
  list = usp_pwm_list_new();

  pwm_test_init(&pwm_test_a, &pwm_a, "test_a", 1);
  pwm_test_init(&pwm_test_b, &pwm_b, "test_b", 1);
  pwm_test_init(&pwm_test_c, &pwm_c, "test_c", 1);

  usp_pwm_list_add(list, &pwm_a);
  usp_pwm_list_add(list, &pwm_b);
  usp_pwm_list_add(list, &pwm_c);

  usp_pwm_unref(&pwm_a);
  usp_pwm_unref(&pwm_b);
  usp_pwm_unref(&pwm_c);

  usp_pwm_list_unref(list);

  fail_if(pwm_test_a.uspt_freed == 0, "pwm_test_a not freed.");
  fail_if(pwm_test_b.uspt_freed == 0, "pwm_test_b not freed.");
  fail_if(pwm_test_c.uspt_freed == 0, "pwm_test_c not freed.");
}
END_TEST

START_TEST(test_list_post_unref_ref)
{
  struct usp_pwm_t pwm_a, pwm_b, pwm_c;
  struct usp_pwm_test_t pwm_test_a, pwm_test_b, pwm_test_c;
  struct usp_pwm_list_t *list;
  list = usp_pwm_list_new();

  pwm_test_init(&pwm_test_a, &pwm_a, "test_a", 1);
  pwm_test_init(&pwm_test_b, &pwm_b, "test_b", 1);
  pwm_test_init(&pwm_test_c, &pwm_c, "test_c", 1);

  usp_pwm_list_add(list, &pwm_a);
  usp_pwm_list_add(list, &pwm_b);
  usp_pwm_list_add(list, &pwm_c);

  usp_pwm_unref(&pwm_a);
  usp_pwm_unref(&pwm_b);
  usp_pwm_unref(&pwm_c);
  usp_pwm_list_unref(list);
}
END_TEST

Suite *
suite_ref_new()
{
  Suite *suite_ref = suite_create("suite_list");
  TCase *case_list_order = tcase_create("case_list_order");
  TCase *case_list_ref = tcase_create("case_list_ref");
  tcase_add_test(case_list_order, test_list_order);
  tcase_add_test(case_list_ref, test_list_post_unref_ref);
  suite_add_tcase(suite_ref, case_list_ref);
  suite_add_tcase(suite_ref, case_list_order);
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
