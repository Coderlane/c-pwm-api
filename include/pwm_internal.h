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

struct udev;
struct udev_device;

struct us_pwm_controller_t;
struct us_pwm_list_t;
struct us_pwm_list_entry_t;
struct us_pwm_t;

/**
 * Controller
 */

struct us_pwm_controller_t {
  USP_REF_PRIVATE
  struct udev *uspc_udev;
};

/**
 * PWM
 */

enum us_pwm_state_e { USPWM_DISABLED = 0, USPWM_ENABLED = 1 };
enum us_pwm_type_e { USPWM_ODC1, USPWM_TEST };

typedef int (*us_pwm_set_float_func_t)(struct us_pwm_t *, float);
typedef int (*us_pwm_get_float_func_t)(struct us_pwm_t *, float *);
typedef int (*us_pwm_generic_func_t)(struct us_pwm_t *);
typedef int (*us_pwm_generic_void_func_t)(struct us_pwm_t *, void *);

struct us_pwm_t {
  USP_REF_PRIVATE

  struct udev_device *uspwm_device;

  us_pwm_generic_func_t uspwm_enable_func;
  us_pwm_generic_func_t uspwm_disable_func;
  us_pwm_set_float_func_t uspwm_set_duty_cycle_func;
  us_pwm_set_float_func_t uspwm_set_frequency_func;
  us_pwm_get_float_func_t uspwm_get_duty_cycle_func;
  us_pwm_get_float_func_t uspwm_get_frequency_func;

  enum us_pwm_type_e uspwm_type;
  void *uspwm_ctx;
  struct us_pwm_t *uspwm_next;
};

struct us_pwm_t *us_pwm_new(struct udev_device *device,
                            enum us_pwm_type_e type);
void us_pwm_delete(void *ctx);

/**
 * List
 */

struct us_pwm_list_t {
  USP_REF_PRIVATE
  struct us_pwm_list_entry_t *uspl_head;
  struct us_pwm_controller_t *uspl_ctrl;
  uint32_t uspl_count;
};

struct us_pwm_list_entry_t {
  struct us_pwm_list_entry_t *uspl_next;
  struct us_pwm_t *uspl_pwm;
};

struct us_pwm_list_t *us_pwm_list_new();
void us_pwm_list_delete(void *ctx);
int us_pwm_list_add(struct us_pwm_list_t *list, struct us_pwm_t *pwm);

#endif /* USP_PWM_INTERNAL_H */
