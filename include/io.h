#ifndef _IO_H_INCLUDED_
#define _IO_H_INCLUDED_

#include "Complex.h"
#include "Vector.h"
#include "fourier.h"

bool load_signal(std::istream &, Vector<Complex<> > &);
bool print_signal(std::ostream &, Vector<Complex<> > const &);
void print_msg_and_exit(std::string const & msg);
bool process(FourierTransform& transform, std::istream& is, std::ostream& os);

#endif	//_IO_H_INCLUDED_
