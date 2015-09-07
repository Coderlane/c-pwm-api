/* The MIT License (MIT)
 * Copyright (c) 2015 Travis Lane
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy 	of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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
