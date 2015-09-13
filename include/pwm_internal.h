/**
 * @file pwm_internal.h
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-05
 */

#ifndef USP_PWM_INTERNAL_H
#define USP_PWM_INTERNAL_H

#include "ref_internal.h"

#define INT_STR_LEN 12

struct udev;
struct udev_device;

struct us_pwm_attr_match_t;
struct us_pwm_controller_t;
struct us_pwm_t;

enum us_pwm_attr_type_e {
  USPAT_SUBSYSTEM,
  USPAT_SYSATTR,
  USPAT_PROPERTY,
  USPAT_TAG,
  USPAT_SYSNAME
};

enum us_pwm_state_e { USPWM_DISABLED = 0, USPWM_ENABLED = 1 };

typedef int (*us_pwm_state_func_t)(struct us_pwm_t *, enum us_pwm_state_e);
typedef int (*us_pwm_set_float_func_t)(struct us_pwm_t*, float);
typedef int (*us_pwm_get_float_func_t)(struct us_pwm_t*, float *);
typedef int (*us_pwm_generic_func_t)(struct us_pwm_t *, void *);

struct us_pwm_attr_match_t {
  enum us_pwm_attr_type_e uspam_type;
  const char *uspam_key;
  const char *uspam_value;
};

struct us_pwm_controller_t {
  struct udev *uspc_udev;
};

struct us_pwm_t {
  USP_REF_PRIVATE
  struct udev_device *uspwm_device;
  us_pwm_state_func_t uspwm_enable_func;
  us_pwm_state_func_t uspwm_disable_func;
  us_pwm_set_float_func_t uspwm_set_duty_cycle_func;
  us_pwm_set_float_func_t uspwm_set_frequency_func;
  us_pwm_get_float_func_t uspwm_get_duty_cycle_func;
  us_pwm_get_float_func_t uspwm_get_frequency_func;

  void *uspwm_ctx;
};

struct us_pwm_attr_match_t *us_pwm_attr_match_new(enum us_pwm_attr_type_e type,
                                                  const char *key,
                                                  const char *value);
void us_pwm_attr_match_delete(struct us_pwm_attr_match_t *usp_attr_match);

struct us_pwm_t *us_pwm_new(struct udev_device *device);
void us_pwm_delete(void *ctx);

#endif /* USP_PWM_INTERNAL_H */
