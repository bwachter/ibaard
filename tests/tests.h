/**
 * @file tests.h
 * @author Bernd Wachter <bwachter-usenet@lart.info>
 * @date 2011
 */

#ifndef _TESTS_H
#define _TESTS_H

#include <check.h>

#define TESTRUNDIR "test-run"

Suite *strip_suite();
Suite *fs_suite();

#endif
