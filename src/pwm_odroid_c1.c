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
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pwm_internal.h"
#include "pwm.h"

struct usp_pwm_odroid_c1_t {
  int odc1_id;
  const char *odc1_name;
  const char *odc1_off_str;
  const char *odc1_on_str;
  const char *odc1_enabled_attr;
  const char *odc1_duty_cycle_attr;
  const char *odc1_frequency_attr;
};

struct usp_pwm_odroid_c1_t odc1_zero = {
    .odc1_id = 0,
    .odc1_name = "odc1_pwm0",
    .odc1_on_str = "PWM_0 : on",
    .odc1_off_str = "PWM_0 : off",
    .odc1_enabled_attr = "/sys/devices/platform/pwm-ctrl/enable0",
    .odc1_duty_cycle_attr = "/sys/devices/platform/pwm-ctrl/duty0",
    .odc1_frequency_attr = "/sys/devices/platform/pwm-ctrl/freq0"
};

struct usp_pwm_odroid_c1_t odc1_one = {
    .odc1_id = 1,
    .odc1_name = "odc1_pwm1",
    .odc1_on_str = "PWM_1 : on",
    .odc1_off_str = "PWM_1 : off",
    .odc1_enabled_attr = "/sys/devices/platform/pwm-ctrl/enable1",
    .odc1_duty_cycle_attr = "/sys/devices/platform/pwm-ctrl/duty1",
    .odc1_frequency_attr = "/sys/devices/platform/pwm-ctrl/freq1"
};

int odc1_disable(struct usp_pwm_t *);
int odc1_enable(struct usp_pwm_t *);
int odc1_set_duty_cycle(struct usp_pwm_t *, float);
int odc1_get_duty_cycle(struct usp_pwm_t *, float *);
int odc1_set_frequency(struct usp_pwm_t *, float);
int odc1_get_frequency(struct usp_pwm_t *, float *);

int odc1_percent_to_duty_cycle(float percent, int *out_duty_cycle);
int odc1_duty_cycle_to_percent(int duty_cycle, float *out_percent);

/**
 * @brief Create a new ODroid C1 pwm. Creates a new generic pwm
 * and populates it's fields with ODroid C1 specific attributes.
 *
 * @param device The udev device parent.
 * @param id The pwm ID, should be 0 or 1.
 *
 * @return A new pwm.
 */
struct usp_pwm_t *
odc1_new(struct udev_device *device, int id)
{
  struct usp_pwm_t *pwm;
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(id == 0 || id == 1);

  odc1_pwm = id == 0 ? &odc1_zero : &odc1_one;

  pwm = usp_pwm_new(device, odc1_pwm->odc1_name, USPWM_ODC1);
  pwm->uspwm_ctx = odc1_pwm;
  pwm->uspwm_enable_func = odc1_enable;
  pwm->uspwm_disable_func = odc1_disable;
  pwm->uspwm_get_duty_cycle_func = odc1_get_duty_cycle;
  pwm->uspwm_set_duty_cycle_func = odc1_set_duty_cycle;
  pwm->uspwm_get_frequency_func = odc1_get_frequency;
  pwm->uspwm_set_frequency_func = odc1_set_frequency;

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
odc1_search(struct usp_controller_t *ctrl)
{
  const char *driver, *path, *attr;
  struct usp_pwm_t *pwm;
  struct udev_device *dev;
  struct udev_enumerate *enumer = NULL;
  struct udev_list_entry *dev_list, *dev_entry, *dev_attributes, *dev_attribute;

  enumer = udev_enumerate_new(ctrl->uspc_udev);

  udev_enumerate_add_match_sysattr(enumer, "driver", "pwm-ctrl");
  udev_enumerate_scan_devices(enumer);
  dev_list = udev_enumerate_get_list_entry(enumer);

  udev_list_entry_foreach(dev_entry, dev_list)
  {
    path = udev_list_entry_get_name(dev_entry);
    dev = udev_device_new_from_syspath(ctrl->uspc_udev, path);
    driver = udev_device_get_driver(dev);
    if (strcmp(driver, "pwm-ctrl") != 0) {
      udev_device_unref(dev);
      continue;
    }

    dev_attributes = udev_device_get_sysattr_list_entry(dev);
    udev_list_entry_foreach(dev_attribute, dev_attributes)
    {
      attr = udev_list_entry_get_name(dev_attribute);
      if (strcmp(attr, "enable0") == 0) {
        pwm = odc1_new(dev, 0);
        usp_controller_add_pwm(ctrl, pwm);
      } else if (strcmp(attr, "enable1") == 0) {
        pwm = odc1_new(dev, 1);
        usp_controller_add_pwm(ctrl, pwm);
      }
    }
  }

  udev_enumerate_unref(enumer);
  return USP_OK;
}

int
odc1_enable(struct usp_pwm_t *pwm)
{
  int rc;
  ssize_t write_len;
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  rc = sysfs_write_attr_str(odc1_pwm->odc1_enabled_attr, odc1_pwm->odc1_on_str,
                            strlen(odc1_pwm->odc1_on_str), &write_len);

  return rc;
}

int
odc1_disable(struct usp_pwm_t *pwm)
{
  int rc;
  ssize_t write_len;
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  rc = sysfs_write_attr_str(odc1_pwm->odc1_enabled_attr, odc1_pwm->odc1_on_str,
                            strlen(odc1_pwm->odc1_on_str), &write_len);
  return rc;
}

int
odc1_set_duty_cycle(struct usp_pwm_t *pwm, float duty_cycle)
{
  int rc, duty;
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  rc = odc1_percent_to_duty_cycle(duty_cycle, &duty);
  if (rc != USP_OK)
    return rc;

  return sysfs_write_attr_int(odc1_pwm->odc1_duty_cycle_attr, duty);
}

int
odc1_get_duty_cycle(struct usp_pwm_t *pwm, float *out_duty_cycle)
{
  int rc, duty;
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  rc = sysfs_read_attr_int(odc1_pwm->odc1_duty_cycle_attr, &duty);
  if (rc != USP_OK)
    return rc;
  return odc1_duty_cycle_to_percent(duty, out_duty_cycle);
}

int
odc1_set_frequency(struct usp_pwm_t *pwm, float frequency)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return sysfs_write_attr_int(odc1_pwm->odc1_frequency_attr, (int)frequency);
}

int
odc1_get_frequency(struct usp_pwm_t *pwm, float *out_frequency)
{
  int freq, rc;
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  rc = sysfs_read_attr_int(odc1_pwm->odc1_frequency_attr, &freq);
  *out_frequency = (float)freq;
  return rc;
}

int
odc1_percent_to_duty_cycle(float percent, int *out_duty_cycle)
{
  if (percent < 0.0f || percent > 100.0f)
    return USP_INVALID_RANGE;

  *out_duty_cycle = (int)roundf((percent / 100.0f) * ODC1_DUTY_MAX);
  return USP_OK;
}

int
odc1_duty_cycle_to_percent(int duty_cycle, float *out_percent)
{
  if (duty_cycle > ODC1_DUTY_MAX || duty_cycle < ODC1_DUTY_MIN)
    return USP_INVALID_RANGE;

  *out_percent = ((float) duty_cycle / (float) ODC1_DUTY_MAX) * 100.0f;
  return USP_OK;
}
