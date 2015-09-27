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
  int tr_freed;
};

void
ref_free(void *arg)
{
  struct test_ref_t *ref = arg;
  ref->tr_freed = 1;
  fail_if(ref->tr_should_free == 0, "ref freed, when I shouldn't have been.");
}

START_TEST(test_ref_sf)
{
  struct test_ref_t ref, *ref_ptr = &ref;
  usp_ref_init(ref_ptr, ref_free);
  ref.tr_should_free = 1;
  ref.tr_freed = 0;

  fail_if(ref.usp_ref_count != 1, "ref not initialized to one.");

  usp_ref(ref_ptr);
  fail_if(ref.usp_ref_count != 2, "ref not incremented properly.");

  usp_unref(ref_ptr);
  fail_if(ref.usp_ref_count != 1, "ref not decremented properly.");
  usp_unref(ref_ptr);
  fail_if(ref.usp_ref_count != 0, "ref not decremented properly.");
  fail_if(ref.tr_freed == 0, "ref not freed when it should have been.");
}
END_TEST

START_TEST(test_ref_snf)
{
  struct test_ref_t ref, *ref_ptr = &ref;
  usp_ref_init(ref_ptr, ref_free);
  ref.tr_should_free = 0;
  ref.tr_freed = 0;

  fail_if(ref.usp_ref_count != 1, "ref not initialized to one.");

  usp_ref(ref_ptr);
  usp_ref(ref_ptr);
  fail_if(ref.usp_ref_count != 3, "ref not incremented properly.");

  usp_unref(ref_ptr);
  fail_if(ref.usp_ref_count != 2, "ref not decremented properly.");
  fail_if(ref.tr_freed == 1, "ref freed when it should not have been.");
}
END_TEST

Suite *
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
main()
{
  int failed;
  Suite *suite_ref = suite_ref_new();
  SRunner *runner_ref = srunner_create(suite_ref);
  srunner_run_all(runner_ref, CK_NORMAL);
  failed = srunner_ntests_failed(runner_ref);
  srunner_free(runner_ref);
  return failed;
}
