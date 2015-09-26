/**
 * @file pwm_odroid_c1.c
 * @brief 
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-21
 */

#include <libudev.h>

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pwm_internal.h"
#include "pwm.h"

struct us_pwm_odroid_c1_t {
  int odc1_id;
  const char *odc1_off_str;
  const char *odc1_on_str;
  const char *odc1_enabled_attr;
  const char *odc1_duty_cycle_attr;
  const char *odc1_frequency_attr;
};

struct us_pwm_odroid_c1_t odc1_zero = {
  .odc1_id = 0,
  .odc1_on_str = "PWM_0 : on",
  .odc1_off_str = "PWM_0 : off",
  .odc1_enabled_attr = "/sys/devices/platform/pwm-ctrl/enabled0",
  .odc1_duty_cycle_attr = "/sys/devices/platform/pwm-ctrl/duty0",
  .odc1_frequency_attr = "/sys/devices/platform/pwm-ctrl/freq0"
};

struct us_pwm_odroid_c1_t odc1_one = {
  .odc1_id = 1,
  .odc1_on_str = "PWM_1 : on",
  .odc1_off_str = "PWM_1 : off",
  .odc1_enabled_attr = "/sys/devices/platform/pwm-ctrl/enabled1",
  .odc1_duty_cycle_attr = "/sys/devices/platform/pwm-ctrl/duty1",
  .odc1_frequency_attr = "/sys/devices/platform/pwm-ctrl/freq1"
};

struct us_pwm_t *
odc1_new(struct udev_device *device, int id)
{
  struct us_pwm_t *pwm;
  assert(id == 0 || id == 1);

  pwm = us_pwm_new(device, USPWM_ODC1);
  pwm->uspwm_ctx = id == 0 ? &odc1_zero : &odc1_one;

  return pwm;
}

void
odc1_search(struct us_pwm_controller_t *ctrl)
{
	struct udev_enumerate *enumer = NULL;
	struct udev_list_entry *dev_list = NULL;

	enumer = udev_enumerate_new(ctrl->uspc_udev);

  udev_enumerate_add_match_sysattr(enumer, "driver", "pwm-ctrl");
	udev_enumerate_scan_devices(enumer);
	dev_list = udev_enumerate_get_list_entry(enumer);


	udev_enumerate_unref(enumer);
}

int
odc1_enable(struct us_pwm_t *pwm)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}

int
odc1_disable(struct us_pwm_t *pwm)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}

int
odc1_set_duty_cycle(struct us_pwm_t *pwm, float duty_cycle)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}

int
odc1_get_duty_cycle(struct us_pwm_t *pwm, float *out_duty_cycle)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}

int
odc1_set_frequency(struct us_pwm_t *pwm, float duty_cycle)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}

int
odc1_get_frequency(struct us_pwm_t *pwm, float *out_duty_cycle)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}
