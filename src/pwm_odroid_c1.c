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


#define ODC1_PWM0_ON  "PWM_0 : on"
#define ODC1_PWM1_ON  "PWM_1 : on"
#define ODC1_PWM0_OFF "PWM_0 : off"
#define ODC1_PWM1_OFF "PWM_1 : off"

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
  .odc1_enabled_attr = "enabled0",
  .odc1_duty_cycle_attr = "duty0",
  .odc1_frequency_attr = "freq0"
};

struct us_pwm_odroid_c1_t odc1_one = {
  .odc1_id = 1,
  .odc1_enabled_attr = "enabled1",
  .odc1_duty_cycle_attr = "duty1",
  .odc1_frequency_attr = "freq1"
};

struct us_pwm_t *
odc1_new(struct udev_device *device, int id)
{
  size_t syspath_len;
  const char *syspath;
  struct us_pwm_t *pwm;
  struct us_pwm_odroid_c1_t *odc1_pwm;


  assert(id == 0 || id == 1);
  pwm = us_pwm_new(device, USPWM_ODC1);
  odc1_pwm = malloc(sizeof(struct us_pwm_odroid_c1_t));
  odc1_pwm->odc1_id = id;

  switch (id) {
  case 0:
    odc1_pwm->odc1_off_str = ODC1_PWM0_OFF;
    odc1_pwm->odc1_on_str = ODC1_PWM0_ON;
    break;
  case 1:
    odc1_pwm->odc1_off_str = ODC1_PWM1_OFF;
    odc1_pwm->odc1_on_str = ODC1_PWM1_ON;
    break;
  }

  syspath = udev_device_get_syspath(device);
  syspath_len = strlen(syspath);


  pwm->uspwm_ctx = odc1_pwm;
  return pwm;
}

void
odc1_search(struct us_pwm_controller_t *ctrl)
{



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
