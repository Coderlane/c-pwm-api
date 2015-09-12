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
 * @brief
 *
 * @param enable_func
 * @param disable_func
 *
 * @return
 */
struct us_pwm_t *
us_pwm_new(struct udev_device *device, us_pwm_state_func_t enable_func,
           us_pwm_state_func_t disable_func)
{
  struct us_pwm_t *pwm = NULL;
  pwm = malloc(sizeof(struct us_pwm_t));
  assert(pwm != NULL);

  pwm->uspwm_device = device;
  pwm->uspwm_enable_func = enable_func;
  pwm->uspwm_disable_func = disable_func;

  usp_ref_init(pwm, us_pwm_delete);

  return pwm;
}

/**
 * @brief
 *
 * @param ctx
 */
void
us_pwm_delete(void *ctx)
{
  struct us_pwm_t *pwm = ctx;
  assert(pwm != NULL);
  assert(pwm->usp_ref_count == 0);

  free(pwm);
}

/**
 * @brief
 *
 * @param pwm
 */
void
us_pwm_ref(struct us_pwm_t *pwm)
{
  usp_ref(pwm);
}

/**
 * @brief
 *
 * @param pwm
 */
void
us_pwm_unref(struct us_pwm_t *pwm)
{
  usp_unref(pwm);
}

/**
 * @brief Enable a pwm. This function will enable the pwm for use,
 * but will not yet start the pwm.
 *
 * @param pwm The pwm to enable.
 *
 * @return A status code.
 */
int
us_pwm_enable(struct us_pwm_t *pwm)
{
  assert(pwm != NULL);
  assert(pwm->uspwm_enable_func != NULL);

  return pwm->uspwm_enable_func(pwm, USPWM_ENABLED);
}

/**
 * @brief Disable a pwm. This function will both stop the pwm,
 * and disable modifications to it, untill it is enabled.
 *
 * @param pwm The pwm to disable.
 *
 * @return A status code.
 */
int
us_pwm_disable(struct us_pwm_t *pwm)
{
  assert(pwm != NULL);
  assert(pwm->uspwm_disable_func != NULL);

  return pwm->uspwm_disable_func(pwm, USPWM_DISABLED);
}
