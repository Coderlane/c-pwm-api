/**
 * @file pwm_controller.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-11
 */

#include <libudev.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "pwm_internal.h"
#include "pwm.h"

/**
 * Controller
 */

/**
 * @brief Create a new PWM controller to collect individual PWMs.
 *
 * @return A new PWM controller.
 */
struct us_pwm_controller_t *
us_pwm_controller_new()
{
  struct us_pwm_controller_t *ctrl = NULL;
  ctrl = calloc(sizeof(struct us_pwm_controller_t), 1);
  assert(ctrl != NULL);

  ctrl->uspc_udev = udev_new();
  usp_ref_init(ctrl, us_pwm_controller_delete);

  return ctrl;
}

/**
 * @brief Delete a PWM controller and all attached PWMs.
 *
 * @param ctx The PWM controller to delete.
 */
void
us_pwm_controller_delete(void *ctx)
{
  struct us_pwm_controller_t *ctrl = ctx;
  assert(ctrl != NULL);

  udev_unref(ctrl->uspc_udev);
  free(ctrl);
}

