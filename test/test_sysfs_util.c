/**
 * @file test_sysfs_util.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-06
 */

#include <check.h>
#include <fcntl.h>
#include <unistd.h>

#include "pwm.h"
#include "pwm_internal.h"

int
usp_test_file_new(const char *path)
{
  int fd;

  fd = open(path, O_RDONLY | O_CREAT);
  assert(fd >= 0);
  close(fd);

  return USP_OK;
}

int
usp_test_file_delete(const char *path)
{
  int rc;
  rc = unlink(path);
  assert(rc == 0);

  return USP_OK;
}

START_TEST(test_set_get_str)
{
  usp_test_file_new("test_set_get_str");

  usp_test_file_delete("test_set_get_str");
}
END_TEST

START_TEST(test_set_get_int)
{
  usp_test_file_new("test_set_get_str");

  usp_test_file_delete("test_set_get_str");
}
END_TEST

START_TEST(test_set_get_float)
{
  usp_test_file_new("test_set_get_str");

  usp_test_file_delete("test_set_get_str");
}
END_TEST

Suite *
suite_su_new()
{
  Suite *suite_su = suite_create("suite_sysfs_util");
  TCase *case_su_set_get_valid = tcase_create("case_set_get_valid");

  tcase_add_test(case_su_set_get_valid, test_set_get_str);
  tcase_add_test(case_su_set_get_valid, test_set_get_int);
  tcase_add_test(case_su_set_get_valid, test_set_get_float);
  suite_add_tcase(suite_su, case_su_set_get_valid);
  return suite_su;
}

int
main()
{
  int failed;
  Suite *suite_su;
  SRunner *runner_su;

  suite_su = suite_su_new();
  runner_su = srunner_create(suite_su);

  srunner_run_all(runner_su, CK_NORMAL);
  failed = srunner_ntests_failed(runner_su);
  srunner_free(runner_su);
  return failed;
}
