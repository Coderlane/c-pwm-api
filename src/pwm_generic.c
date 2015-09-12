/**
 * @file pwm_generic.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-05
 */

#include <libudev.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "pwm_internal.h"
#include "pwm.h"

/**
 * @brief Enable a generic pwm.
 *
 * @param us_pwm The pwm to enable.
 *
 * @return A status code.
 */
int
us_pwm_enable(struct us_pwm_t *us_pwm)
{
  assert(us_pwm != NULL);
  assert(us_pwm->uspwm_enable_func != NULL);

  return us_pwm->uspwm_enable_func(us_pwm, USPWM_ENABLED);
}

/**
 * @brief Disable a generic pwm.
 *
 * @param us_pwm The pwm to disable.
 *
 * @return A status code.
 */
int
us_pwn_disable(struct us_pwm_t *us_pwm)
{
  assert(us_pwm != NULL);
  assert(us_pwm->uspwm_disable_func != NULL);

  return us_pwm->uspwm_disable_func(us_pwm, USPWM_DISABLED);
}
