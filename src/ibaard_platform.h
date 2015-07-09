/*
 * ibaard_platform.h    -- Bernd Wachter <bwachter@lart.info>
 */

#ifndef _IBAARD_PLATFORM_H
#define _IBAARD_PLATFORM_H

/** @file
 * Define common macros for different compilers
 */

#ifdef _MSC_VER //FIXME: don't recent versions of msvc define WIN32?
#ifndef _WIN32
#define _WIN32
#endif
#endif

#if (defined(_WIN32)) && (defined(__BORLANDC__))
#define __WIN32BORLAND
#endif

#if (defined(_WIN32)) && (defined(_MSC_VER))
#define __WIN32MSC
#endif

#endif
