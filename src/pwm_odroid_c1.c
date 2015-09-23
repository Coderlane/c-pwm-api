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

#include "pwm_internal.h"
#include "pwm.h"

struct us_pwm_odroid_c1_t {
  int odc1_id;
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

int
odc1_enable(struct us_pwm_t *pwm, enum us_pwm_state_e state)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(state == USPWM_ENABLED);
  assert(pwm->uspwm_type == USPWM_ODC1);
  odc1_pwm = pwm->uspwm_ctx;

  return USP_NOT_IMPLEMENTED;
}

int
odc1_disable(struct us_pwm_t *pwm, enum us_pwm_state_e state)
{
  struct us_pwm_odroid_c1_t *odc1_pwm;
  assert(state == USPWM_DISABLED);
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
