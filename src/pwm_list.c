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

/**
 * @brief Create a new list to hold pwms.
 *
 * @return A new empty list to hold pwms.
 */
struct us_pwm_list_t *
us_pwm_list_new()
{
  struct us_pwm_list_t *list = NULL;
  list = calloc(sizeof(struct us_pwm_list_t), 1);
  usp_ref_init(list, us_pwm_list_delete);
  return list;
}

/**
 * @brief Destroy the list of pwms. This simply decrements the refcount on
 * each of the items in the list by one. If something ref'd a pwm it will
 * still prevent the pwm from being free'd until its refcount goes to 0.
 *
 * @param ctx The list of pwms to delete.
 */
void
us_pwm_list_delete(void *ctx)
{
  struct us_pwm_list_t *list = ctx;
  struct us_pwm_t *pwm;
  PWM_FOREACH_UNSAFE(list, pwm) { us_pwm_unref(pwm); }
  list->uspl_count = 0;
  free(list);
}

/**
 * @brief Add a pwm to a list of pwms. Note: a pwm can only belong
 * to one list of pwms.
 *
 * @param list The list of pwms to add the pwm to.
 * @param pwm The pwm to add to the list of pwms.
 *
 * @return A status code.
 */
int
us_pwm_list_add(struct us_pwm_list_t *list, struct us_pwm_t *pwm)
{
  assert(list != NULL);
  assert(pwm != NULL);
  assert(pwm->uspwm_next == NULL);
  usp_ref(pwm);

  if(list->uspl_head != NULL) {
    pwm->uspwm_next = list->uspl_head;
  }

  list->uspl_head = pwm;

  return USP_OK;
}

/**
 * @brief Take a reference to a list of PWMs.
 *
 * @param list The list of pwms to take a reference to.
 */
void
us_pwm_list_ref(struct us_pwm_list_t *list)
{
  usp_ref(list);
}

/**
 * @brief Release a reference to a list of PWMs.
 *
 * @param list The list of pwms to release a reference to.
 */
void
us_pwm_list_unref(struct us_pwm_list_t *list)
{
  usp_unref(list);
}
