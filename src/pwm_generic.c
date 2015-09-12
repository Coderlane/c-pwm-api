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

struct us_pwm_t *
us_pwm_new(us_pwm_state_func_t enable_func, us_pwm_state_func_t disable_func)
{
  struct us_pwm_t *us_pwm = NULL;
  us_pwm = malloc(sizeof(struct us_pwm_t));
  assert(us_pwm != NULL);

  us_pwm->uspwm_enable_func = enable_func;
  us_pwm->uspwm_disable_func = disable_func;

  usp_ref_init(us_pwm, us_pwm_delete);

  return us_pwm;
}

void
us_pwm_delete(void *ctx)
{
  struct us_pwm_t *us_pwm = ctx;
  assert(us_pwm != NULL);
  assert(us_pwm->usp_ref_count == 0);

  free(us_pwm);
}

void
us_pwm_ref(struct us_pwm_t *us_pwm)
{
  usp_ref(us_pwm);
}

void
us_pwm_unref(struct us_pwm_t *us_pwm)
{
  usp_unref(us_pwm);
}

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
