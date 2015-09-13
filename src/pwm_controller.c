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

  return ctrl;
}

/**
 * @brief Delete a PWM controller and all attached PWMs.
 *
 * @param ctrl The PWM controller to delete.
 */
void
us_pwm_controller_delete(struct us_pwm_controller_t *ctrl)
{
  assert(ctrl != NULL);

  udev_unref(ctrl->uspc_udev);
  free(ctrl);
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
  struct us_pwm_attr_match_t *attr_match = NULL;
  attr_match = malloc(sizeof(struct us_pwm_attr_match_t));
  assert(attr_match != NULL);

  attr_match->uspam_type = type;
  attr_match->uspam_key = key;
  attr_match->uspam_value = value;

  return attr_match;
}

/**
 * @brief
 *
 * @param attr_match
 */
void
us_pwm_attr_match_delete(struct us_pwm_attr_match_t *attr_match)
{
  assert(attr_match != NULL);
  free(attr_match);
}
