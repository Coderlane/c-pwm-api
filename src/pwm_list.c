/**
 * @file pwm_list.c
 * @brief
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-13
 */

#include <libudev.h>

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "pwm_internal.h"
#include "pwm.h"

struct us_pwm_list_t *
us_pwm_list_new(struct us_pwm_controller_t *ctrl)
{
  struct us_pwm_list_t *list = NULL;
  list = calloc(sizeof(struct us_pwm_list_t), 1);
  usp_ref_init(list, us_pwm_list_delete);
  list->uspl_ctrl = ctrl;
  return list;
}

void
us_pwm_list_delete(void *ctx)
{
  struct us_pwm_list_t *list = ctx;
  struct us_pwm_t *pwm;
  PWM_FOREACH_UNSAFE(list, pwm) { us_pwm_unref(pwm); }
  list->uspl_count = 0;
  free(list);
}
