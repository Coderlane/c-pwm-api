/**
 * @file pwm.h
 * @brief 
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-05
 */

#ifndef USP_PWM_H
#define USP_PWM_H

struct us_pwm_controller_t;
struct us_pwm_t;

enum us_pwm_error_e {
	USP_OK = 0,
	USP_GENERIC_ERROR = 1
};

struct us_pwm_controller_t *us_pwm_controller_new();
void us_pwm_controller_delete(struct us_pwm_controller_t *usp_ctrl);

#endif /* USP_PWM_H */
