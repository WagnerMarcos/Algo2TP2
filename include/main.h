#ifndef _MAIN_H_INCLUDED_
#define _MAIN_H_INCLUDED_

#include "cmdline.h"
#include "Complex.h"
#include "Vector.h"
#include "io.h"
#include "fourier.h"

static void opt_input(std::string const &);
static void opt_output(std::string const &);
static void opt_method(std::string const &);
static void opt_help(std::string const & = "");
FourierAlgorithm* choose_method(std::string read_method);

#endif	// _MAIN_H_INCLUDED_
