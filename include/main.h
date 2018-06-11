#ifndef _MAIN_H_INCLUDED_
#define _MAIN_H_INCLUDED_

#include "cmdline.h"
#include "Complex.h"
#include "Vector.h"
#include "io.h"
#include "fourier.h"
#include "process.h"

#define DEFAULT_ERROR_TRESHOLD 1e-3

static void opt_input(std::string const &);
static void opt_output(std::string const &);
static void opt_regression(std::string const &);
static void opt_error(std::string const &);
static void opt_method(std::string const &);
static void opt_help(std::string const & = "");
FourierAlgorithm* choose_method(std::string read_method);

#endif	// _MAIN_H_INCLUDED_
