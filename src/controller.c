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
  /* Do this first. */
  ctrl->uspc_initialized = false;

  ctrl->uspc_udev = udev_new();
  ctrl->uspc_dev_list = usp_pwm_list_new();

  /* Leave this for last. */
  ctrl->uspc_initialized = true;
  return ctrl;
}

/**
 * @brief Delete a PWM controller and all attached PWMs.
 *
 * @param ctx The PWM controller to delete.
 */
void
usp_controller_delete(struct usp_controller_t *ctrl)
{
  udev_unref(ctrl->uspc_udev);
  usp_pwm_list_unref(ctrl->uspc_dev_list);
  free(ctrl);
}

/**
 * @brief Search for all avaliable pwms. This should not be
 * done after initialization.
 *
 * @param ctrl The controller to add the pwms to.
 *
 * @return A status code.
 */
int
usp_controller_search(struct usp_controller_t *ctrl)
{
  assert(ctrl->uspc_initialized == false);
  for (int i = 0; i < USP_SEARCH_FUNC_COUNT; i++) {
    usp_controller_search_funcs[i](ctrl);
  }
  return USP_OK;
}

/**
 * @brief Add a pwm to a controller. This should not be done
 * after initialization.
 *
 * @param ctrl The controller to add a pwm to.
 * @param pwm The pwm to add to teh controller.
 *
 * @return A status code.
 */
int
usp_controller_add_pwm(struct usp_controller_t *ctrl, struct usp_pwm_t *pwm)
{
  assert(ctrl->uspc_initialized == false);
  return usp_pwm_list_add(ctrl->uspc_dev_list, pwm);
}

/**
 * @brief Get a list of pwms from the controller. This will
 * reference this list of pwms. Don't forget to unreference it.
 *
 * @param ctrl The controller to get a list of pwms from.
 *
 * @return A list of pwms.
 */
struct usp_pwm_list_t *
usp_controller_get_pwms(struct usp_controller_t *ctrl)
{
  assert(ctrl->uspc_initialized == true);
  usp_pwm_list_ref(ctrl->uspc_dev_list);
  return ctrl->uspc_dev_list;
}
