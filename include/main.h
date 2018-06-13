#ifndef _MAIN_H_INCLUDED_
#define _MAIN_H_INCLUDED_

#include "cmdline.h"
#include "fourier.h"
#include "process.h"

static void opt_input(std::string const &);
static void opt_output(std::string const &);
static void opt_regression(std::string const &);
static void opt_error(std::string const &);
static void opt_method(std::string const &);
static void opt_help(std::string const & = "");
FourierAlgorithm* choose_method(std::string read_method);

#endif	// _MAIN_H_INCLUDED_
