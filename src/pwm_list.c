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
struct usp_pwm_list_t *
usp_pwm_list_new()
{
  struct usp_pwm_list_t *list = NULL;
  list = malloc(sizeof(struct usp_pwm_list_t));
  list->usppl_count = 0;
  list->usppl_ctrl = NULL;
  list->usppl_head = NULL;
  uspp_ref_init(list, usp_pwm_list_delete);
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
usp_pwm_list_delete(void *ctx)
{
  struct usp_pwm_list_t *list = ctx;
  struct usp_pwm_list_entry_t *entry, *next;
  usp_pwm_list_foreach_safe(list, entry, next) {
    usp_pwm_unref(entry->usppl_pwm);
    free(entry);
  }
  free(list);
}

/**
 * @brief Add a pwm to a list of pwms. Note: a pwm can only belong
 * to one list of pwms.
 *
 * @param list The list of pwms to add the pwm to.
 * @param pwm The pwm to add to the list of pwms.
 *
 * @return A statusp code.
 */
int
usp_pwm_list_add(struct usp_pwm_list_t *list, struct usp_pwm_t *pwm)
{
  struct usp_pwm_list_entry_t *list_entry = NULL;
  assert(list != NULL);
  assert(pwm != NULL);

  list_entry = malloc(sizeof(struct usp_pwm_list_entry_t));
  list_entry->usppl_pwm = pwm;
  uspp_ref(pwm);

  list->usppl_count++;

  list_entry->usppl_next = list->usppl_head;
  list->usppl_head = list_entry;
  return USP_OK;
}

/**
 * @brief Get the head of the list.
 *
 * @param list The list to get the head of.
 *
 * @return The head of the list, may be NULL.
 */
struct usp_pwm_list_entry_t *
usp_pwm_list_head(struct usp_pwm_list_t *list)
{
  return list->usppl_head;
}

/**
 * @brief Get the next entry according to a list entry.
 *
 * @param entry The list entry to get the next entry of.
 *
 * @return The next entry in the list, NULL if we are at the end.
 */
struct usp_pwm_list_entry_t *
usp_pwm_list_entry_next(struct usp_pwm_list_entry_t *entry)
{
  return entry == NULL ? NULL : entry->usppl_next;
}

/**
 * @brief Take a reference to a list of PWMs.
 *
 * @param list The list of pwms to take a reference to.
 */
void
usp_pwm_list_ref(struct usp_pwm_list_t *list)
{
  uspp_ref(list);
}

/**
 * @brief Release a reference to a list of PWMs.
 *
 * @param list The list of pwms to release a reference to.
 */
void
usp_pwm_list_unref(struct usp_pwm_list_t *list)
{
  uspp_unref(list);
}
