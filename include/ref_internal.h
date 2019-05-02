/**
 * @file ref_internal.h
 * @brief
 * @author Travis Lane
 * @version 0.0.2
 * @date 2015-09-06
 */

#ifndef USP_REF_INTERNAL_H
#define USP_REF_INTERNAL_H

#include <stdio.h>
#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>

typedef void (*usp_ref_delete_t)(void *);

#define USP_REF_PRIVATE      \
  atomic_uint usp_ref_count; \
  usp_ref_delete_t usp_ref_delete;

/**
 * @brief Initialize a new reference counting object. Gives the ref
 * one starting reference.
 *
 * @param ref The reference counting object to initialize.
 * @param ref_delete The function usped to delete the reference counter.
 */
#define usp_ref_init(ref, ref_delete) \
  (ref)->usp_ref_count = 1;           \
  (ref)->usp_ref_delete = ref_delete;

/**
 * @brief Increment the reference on a reference counted object.
 *
 * @param ref The object to reference.
 */
#define usp_ref(ref) atomic_fetch_add(&(ref->usp_ref_count), 1);

/**
 * @brief Decrement the reference on a reference counted object.
 *
 * @param ref The object to unreference. If there are no reference
 * holders, the object is destroyed.
 */
#define usp_unref(ref)                                   \
  if (atomic_fetch_sub(&((ref)->usp_ref_count), 1) == 1) \
    (ref)->usp_ref_delete((ref));

#endif /* USP_REF_INTERNAL_H */
