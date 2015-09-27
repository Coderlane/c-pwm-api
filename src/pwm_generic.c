/**
 * @file pwm_generic.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-05
 */

#include <libudev.h>

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "pwm_internal.h"
#include "pwm.h"

/**
 * @brief
 *
 * @param device
 *
 * @return
 */
struct usp_pwm_t *
usp_pwm_new(struct udev_device *device, enum usp_pwm_type_e type)
{
  struct usp_pwm_t *pwm = NULL;

  pwm = calloc(sizeof(struct usp_pwm_t), 1);
  assert(pwm != NULL);

  udev_device_ref(pwm->usppwm_device);
  pwm->usppwm_device = device;
  pwm->usppwm_type = type;
  uspp_ref_init(pwm, usp_pwm_delete);

  return pwm;
}

/**
 * @brief
 *
 * @param ctx
 */
void
usp_pwm_delete(void *ctx)
{
  struct usp_pwm_t *pwm = ctx;
  assert(pwm != NULL);
  assert(pwm->uspp_ref_count == 0);

  if (pwm->usppwm_device != NULL)
    udev_device_unref(pwm->usppwm_device);
  free(pwm);
}

/**
 * @brief Take a reference on a pwm.
 *
 * @param pwm The pwm to reference.
 */
void
usp_pwm_ref(struct usp_pwm_t *pwm)
{
  uspp_ref(pwm);
}

/**
 * @brief Unreference a pwm. If there are no more references
 * to the pwm, the device will be freed.
 *
 * @param pwm The pwm to unreference.
 */
void
usp_pwm_unref(struct usp_pwm_t *pwm)
{
  uspp_unref(pwm);
}

/**
 * @brief Enable a pwm. This function will enable the pwm for uspe,
 * but will not yet start the pwm.
 *
 * @param pwm The pwm to enable.
 *
 * @return A statusp code.
 */
int
usp_pwm_enable(struct usp_pwm_t *pwm)
{
  return pwm->usppwm_enable_func(pwm);
}

/**
 * @brief Disable a pwm. This function will both stop the pwm,
 * and disable modifications to it, untill it is enabled.
 *
 * @param pwm The pwm to disable.
 *
 * @return A statusp code.
 */
int
usp_pwm_disable(struct usp_pwm_t *pwm)
{
  return pwm->usppwm_disable_func(pwm);
}

/**
 * @brief Set the duty cycle of the pwm as a percentage from 0 to 100.
 *
 * @param pwm The pwm to set the duty cycle of.
 * @param duty_cycle The new duty cycle for the pwm.
 *
 * @return A statusp code.
 */
int
usp_pwm_set_duty_cycle(struct usp_pwm_t *pwm, float duty_cycle)
{
  if (duty_cycle < 0.0f || duty_cycle > 100.0f)
    return USP_INVALID_RANGE;
  return pwm->usppwm_set_duty_cycle_func(pwm, duty_cycle);
}

/**
 * @brief Set the frequency of the pwm in hertz.
 *
 * @param pwm The pwm to set the frequency of.
 * @param frequency The new frequency for the pwm.
 *
 * @return A statusp code.
 */
int
usp_pwm_set_frequency(struct usp_pwm_t *pwm, float frequency)
{
  if (frequency < 0.0f)
    return USP_INVALID_RANGE;
  return pwm->usppwm_set_duty_cycle_func(pwm, frequency);
}

/**
 * @brief Get the current duty cycle of the pwm as a percentage.
 *
 * @param pwm The pwm to get the duty_cycle of.
 * @param out_duty_cycle The current duty cycle of the pwm.
 *
 * @return A statusp code.
 */
int
usp_pwm_get_duty_cycle(struct usp_pwm_t *pwm, float *out_duty_cycle)
{
  assert(out_duty_cycle != NULL);
  return pwm->usppwm_get_duty_cycle_func(pwm, out_duty_cycle);
}

/**
 * @brief Get the current frequency of the PWM in hertz.
 *
 * @param pwm The pwm to get the frequency of.
 * @param out_frequency The current frequency of the pwm.
 *
 * @return A statusp code.
 */
int
usp_pwm_get_frequency(struct usp_pwm_t *pwm, float *out_frequency)
{
  assert(out_frequency != NULL);
  return pwm->usppwm_get_duty_cycle_func(pwm, out_frequency);
}
