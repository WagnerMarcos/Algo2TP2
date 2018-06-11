#include <iostream>
#include <cstdlib>
#include <sstream>

#include "io.h"

using namespace std;

bool
load_signal(istream & is, Vector<Complex<> > & input)
{
	Complex <long double> c;
	while (is >> c)
		input.push_back(c);
	if (is.bad())
		return false;
	return true;
}

bool
print_signal(ostream & os, Vector<Complex<> > const & output)
{
	for (size_t i = 0; i < output.size(); ++i)
		os << output[i] << ' ';
	os << endl;
	if (os.bad())
		return false;
	return true;
}
