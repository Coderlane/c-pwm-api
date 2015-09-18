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
  struct us_pwm_list_entry_t *entry;
  us_pwm_list_foreach(list, entry) { us_pwm_unref(entry->uspl_pwm); }
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
  struct us_pwm_list_entry_t *list_entry = NULL;
  assert(list != NULL);
  assert(pwm != NULL);
  assert(pwm->uspwm_next == NULL);

  list_entry = malloc(sizeof(struct us_pwm_list_entry_t));
  list_entry->uspl_pwm = pwm;
  usp_ref(pwm);

  if(list->uspl_head != NULL) {
    list_entry->uspl_next = list->uspl_head;
  }

  list->uspl_head = list_entry;

  return USP_OK;
}

/**
 * @brief Get the head of the list.
 *
 * @param list The list to get the head of.
 *
 * @return The head of the list, may be NULL.
 */
struct us_pwm_list_entry_t *
us_pwm_list_head(struct us_pwm_list_t *list)
{
  return list->uspl_head;
}

/**
 * @brief Get the next entry according to a list entry.
 *
 * @param entry The list entry to get the next entry of.
 *
 * @return The next entry in the list, NULL if we are at the end.
 */
struct us_pwm_list_entry_t *
us_pwm_list_entry_next(struct us_pwm_list_entry_t *entry)
{
  return entry->uspl_next;
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
