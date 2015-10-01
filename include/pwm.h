/**
 * @file pwm.h
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-05
 */

#ifndef USP_PWM_H
#define USP_PWM_H

#include <stdint.h>

struct usp_controller_t;
struct usp_pwm_list_t;
struct usp_pwm_t;

enum usp_pwm_error_e {
  USP_NOT_IMPLEMENTED = -64,
  USP_FILE_ERROR = -5,
  USP_IO_ERROR = -4,
  USP_INVALID_RANGE = -3,
  USP_INVALID_STATE = -2,
  USP_GENERIC_ERROR = -1,
  USP_OK = 0
};

/**
 * Controller Public
 */

struct usp_controller_t *usp_controller_new();
void usp_controller_delete(void *ctx);
struct usp_pwm_list_t *usp_controller_get_pwms(struct usp_controller_t *ctrl);

/**
 * PWM Public
 */

void usp_pwm_ref(struct usp_pwm_t *pwm);
void usp_pwm_unref(struct usp_pwm_t *pwm);

const char *usp_pwm_get_name(struct usp_pwm_t *pwm);

int usp_pwm_enable(struct usp_pwm_t *pwm);
int usp_pwm_disable(struct usp_pwm_t *pwm);

int usp_pwm_set_duty_cycle(struct usp_pwm_t *pwm, float duty_cycle);
// int usp_pwm_set_period(struct usp_pwm_t *pwm, float period);
int usp_pwm_set_frequency(struct usp_pwm_t *pwm, float frequency);
// int usp_pwm_set_polarity(struct usp_pwm_t *pwm, float polarity);

int usp_pwm_get_duty_cycle(struct usp_pwm_t *pwm, float *out_duty_cycle);
// int usp_pwm_get_period(struct usp_pwm_t *pwm, float *out_period);
int usp_pwm_get_frequency(struct usp_pwm_t *pwm, float *out_frequency);
// int usp_pwm_get_polarity(struct usp_pwm_t *pwm, float *out_polarity);

/**
 * List Public
 */

#define usp_pwm_list_foreach(list, entry)              \
  for (entry = usp_pwm_list_head(list); entry != NULL; \
       entry = usp_pwm_list_entry_next(entry))

#define usp_pwm_list_foreach_safe(list, entry, next)                           \
  for (entry = usp_pwm_list_head(list), next = usp_pwm_list_entry_next(entry); \
       entry != NULL; entry = next, next = usp_pwm_list_entry_next(entry))

void usp_pwm_list_ref(struct usp_pwm_list_t *list);
void usp_pwm_list_unref(struct usp_pwm_list_t *list);
struct usp_pwm_list_entry_t *
usp_pwm_list_entry_next(struct usp_pwm_list_entry_t *entry);
struct usp_pwm_list_entry_t *usp_pwm_list_head(struct usp_pwm_list_t *list);

#endif /* USP_PWM_H */
