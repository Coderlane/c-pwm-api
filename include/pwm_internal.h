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

struct usp_controller_t;
struct usp_pwm_list_t;
struct usp_pwm_list_entry_t;
struct usp_pwm_t;

/**
 * Controller
 */



/* Search Functions */

typedef int (*usp_pwm_search_func_t)(struct usp_controller_t *);
int odc1_search(struct usp_controller_t *ctrl);

#define USP_SEARCH_FUNC_COUNT 1

int usp_controller_search(struct usp_controller_t *ctrl);
int usp_controller_add_pwm(struct usp_controller_t *ctrl,
                           struct usp_pwm_t *pwm);

struct usp_controller_t {
  USP_REF_PRIVATE

  struct usp_pwm_list_t *uspc_dev_list;
  struct udev *uspc_udev;
};

/**
 * PWM
 */

enum usp_pwm_state_e { USPWM_DISABLED = 0, USPWM_ENABLED = 1 };
enum usp_pwm_type_e { USPWM_ODC1, USPWM_TEST };

typedef int (*usp_pwm_set_float_func_t)(struct usp_pwm_t *, float);
typedef int (*usp_pwm_get_float_func_t)(struct usp_pwm_t *, float *);
typedef int (*usp_pwm_generic_func_t)(struct usp_pwm_t *);
typedef int (*usp_pwm_generic_void_func_t)(struct usp_pwm_t *, void *);

struct usp_pwm_t {
  USP_REF_PRIVATE

  struct udev_device *uspwm_device;

  usp_pwm_generic_func_t uspwm_enable_func;
  usp_pwm_generic_func_t uspwm_disable_func;
  usp_pwm_set_float_func_t uspwm_set_duty_cycle_func;
  usp_pwm_set_float_func_t uspwm_set_frequency_func;
  usp_pwm_get_float_func_t uspwm_get_duty_cycle_func;
  usp_pwm_get_float_func_t uspwm_get_frequency_func;

  enum usp_pwm_type_e uspwm_type;
  void *uspwm_ctx;
  struct usp_pwm_t *uspwm_next;
};

struct usp_pwm_t *usp_pwm_new(struct udev_device *device,
                              enum usp_pwm_type_e type);
void usp_pwm_delete(void *ctx);

/**
 * List
 */

struct usp_pwm_list_t {
  USP_REF_PRIVATE
  struct usp_pwm_list_entry_t *uspl_head;
  struct usp_controller_t *uspl_ctrl;
  uint32_t uspl_count;
};

struct usp_pwm_list_entry_t {
  struct usp_pwm_list_entry_t *uspl_next;
  struct usp_pwm_t *uspl_pwm;
};

struct usp_pwm_list_t *usp_pwm_list_new();
void usp_pwm_list_delete(void *ctx);
int usp_pwm_list_add(struct usp_pwm_list_t *list, struct usp_pwm_t *pwm);

/**
 * sysfs
 */
int sysfs_read_attr_str(const char *path, char *buff, size_t buff_size,
                        ssize_t *out_len);
int sysfs_write_attr_str(const char *path, const char *buff, size_t buff_len,
                         ssize_t *out_len);
int sysfs_read_attr_int(const char *path, int *data);
int sysfs_write_attr_int(const char *path, int data);
int sysfs_read_attr_float(const char *path, float *data);
int sysfs_write_attr_float(const char *path, float data);

#endif /* USP_PWM_INTERNAL_H */
