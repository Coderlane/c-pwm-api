/**
 * @file test_ref.c
 * @brief
 * @author Travis Lane
 * @version 0.0.2
 * @date 2015-09-06
 */


#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <check.h>

#include "pwm.h"
#include "pwm_internal.h"

/* A global test PWM */
static struct usp_pwm_t *pwm;

static char pwm_dir[] = "/tmp/odc1.XXXXXX";

static void touch(const char *path)
{
  int fd;

  fd = open(path, O_RDONLY|O_CREAT,  S_IRUSR | S_IWUSR);
  fail_if(fd < 0, "Failed to create temp file.");

  close(fd);
}

/* Setup the test PWM */
static void test_setup(void)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  char template[] = "/tmp/odc1.XXXXXX";
  char *dir_name;
  int rv;

  dir_name = mkdtemp(template);
  strcpy(pwm_dir, dir_name);

  pwm = odc1_new(NULL, dir_name, 0);
  fail_if(pwm == NULL, "Failed to create PWM.");

  odc1_pwm = pwm->uspwm_ctx;
  fail_if(odc1_pwm == NULL, "Failed to create PWM context.");

  touch(odc1_pwm->odc1_enabled_attr);
  touch(odc1_pwm->odc1_duty_cycle_attr);
  touch(odc1_pwm->odc1_frequency_attr);

  rv = usp_pwm_enable(pwm);
  fail_if(rv != USP_OK, "Enable return value not expected value: %d", rv);
}

/* Cleanup the test PWM */
static void test_teardown(void)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  int rv;

  rv = usp_pwm_disable(pwm);
  fail_if(rv != USP_OK, "Disable return value not expected value: %d", rv);

  odc1_pwm = pwm->uspwm_ctx;
  remove(odc1_pwm->odc1_enabled_attr);
  remove(odc1_pwm->odc1_duty_cycle_attr);
  remove(odc1_pwm->odc1_frequency_attr);
  rmdir(pwm_dir);

  usp_pwm_unref(pwm);
}

START_TEST(test_pwm_get_name)
{
  fail_if(strcmp(usp_pwm_get_name(pwm), "odc1_pwm0") != 0,
      "Name %s did not match expected value.", usp_pwm_get_name(pwm));
}
END_TEST

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
  fail_if(floor(duty_cycle) != 33, "Duty cycle not expected value.");

  rv = usp_pwm_set_duty_cycle(pwm, 50.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_duty_cycle(pwm, &duty_cycle);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(floor(duty_cycle) != 50.0f, "Duty cycle not expected value.");

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

  rv = usp_pwm_set_frequency(pwm, 33.0f);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  rv = usp_pwm_get_frequency(pwm, &frequency);
  fail_if(rv != USP_OK, "Return value not expected value: %d", rv);
  fail_if(floor(frequency) != 33.0f, "Frequency not expected value.");

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
  TCase *case_odc1_generic = tcase_create("test_generic");
  TCase *case_odc1_converters = tcase_create("test_converters");
  TCase *case_odc1_duty_cycle = tcase_create("test_odc1_duty_cycle");
  TCase *case_odc1_frequency = tcase_create("test_odc1_frequency");

  tcase_add_checked_fixture(case_odc1_generic, test_setup, test_teardown);
  tcase_add_checked_fixture(case_odc1_duty_cycle, test_setup, test_teardown);
  tcase_add_checked_fixture(case_odc1_frequency, test_setup, test_teardown);

  tcase_add_test(case_odc1_generic, test_pwm_get_name);
  tcase_add_test(case_odc1_duty_cycle, test_pwm_set_get_duty_cycle_valid);
  tcase_add_test(case_odc1_duty_cycle, test_pwm_set_get_duty_cycle_invalid);
  tcase_add_test(case_odc1_frequency, test_pwm_set_get_frequency_valid);
  tcase_add_test(case_odc1_frequency, test_pwm_set_get_frequency_invalid);

  suite_add_tcase(suite_odc1, case_odc1_duty_cycle);
  suite_add_tcase(suite_odc1, case_odc1_frequency);

  tcase_add_test(case_odc1_converters, test_odc1_duty_to_percent);
  tcase_add_test(case_odc1_converters, test_odc1_percent_to_duty);

  suite_add_tcase(suite_odc1, case_odc1_converters);
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
