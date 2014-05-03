/**
 * @file test_strip.c
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2011
 */

#include <stdlib.h>

#include <ibaard_strip.h>

#include "tests.h"

START_TEST(test_stripn){
  char *unstripped_string="Foo\nbar\n\n";
  char *stripped_string;

  stripped_string=stripn(strdup(unstripped_string));
  fail_unless(strlen(stripped_string) == strlen(unstripped_string)-1, "Stripped string longer than expected");
  fail_unless(!strncmp(stripped_string, unstripped_string, strlen(stripped_string)), NULL);

  free(stripped_string);
}
END_TEST

START_TEST(test_striprn){
  char *unstripped_string="Foo\nbar\n\r\n";
  char *stripped_string;

  stripped_string=striprn(strdup(unstripped_string));
  fail_unless(strlen(stripped_string) == strlen(unstripped_string)-2, "Stripped string longer than expected");
  fail_unless(!strncmp(stripped_string, unstripped_string, strlen(stripped_string)), NULL);

  free(stripped_string);
}
END_TEST

START_TEST(test_stripa){
  char *unstripped_string="Foo\nbar\n\x01";
  char *stripped_string;

  stripped_string=stripa(strdup(unstripped_string));
  fail_unless(strlen(stripped_string) == strlen(unstripped_string)-1, NULL);
  fail_unless(!strncmp(stripped_string, unstripped_string, strlen(stripped_string)), NULL);

  free(stripped_string);
}
END_TEST

Suite *strip_suite(){
  Suite *s=suite_create("Strip characters off a string");

  TCase *tc_core=tcase_create("Core");
  tcase_add_test(tc_core, test_stripn);
  tcase_add_test(tc_core, test_striprn);
  tcase_add_test(tc_core, test_stripa);
  suite_add_tcase(s, tc_core);

  return s;
}
