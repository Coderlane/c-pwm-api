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
  int uspt_enabled;
  float uspt_duty_cycle;
  float uspt_frequency;
};

int
uspt_set_duty_cycle(struct us_pwm_t *pwm, float duty_cycle)
{
  struct us_pwm_test_t *pwm_test = pwm->uspwm_ctx;
  pwm_test->uspt_duty_cycle = duty_cycle;
  return USP_OK;
}

int
uspt_get_duty_cycle(struct us_pwm_t *pwm, float *out_duty_cycle)
{
  struct us_pwm_test_t *pwm_test = pwm->uspwm_ctx;
  *out_duty_cycle = pwm_test->uspt_duty_cycle;
  return USP_OK;
}

int
uspt_set_frequency(struct us_pwm_t *pwm, float frequency)
{
  struct us_pwm_test_t *pwm_test = pwm->uspwm_ctx;
  pwm_test->uspt_frequency = frequency;
  return USP_OK;
}

int
uspt_get_frequency(struct us_pwm_t *pwm, float *out_frequency)
{
  struct us_pwm_test_t *pwm_test = pwm->uspwm_ctx;
  *out_frequency = pwm_test->uspt_frequency;
  return USP_OK;
}

void
uspt_init(struct us_pwm_t *pwm, struct us_pwm_test_t *pwm_test)
{
  pwm->uspwm_ctx = pwm_test;
  pwm->uspwm_set_duty_cycle_func = uspt_set_duty_cycle;
  pwm->uspwm_get_duty_cycle_func = uspt_get_duty_cycle;
  pwm->uspwm_set_frequency_func = uspt_set_frequency;
  pwm->uspwm_get_frequency_func = uspt_get_frequency;
  us_pwm_ref(pwm);
}

START_TEST(test_pwm_set_get_duty_cycle_valid)
{
  int rv;
  float duty_cycle;
  struct us_pwm_t *pwm;
  struct us_pwm_test_t pwm_test;
  pwm = us_pwm_new((struct udev_device *)&pwm);
  uspt_init(pwm, &pwm_test);

  rv = us_pwm_set_duty_cycle(pwm, 0.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(duty_cycle != 0.0f, "Duty cycle not expected value.");

  rv = us_pwm_set_duty_cycle(pwm, 33.33333333333f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(duty_cycle != 33.33333333333f, "Duty cycle not expected value.");

  rv = us_pwm_set_duty_cycle(pwm, 50.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(duty_cycle != 50.0f, "Duty cycle not expected value.");

  rv = us_pwm_set_duty_cycle(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(duty_cycle != 100.0f, "Duty cycle not expected value.");

  us_pwm_unref(pwm);
}
END_TEST

START_TEST(test_pwm_set_get_duty_cycle_invalid)
{
  int rv;
  float duty_cycle;
  struct us_pwm_t *pwm;
  struct us_pwm_test_t pwm_test;
  pwm = us_pwm_new((struct udev_device *)&pwm);
  uspt_init(pwm, &pwm_test);

  rv = us_pwm_set_duty_cycle(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");

  rv = us_pwm_set_duty_cycle(pwm, 110.0f);
  fail_if(rv == USP_OK, "Return value not expected value.");
  rv = us_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(duty_cycle != 100.0f, "Duty cycle not expected value.");

  rv = us_pwm_set_duty_cycle(pwm, -0.1f);
  fail_if(rv == USP_OK, "Return value not expected value.");
  rv = us_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(duty_cycle != 100.0f, "Duty cycle not expected value.");

  us_pwm_unref(pwm);
}
END_TEST

START_TEST(test_pwm_set_get_frequency_valid)
{
  int rv;
  float frequency;
  struct us_pwm_t *pwm;
  struct us_pwm_test_t pwm_test;
  pwm = us_pwm_new((struct udev_device *)&pwm);
  uspt_init(pwm, &pwm_test);

  rv = us_pwm_set_frequency(pwm, 0.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(frequency != 0.0f, "Frequency not expected value.");

  rv = us_pwm_set_frequency(pwm, 33.33333333333f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(frequency != 33.33333333333f, "Frequency not expected value.");

  rv = us_pwm_set_frequency(pwm, 50.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(frequency != 50.0f, "Frequency not expected value.");

  rv = us_pwm_set_frequency(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");
  rv = us_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(frequency != 100.0f, "Frequency not expected value.");

  us_pwm_unref(pwm);
}
END_TEST

START_TEST(test_pwm_set_get_frequency_invalid)
{
  int rv;
  float frequency;
  struct us_pwm_t *pwm;
  struct us_pwm_test_t pwm_test;
  pwm = us_pwm_new((struct udev_device *)&pwm);
  uspt_init(pwm, &pwm_test);

  rv = us_pwm_set_frequency(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value.");

  rv = us_pwm_set_frequency(pwm, -0.1f);
  fail_if(rv == USP_OK, "Return value not expected value.");
  rv = us_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value.");
  fail_if(frequency != 100.0f, "Frequency not expected value.");

  us_pwm_unref(pwm);
}
END_TEST

Suite *
suite_pwm_new()
{
  Suite *suite_pwm = suite_create("suite_pwm");
  TCase *case_pwm_duty_cycle = tcase_create("test_pwm_duty_cycle");
  TCase *case_pwm_frequency = tcase_create("test_pwm_frequency");

  tcase_add_test(case_pwm_duty_cycle, test_pwm_set_get_duty_cycle_valid);
  tcase_add_test(case_pwm_duty_cycle, test_pwm_set_get_duty_cycle_invalid);
  tcase_add_test(case_pwm_frequency, test_pwm_set_get_frequency_valid);
  tcase_add_test(case_pwm_frequency, test_pwm_set_get_frequency_invalid);

  suite_add_tcase(suite_pwm, case_pwm_duty_cycle);
  suite_add_tcase(suite_pwm, case_pwm_frequency);
  return suite_pwm;
}

int
main()
{
  int failed;
  Suite *suite_pwm = suite_pwm_new();
  SRunner *runner_pwm = srunner_create(suite_pwm);
  srunner_run_all(runner_pwm, CK_NORMAL);
  failed = srunner_ntests_failed(runner_pwm);
  srunner_free(runner_pwm);
  return failed;
}
