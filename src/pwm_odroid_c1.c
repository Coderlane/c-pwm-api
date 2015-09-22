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

struct us_pwm_odroid_c1_t pwm_zero = {
  .odc1_id = 0,
  .odc1_enabled_attr = "enabled0",
  .odc1_duty_cycle_attr = "duty0",
  .odc1_frequency_attr = "freq0"
};

struct us_pwm_odroid_c1_t pwm_one = {
  .odc1_id = 1,
  .odc1_enabled_attr = "enabled1",
  .odc1_duty_cycle_attr = "duty1",
  .odc1_frequency_attr = "freq1"
};
