/* The MIT License (MIT)
 * Copyright (c) 2015 Travis Lane
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
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
 * @file test_ref.c
 * @brief 
 * @author Travis Lane
 * @version 0.0.1
 * @date 2015-09-06
 */

#include <check.h>

#include "ref_internal.h"

struct test_ref_t {
	USP_REF_PRIVATE
	int tr_should_free;
};

void 
ref_free(void *arg)
{
	struct test_ref_t *ref = arg;
	
	fail_if(ref->tr_should_free == 0, 
			"ref freed, when I shouldn't have been.");
}

START_TEST (test_ref_sf)
{
	struct test_ref_t ref, *ref_ptr = &ref;
	usp_ref_init(ref_ptr, ref_free);
	ref.tr_should_free = 1;

	fail_if(ref.usp_ref_count != 0, "ref not initialized to zero.");

	usp_ref(ref_ptr);
	usp_ref(ref_ptr);
	fail_if(ref.usp_ref_count != 2, "ref not incremented properly.");
	
	usp_unref(ref_ptr);
	usp_unref(ref_ptr);

	fail_if(ref.usp_ref_count != 0, "ref not decremented properly.");
}
END_TEST

START_TEST (test_ref_snf)
{
	struct test_ref_t ref, *ref_ptr = &ref;
	usp_ref_init(ref_ptr, ref_free);
	ref.tr_should_free = 0;

	fail_if(ref.usp_ref_count != 0, "ref not initialized to zero.");

	usp_ref(ref_ptr);
	usp_ref(ref_ptr);
	fail_if(ref.usp_ref_count != 2, "ref not incremented properly.");
	
	usp_unref(ref_ptr);
	fail_if(ref.usp_ref_count != 1, "ref not decremented properly.");
}
END_TEST

Suite* 
suite_ref_new()
{
        Suite *suite_ref = suite_create("suite_ref");
        TCase *case_ref_sf = tcase_create("test_ref_should_free");
        TCase *case_ref_snf = tcase_create("test_ref_should_not_free");
        tcase_add_test(case_ref_sf, test_ref_sf);
        tcase_add_test(case_ref_snf, test_ref_snf);
        suite_add_tcase(suite_ref, case_ref_sf);
        suite_add_tcase(suite_ref, case_ref_snf);
        return suite_ref;
}

int 
main (int argc, char **argv)
{
        int failed;
        Suite *suite_ref = suite_ref_new();
        SRunner *runner_ref = srunner_create(suite_ref);
        srunner_run_all(runner_ref, CK_NORMAL);
        failed = srunner_ntests_failed(runner_ref);
        srunner_free(runner_ref);
        return failed;
}
