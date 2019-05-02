/**
 * @file pwm_odroid_c1.c
 * @brief
 * @author Travis Lane
 * @version 0.0.2
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
#include <limits.h>

#include "pwm_internal.h"
#include "pwm.h"

static void odc1_delete(void *ctx);
static int odc1_disable(struct usp_pwm_t *);
static int odc1_enable(struct usp_pwm_t *);
static int odc1_set_duty_cycle(struct usp_pwm_t *, float);
static int odc1_get_duty_cycle(struct usp_pwm_t *, float *);
static int odc1_set_frequency(struct usp_pwm_t *, float);
static int odc1_get_frequency(struct usp_pwm_t *, float *);

struct usp_pwm_odroid_c1_t *
odc1_new_ctx(const char *path, unsigned int id)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm = NULL;
  size_t path_len, id_len;

  odc1_pwm = calloc(sizeof(*odc1_pwm), 1);
  if (odc1_pwm == NULL) {
    goto err;
  }

  // Figure out variable lengths
  path_len = strlen(path);
  id_len = floor(log10(id)) + 1;

  // Allocate space for variables
  odc1_pwm->odc1_name = malloc(sizeof(char *) *
      (sizeof("odc1_pwm") + id_len));
  if (odc1_pwm->odc1_name == NULL) {
    goto err;
  }

  odc1_pwm->odc1_enabled_attr = malloc(sizeof(char *) *
      (path_len + sizeof("/enable") + id_len));
  if (odc1_pwm->odc1_enabled_attr == NULL) {
    goto err;
  }

  odc1_pwm->odc1_duty_cycle_attr = malloc(sizeof(char *) *
      (path_len + sizeof("/duty") + id_len));
  if (odc1_pwm->odc1_duty_cycle_attr == NULL) {
    goto err;
  }

  odc1_pwm->odc1_frequency_attr = malloc(sizeof(char *) *
      (path_len + sizeof("/freq") + id_len));
  if (odc1_pwm->odc1_frequency_attr == NULL) {
    goto err;
  }

  // Setup the context
  odc1_pwm->odc1_id = id;
  sprintf(odc1_pwm->odc1_name, "odc1_pwm%u", id);
  sprintf(odc1_pwm->odc1_enabled_attr, "%s/enable%u", path, id);
  sprintf(odc1_pwm->odc1_duty_cycle_attr, "%s/duty%u", path, id);
  sprintf(odc1_pwm->odc1_frequency_attr, "%s/freq%u", path, id);

  goto out;
err:
  odc1_delete(odc1_pwm);
  odc1_pwm = NULL;
out:
  return odc1_pwm;
}

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
odc1_new(struct udev_device *device,
    const char *path, unsigned int id)
{
  struct usp_pwm_t *pwm = NULL;
  struct usp_pwm_odroid_c1_t *odc1_pwm = NULL;

  odc1_pwm = odc1_new_ctx(path, id);
  if (odc1_pwm == NULL) {
    goto out;
  }

  pwm = usp_pwm_new(device, odc1_pwm->odc1_name, USPWM_ODC1);
  pwm->uspwm_ctx = odc1_pwm;
  pwm->uspwm_ctx_delete_func = odc1_delete;
  pwm->uspwm_enable_func = odc1_enable;
  pwm->uspwm_disable_func = odc1_disable;
  pwm->uspwm_get_duty_cycle_func = odc1_get_duty_cycle;
  pwm->uspwm_set_duty_cycle_func = odc1_set_duty_cycle;
  pwm->uspwm_get_frequency_func = odc1_get_frequency;
  pwm->uspwm_set_frequency_func = odc1_set_frequency;

out:
  if (pwm == NULL) {
    odc1_delete(odc1_pwm);
  }

  return pwm;
}

static void
odc1_delete(void *ctx)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm = (struct usp_pwm_odroid_c1_t *) ctx;

  if (odc1_pwm == NULL) {
    return;
  }

  free(odc1_pwm->odc1_name);
  free(odc1_pwm->odc1_enabled_attr);
  free(odc1_pwm->odc1_duty_cycle_attr);
  free(odc1_pwm->odc1_frequency_attr);

  free(odc1_pwm);
}

static bool
odc1_is_pwm(const char *driver)
{
  return strcmp(driver, "pwm-ctrl") == 0;
}

static int
odc1_pwm_id(const char *attr)
{
  unsigned int id;

  if (sscanf(attr, "enable%u", &id) != 1) {
    return -1;
  }

  if (id > INT_MAX) {
    return USP_INVALID_RANGE;
  }
  return (int) id;
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
    if (!odc1_is_pwm(driver)) {
      udev_device_unref(dev);
      continue;
    }

    dev_attributes = udev_device_get_sysattr_list_entry(dev);
    udev_list_entry_foreach(dev_attribute, dev_attributes)
    {
      int id;

      attr = udev_list_entry_get_name(dev_attribute);
      id = odc1_pwm_id(attr);
      if (id >= 0) {
        pwm = odc1_new(dev, path, (unsigned int) id);
        if (pwm == NULL) {
          continue;
        }
        usp_controller_add_pwm(ctrl, pwm);
      }
    }
  }

  udev_enumerate_unref(enumer);
  return USP_OK;
}

static int
odc1_enable(struct usp_pwm_t *pwm)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  char write_buf[32];
  ssize_t write_len;

  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  write_len = snprintf(write_buf, sizeof(write_buf) - 1,
      "PWM_%u : on", odc1_pwm->odc1_id);
  return sysfs_write_attr_str(odc1_pwm->odc1_enabled_attr,
      write_buf, write_len, &write_len);
}

static int
odc1_disable(struct usp_pwm_t *pwm)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  char write_buf[32];
  ssize_t write_len;

  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  write_len = snprintf(write_buf, sizeof(write_buf) - 1,
      "PWM_%u : off", odc1_pwm->odc1_id);
  return sysfs_write_attr_str(odc1_pwm->odc1_enabled_attr,
      write_buf, write_len, &write_len);
}

static int
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

static int
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

static int
odc1_set_frequency(struct usp_pwm_t *pwm, float frequency)
{
  struct usp_pwm_odroid_c1_t *odc1_pwm;
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return sysfs_write_attr_int(odc1_pwm->odc1_frequency_attr, (int)frequency);
}

static int
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
