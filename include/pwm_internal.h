/**
 * @file pwm_internal.h
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-05
 */

#ifndef USP_PWM_INTERNAL_H
#define USP_PWM_INTERNAL_H

struct udev;

enum us_pwm_attr_type_e {
  USPAT_SUBSYSTEM,
  USPAT_SYSATTR,
  USPAT_PROPERTY,
  USPAT_TAG,
  USPAT_SYSNAME
};

struct us_pwm_attr_match_t {
  enum us_pwm_attr_type_e uspam_type;
  const char *uspam_key;
  const char *uspam_value;
};

struct us_pwm_controller_t {
  struct udev *uspc_udev;
};

struct us_pwm_t {

};

struct us_pwm_attr_match_t *us_pwm_attr_match_new(enum us_pwm_attr_type_e type,
                                                  const char *key,
                                                  const char *value);
void us_pwm_attr_match_delete(struct us_pwm_attr_match_t *usp_attr_match);

#endif /* USP_PWM_INTERNAL_H */
