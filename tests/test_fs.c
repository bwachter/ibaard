/**
 * @file test_fs.c
 * @author Bernd Wachter <bwachter-usenet@lart.info>
 * @date 2011
 */

#include <stdlib.h>
#include <sys/stat.h>

#include <ibaard_fs.h>

#include "tests.h"

static mode_t mode;
static mode_t modelist[]={S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP, // 660
                          S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, // 644
                          S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH, // 666
};
static int num_modes=3;

START_TEST(test_touch){
  struct stat tmpstat;
  char *name="test-run/touchtest";

  // test that we can create files, and don't fail on existing files
  for (int i=0;i<2;i++){
    touch(name);
    fail_unless(stat(name, &tmpstat)==0, NULL);
    fail_unless(S_ISREG(tmpstat.st_mode), NULL);
  }
}
END_TEST

START_TEST(test_mf2){
  struct stat tmpstat;
  char *name="test-run/mf2test";

  mf2(name, modelist[_i]);
  fail_unless(stat(name, &tmpstat)==0, NULL);
  fail_unless(S_ISFIFO(tmpstat.st_mode), NULL);
  fail_unless(modelist[_i]==(tmpstat.st_mode&modelist[_i]),
              "Expected %lo, but got %lo for mode", modelist[_i], tmpstat.st_mode&modelist[_i]);
}
END_TEST

START_TEST(test_mf2_exists){
  char *name="test-run/mf2exists";
  mode=0666;

  fail_unless(mknod(name, S_IFREG|mode, 0)==0, "Unable to set up test");
  fail_unless(mf2(name, mode)==-1, NULL);
}
END_TEST

START_TEST(test_mf2_error){
  char *name="/foo/bar/baz";
  mode=0666;

  fail_unless(mf2(name, mode)==-1, NULL);
}
END_TEST

START_TEST(test_mf){
  struct stat tmpstat;
  char *name="test-run/mftest";
  mode_t mask=umask(0000);
  umask(mask);
  mode=0666&~mask;

  mf(name);
  fail_unless(stat(name, &tmpstat)==0, NULL);
  fail_unless(S_ISFIFO(tmpstat.st_mode), NULL);
  fail_unless(mode==(tmpstat.st_mode&mode),
              "Expected %lo, but got %lo for mode", mode, tmpstat.st_mode&mode);
}
END_TEST

/* Test cases for testing objects in the filesystem */

START_TEST(test_tf_exists){
  char *name="test-run/tftest";
  mode=0666;

  fail_unless(mknod(name, S_IFREG|mode, 0)==0, "Unable to set up test");
  fail_unless(tf(name)==0, NULL);
}
END_TEST

START_TEST(test_tf_exists_but_unreadable){
  char *name="test-run/tftest_unreadable";
  mode=0000;

  fail_unless(mknod(name, S_IFREG|mode, 0)==0, "Unable to set up test");
  fail_unless(tf(name)==-1, NULL);
}
END_TEST

START_TEST(test_tf_does_not_exist){
  char *name="test-run/tftest_na";

  fail_unless(tf(name)!=0, NULL);
}
END_TEST

Suite *fs_suite(){
  Suite *s=suite_create("Filesystem related functions");

  TCase *tc_core=tcase_create("FIFO");
  tcase_add_test(tc_core, test_touch);
  tcase_add_loop_test(tc_core, test_mf2, 0, num_modes);
  tcase_add_test(tc_core, test_mf2_exists);
  tcase_add_test(tc_core, test_mf2_error);
  tcase_add_test(tc_core, test_mf);

  TCase *tc_tests=tcase_create("Tests");
  tcase_add_test(tc_tests, test_tf_does_not_exist);
  tcase_add_test(tc_tests, test_tf_exists);

  suite_add_tcase(s, tc_core);
  suite_add_tcase(s, tc_tests);

  return s;
}
