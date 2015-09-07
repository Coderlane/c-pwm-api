/**
 * @file ref_internal.h
 * @brief 
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-06
 */

#include <stdatomic.h>
#include <assert.h>

typedef void (*usp_ref_free_t)(void *);

#define USP_REF_PRIVATE \
	atomic_uint 		usp_ref_count; \
	usp_ref_free_t 	usp_ref_free;

/**
 * @brief Initialize a new reference counting object. 
 *
 * @param ref The reference counting object to initialize.
 * @param ref_free The function used to free the reference counter.
 */
#define usp_ref_init(ref, ref_free) \
	assert(ref != NULL); \
	ref->usp_ref_count = 0; \
	ref->usp_ref_free = ref_free;

/**
 * @brief Incremente the reference on a reference counted object.
 *
 * @param ref The object to reference.
 */
#define usp_ref(ref) \
	assert(ref != NULL); \
	atomic_fetch_add(&(ref->usp_ref_count), 1);

/**
 * @brief Decrement the reference on a reference counted object.
 *
 * @param ref The object to unreference. If there are no reference
 * holders, the reference is dropped.
 */
#define usp_unref(ref) \
	assert(ref != NULL); \
	if (atomic_fetch_sub(&(ref->usp_ref_count), 1) == 0) \
		ref->usp_ref_free(ref);
