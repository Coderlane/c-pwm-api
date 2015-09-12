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
  struct us_pwm_controller_t *usp_ctrl = NULL;

  usp_ctrl = calloc(sizeof(struct us_pwm_controller_t), 1);
  assert(usp_ctrl != NULL);

  usp_ctrl->uspc_udev = udev_new();

  return usp_ctrl;
}

/**
 * @brief Delete a PWM controller and all attached PWMs.
 *
 * @param usp_ctrl The PWM controller to delete.
 */
void
us_pwm_controller_delete(struct us_pwm_controller_t *usp_ctrl)
{
  assert(usp_ctrl != NULL);

  udev_unref(usp_ctrl->uspc_udev);
  free(usp_ctrl);
}

/** 
 * Attribute Matching
 */

/**
 * @brief
 *
 * @param type
 * @param key
 * @param value
 *
 * @return
 */
struct us_pwm_attr_match_t *
us_pwm_attr_match_new(enum us_pwm_attr_type_e type, const char *key,
                      const char *value)
{
  struct us_pwm_attr_match_t *usp_attr_match = NULL;

  usp_attr_match = malloc(sizeof(struct us_pwm_attr_match_t));
  usp_attr_match->uspam_type = type;
  usp_attr_match->uspam_key = key;
  usp_attr_match->uspam_value = value;

  return usp_attr_match;
}

/**
 * @brief 
 *
 * @param usp_attr_match
 */
void
us_pwm_attr_match_delete(struct us_pwm_attr_match_t *usp_attr_match)
{
  free(usp_attr_match);
}

