#ifndef _IO_H_INCLUDED_
#define _IO_H_INCLUDED_

#include "Complex.h"
#include "Vector.h"
#include "fourier.h"

bool load_signal(std::istream &, Vector<Complex<> > &);
bool print_signal(std::ostream &, Vector<Complex<> > const &);
void print_error_on_line(std::string line, size_t line_number);

#endif	//_IO_H_INCLUDED_
