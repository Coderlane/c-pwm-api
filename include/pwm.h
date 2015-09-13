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

struct us_pwm_controller_t;
struct us_pwm_t;

enum us_pwm_error_e {
  USP_INVALID_STATE = -1,
  USP_OK = 0,
  USP_GENERIC_ERROR = 1
};

struct us_pwm_controller_t *us_pwm_controller_new();
void us_pwm_controller_delete(struct us_pwm_controller_t *ctrl);

void us_pwm_ref(struct us_pwm_t *pwm);
void us_pwm_unref(struct us_pwm_t *pwm);

int us_pwm_enable(struct us_pwm_t *pwm);
int us_pwm_disable(struct us_pwm_t *pwm);

int us_pwn_set_duty_cycle(struct us_pwm_t *pwm, float duty_cycle);
// int us_pwn_set_period(struct us_pwm_t *pwm, float period);
int us_pwm_set_frequency(struct us_pwm_t *pwm, float frequency);
// int us_pwn_set_polarity(struct us_pwm_t *pwm, float polarity);

int us_pwn_get_duty_cycle(struct us_pwm_t *pwm, float *out_duty_cycle);
// int us_pwn_get_period(struct us_pwm_t *pwm, float *out_period);
int us_pwm_get_frequency(struct us_pwm_t *pwm, float *out_frequency);
// int us_pwn_get_polarity(struct us_pwm_t *pwm, float *out_polarity);

#endif /* USP_PWM_H */
