/**
 * @file pwm_test.c
 * @brief
 * @author Travis Lane
 * @version 0.0.2
 * @date 2015-09-21
 */

#include <libudev.h>

#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pwm_internal.h"
#include "pwm.h"

struct usp_pwm_test_t {
  int test_id;
  const char *test_name;
  const char *test_off_str;
  const char *test_on_str;
  const char *test_enabled_attr;
  const char *test_duty_cycle_attr;
  const char *test_frequency_attr;
};

struct usp_pwm_test_t test_zero = {
    .test_id = 0,
    .test_name = "test_pwm0",
    .test_on_str = "on",
    .test_off_str = "off",
    .test_enabled_attr = "enable0",
    .test_duty_cycle_attr = "duty0",
    .test_frequency_attr = "freq0"
};

struct usp_pwm_test_t test_one = {
    .test_id = 1,
    .test_name = "test_pwm1",
    .test_on_str = "on",
    .test_off_str = "off",
    .test_enabled_attr = "enable1",
    .test_duty_cycle_attr = "duty1",
    .test_frequency_attr = "freq1"
};

static int test_create_file(const char *file);
static int test_delete_file(const char *file);
static int test_disable(struct usp_pwm_t *);
static int test_enable(struct usp_pwm_t *);
static int test_set_duty_cycle(struct usp_pwm_t *, float);
static int test_get_duty_cycle(struct usp_pwm_t *, float *);
static int test_set_frequency(struct usp_pwm_t *, float);
static int test_get_frequency(struct usp_pwm_t *, float *);

/**
 * @brief Create a new test pwm. Creates a new generic pwm
 * and populates it's fields with test pwm specific attributes.
 *
 * @param device The udev device parent.
 * @param id The pwm ID, should be 0 or 1.
 *
 * @return A new pwm.
 */
struct usp_pwm_t *
test_new(struct udev_device *device, int id)
{
  struct usp_pwm_t *pwm;
  struct usp_pwm_test_t *test_pwm;
  assert(id == 0 || id == 1);

  test_pwm = id == 0 ? &test_zero : &test_one;

  pwm = usp_pwm_new(device, test_pwm->test_name, USPWM_TEST);
  pwm->uspwm_ctx = test_pwm;
  pwm->uspwm_enable_func = test_enable;
  pwm->uspwm_disable_func = test_disable;
  pwm->uspwm_get_duty_cycle_func = test_get_duty_cycle;
  pwm->uspwm_set_duty_cycle_func = test_set_duty_cycle;
  pwm->uspwm_get_frequency_func = test_get_frequency;
  pwm->uspwm_set_frequency_func = test_set_frequency;

  return pwm;
}

/**
 * @brief Search for pwms and attach them to a controller.
 *
 * @param ctrl The controller to attach the pwms to.
 *
 * @return A status code.
 */
int
test_search(struct usp_controller_t *ctrl)
{
  struct usp_pwm_t *pwm;

  pwm = test_new(NULL, 0);
  usp_controller_add_pwm(ctrl, pwm);
  pwm = test_new(NULL, 1);
  usp_controller_add_pwm(ctrl, pwm);

  return USP_OK;
}

static int
test_enable(struct usp_pwm_t *pwm)
{
  int rc;
  ssize_t write_len;
  struct usp_pwm_test_t *test_pwm;
  assert(pwm->uspwm_type == USPWM_TEST);
  test_pwm = pwm->uspwm_ctx;

  test_create_file(test_pwm->test_enabled_attr);
  test_create_file(test_pwm->test_duty_cycle_attr);
  test_create_file(test_pwm->test_frequency_attr);

  rc = sysfs_write_attr_str(test_pwm->test_enabled_attr, test_pwm->test_on_str,
                            strlen(test_pwm->test_on_str), &write_len);

  return rc;
}

static int
test_disable(struct usp_pwm_t *pwm)
{
  int rc;
  ssize_t write_len;
  struct usp_pwm_test_t *test_pwm;
  assert(pwm->uspwm_type == USPWM_TEST);
  test_pwm = pwm->uspwm_ctx;

  rc = sysfs_write_attr_str(test_pwm->test_enabled_attr, test_pwm->test_on_str,
                            strlen(test_pwm->test_on_str), &write_len);

  test_delete_file(test_pwm->test_enabled_attr);
  test_delete_file(test_pwm->test_duty_cycle_attr);
  test_delete_file(test_pwm->test_frequency_attr);

  return rc;
}

static int
test_create_file(const char *file)
{
  int fd;
  fd = creat(file, 0777);
  if (fd < 0)
    return USP_FILE_ERROR;

  close(fd);
  return USP_OK;
}

static int
test_delete_file(const char *file)
{
  return unlink(file) ? USP_FILE_ERROR : USP_OK;
}

static int
test_set_duty_cycle(struct usp_pwm_t *pwm, float duty_cycle)
{
  struct usp_pwm_test_t *test_pwm;
  assert(pwm->uspwm_type == USPWM_TEST);
  test_pwm = pwm->uspwm_ctx;

  return sysfs_write_attr_float(test_pwm->test_duty_cycle_attr, duty_cycle);
}

static int
test_get_duty_cycle(struct usp_pwm_t *pwm, float *out_duty_cycle)
{
  struct usp_pwm_test_t *test_pwm;
  assert(pwm->uspwm_type == USPWM_TEST);
  test_pwm = pwm->uspwm_ctx;

  return sysfs_read_attr_float(test_pwm->test_duty_cycle_attr, out_duty_cycle);
}

static int
test_set_frequency(struct usp_pwm_t *pwm, float frequency)
{
  struct usp_pwm_test_t *test_pwm;
  assert(pwm->uspwm_type == USPWM_TEST);
  test_pwm = pwm->uspwm_ctx;

  return sysfs_write_attr_float(test_pwm->test_frequency_attr, frequency);
}

static int
test_get_frequency(struct usp_pwm_t *pwm, float *out_frequency)
{
  struct usp_pwm_test_t *test_pwm;
  assert(pwm->uspwm_type == USPWM_TEST);
  test_pwm = pwm->uspwm_ctx;

  return sysfs_read_attr_float(test_pwm->test_frequency_attr, out_frequency);
}
