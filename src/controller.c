/**
 * @file controller.c
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
struct usp_controller_t *
usp_controller_new()
{
  struct usp_controller_t *ctrl = NULL;
  ctrl = calloc(sizeof(struct usp_controller_t), 1);
  assert(ctrl != NULL);

  ctrl->usppc_udev = udev_new();
  uspp_ref_init(ctrl, usp_controller_delete);

  return ctrl;
}

/**
 * @brief Delete a PWM controller and all attached PWMs.
 *
 * @param ctx The PWM controller to delete.
 */
void
usp_controller_delete(void *ctx)
{
  struct usp_controller_t *ctrl = ctx;
  assert(ctrl != NULL);

  udev_unref(ctrl->usppc_udev);
  free(ctrl);
}

