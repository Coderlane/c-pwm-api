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

static usp_pwm_search_func_t usp_controller_search_funcs[] = {odc1_search};

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

  ctrl->uspc_udev = udev_new();
  usp_ref_init(ctrl, usp_controller_delete);

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

  udev_unref(ctrl->uspc_udev);
  free(ctrl);
}

int
usp_controller_search(struct usp_controller_t *ctrl)
{
  for (int i = 0; i < USP_SEARCH_FUNC_COUNT; i++) {
    usp_controller_search_funcs[i](ctrl);
  }
  return USP_OK;
}

int
usp_controller_add_pwm(struct usp_controller_t *ctrl, struct usp_pwm_t *pwm)
{
  return usp_pwm_list_add(ctrl->uspc_dev_list, pwm);
}

struct usp_pwm_list_t *
usp_controller_get_pwms(struct usp_controller_t *ctrl)
{
  usp_pwm_list_ref(ctrl->uspc_dev_list);
  return ctrl->uspc_dev_list;
}
