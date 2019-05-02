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

/* A global test PWM */
static struct usp_pwm_t *pwm;

/* Setup the test PWM */
static void test_setup(void)
{
  int rv;

  pwm = test_new(NULL, 1);
  rv = usp_pwm_enable(pwm);
  fail_if(rv != USP_OK, "Enable return value not expected value: %d", rv);
}

/* Cleanup the test PWM */
static void test_teardown(void)
{
  int rv;

  rv = usp_pwm_disable(pwm);
  fail_if(rv != USP_OK, "Disable return value not expected value: %d", rv);
  usp_pwm_unref(pwm);
}

START_TEST(test_pwm_set_get_duty_cycle_valid)
{
  int rv;
  float duty_cycle;

  rv = usp_pwm_set_duty_cycle(pwm, 0.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(duty_cycle != 0.0f, "Duty cycle not expected value.");

  rv = usp_pwm_set_duty_cycle(pwm, 33.33333333333f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(duty_cycle != 33.33333333333f, "Duty cycle not expected value.");

  rv = usp_pwm_set_duty_cycle(pwm, 50.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(duty_cycle != 50.0f, "Duty cycle not expected value.");

  rv = usp_pwm_set_duty_cycle(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(duty_cycle != 100.0f, "Duty cycle not expected value.");
}
END_TEST

START_TEST(test_pwm_set_get_duty_cycle_invalid)
{
  int rv;
  float duty_cycle;

  rv = usp_pwm_set_duty_cycle(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);

  rv = usp_pwm_set_duty_cycle(pwm, 110.0f);
  fail_if(rv == USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(duty_cycle != 100.0f, "Duty cycle not expected value.");

  rv = usp_pwm_set_duty_cycle(pwm, -0.1f);
  fail_if(rv == USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(duty_cycle != 100.0f, "Duty cycle not expected value.");
}
END_TEST

START_TEST(test_pwm_set_get_frequency_valid)
{
  int rv;
  float frequency;

  rv = usp_pwm_set_frequency(pwm, 0.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(frequency != 0.0f, "Frequency not expected value.");

  rv = usp_pwm_set_frequency(pwm, 33.33333333333f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(frequency != 33.33333333333f, "Frequency not expected value.");

  rv = usp_pwm_set_frequency(pwm, 50.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(frequency != 50.0f, "Frequency not expected value.");

  rv = usp_pwm_set_frequency(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(frequency != 100.0f, "Frequency not expected value.");
}
END_TEST

START_TEST(test_pwm_set_get_frequency_invalid)
{
  int rv;
  float frequency;

  rv = usp_pwm_set_frequency(pwm, 100.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);

  rv = usp_pwm_set_frequency(pwm, -0.1f);
  fail_if(rv == USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(frequency != 100.0f, "Frequency not expected value.");
}
END_TEST

Suite *
suite_pwm_new()
{
  Suite *suite_pwm = suite_create("suite_pwm");
  TCase *case_pwm_duty_cycle = tcase_create("test_pwm_duty_cycle");
  TCase *case_pwm_frequency = tcase_create("test_pwm_frequency");

  tcase_add_checked_fixture(case_pwm_duty_cycle, test_setup, test_teardown);
  tcase_add_checked_fixture(case_pwm_frequency, test_setup, test_teardown);

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
