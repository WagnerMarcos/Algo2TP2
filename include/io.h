#ifndef IO_H
#define IO_H

#include "Complex.h"
#include "Vector.h"
#include "fourier.h"

bool load_signal(std::istream &, Vector<Complex<> > &);
bool print_signal(std::ostream &, Vector<Complex<> > const &);
void print_error_on_line(std::string line, size_t line_number);

#endif	// IO_H
