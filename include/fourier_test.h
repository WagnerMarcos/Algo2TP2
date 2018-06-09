#ifndef _FOURIER_TEST_H_INCLUDED_
#define _FOURIER_TEST_H_INCLUDED_

#include "cmdline.h"
#include "io.h"
#include "fourier.h"
#include "Complex.h"
#include "Vector.h"

#define DEFAULT_AMOUNT "5000"
#define DEFAULT_METHOD "fast"

static void opt_number(std::string const & arg);
static void opt_method(std::string const & arg);
static void opt_help(std::string const & arg = "");

#endif // _FOURIER_TEST_H_INCLUDED_