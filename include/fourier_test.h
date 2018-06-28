#ifndef FOURIER_TEST_H
#define FOURIER_TEST_H

#include "cmdline.h"
#include "io.h"
#include "fourier.h"
#include "Complex.h"
#include "Vector.h"

#define DEFAULT_AMOUNT "5000"
#define DEFAULT_METHOD "fast"

static void opt_number(std::string const &);
static void opt_method(std::string const &);
static void opt_error(std::string const &);
static void opt_help(std::string const & = "");

#endif // FOURIER_TEST_H