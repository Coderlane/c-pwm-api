/**
 * @file test_sysfs_util.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-06
 */

#include <sys/stat.h>

#include <check.h>
#include <fcntl.h>
#include <unistd.h>

#include "pwm.h"
#include "pwm_internal.h"

static char *test_file_str = "./test_file";

void
test_file_setup()
{
  int fd;

  fd = open(test_file_str, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
  fail_if(fd < 0, "Failed to create test file.");
  close(fd);
}

void
test_file_cleanup()
{
  int rc;
  rc = unlink(test_file_str);
  fail_if(rc != 0, "Failed to clean up test file.");
}

START_TEST(test_set_get_str)
{
  int rc;
  ssize_t out_len, data_len;
  const char *data = "test data";
  char data_out[16];

  data_len = strlen(data);

  rc = sysfs_write_attr_str(test_file_str, data, strlen(data), &out_len);
  fail_if(rc != USP_OK);
  fail_if(out_len != data_len);

  rc = sysfs_read_attr_str(test_file_str, data_out, sizeof(data_out), &out_len);
  fail_if(rc != USP_OK);
  fail_if(out_len != data_len);
  printf("data: \"%s\" data_out \"%s\"", data, data_out);
  fail_if(strcmp(data, data_out) != 0);
}
END_TEST

START_TEST(test_set_get_int)
{
  int rc;
  int data, data_out;

  data = 0;
  rc = sysfs_write_attr_int(test_file_str, data);
  fail_if(rc != USP_OK);

  rc = sysfs_read_attr_int(test_file_str, &data_out);
  fail_if(rc != USP_OK);
  fail_if(data_out != data);
}
END_TEST

START_TEST(test_set_get_float)
{
  int rc;
  float data, data_out;

  data = 0.0f;
  rc = sysfs_write_attr_float(test_file_str, data);
  fail_if(rc != USP_OK);

  rc = sysfs_read_attr_float(test_file_str, &data_out);
  fail_if(rc != USP_OK);
  fail_if(data_out != data);
}
END_TEST

Suite *
suite_su_new()
{
  Suite *suite_su = suite_create("suite_sysfs_util");
  TCase *case_su_sg_valid = tcase_create("case_set_get_valid");

  tcase_add_test(case_su_sg_valid, test_set_get_str);
  tcase_add_test(case_su_sg_valid, test_set_get_int);
  tcase_add_test(case_su_sg_valid, test_set_get_float);
  tcase_add_checked_fixture(case_su_sg_valid, test_file_setup,
                            test_file_cleanup);
  suite_add_tcase(suite_su, case_su_sg_valid);
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
