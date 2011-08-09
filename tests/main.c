#include <stdlib.h>
#include <sys/stat.h>

#include "tests.h"

START_TEST(test_main){
  struct stat tmpstat;
  char *name="test-run/empty";
  mode_t mode=0666;

  fail_if(stat(name, &tmpstat)==0, "Test directory not empty, tests will generate wrong results");
  mknod(name, S_IFREG|mode, 0);
}
END_TEST


Suite *main_suite(){
  Suite *s=suite_create("Main");

  TCase *tc_core=tcase_create("Core");
  tcase_add_test(tc_core, test_main);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void){
  int num_failed;
  Suite *s=main_suite();
  SRunner *sr=srunner_create (s);
  srunner_add_suite(sr, strip_suite());
  srunner_add_suite(sr, fs_suite());
  srunner_run_all (sr, CK_NORMAL);
  num_failed=srunner_ntests_failed(sr);
  srunner_free(sr);
  return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
