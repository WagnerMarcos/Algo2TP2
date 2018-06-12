#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>

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

void
print_msg(string const & msg)
{
	cerr << msg
	     << endl;
	exit(1);
}

bool
process(FourierTransform& transform, istream& is, ostream& os, ifstream *rfs)
{
	ComplexVector inSignal;
	ComplexVector outSignal;
	istringstream line;
	string s;
	bool status;

	for (int lineNo = 1; getline(is, s); ++lineNo) {
		if (is.bad()) {
			print_msg("An error occured while processing line " + to_string(lineNo) + ".");
			return false;
		}
		
		line.clear(); // vacía los flags del istringstream
		line.str(s);

		status = load_signal(line, inSignal);
		if (!status) {
			print_msg("Error processing \"" + line.str() + "\" (line " + to_string(lineNo) + ").");
			return false;
		}

		status = transform.compute(inSignal, outSignal);
		if (!status) {
			print_msg("An error occured while performing the requested operation.");
			return false;
		}

		status = regression(outSignal, rfs);

		status = print_signal(os, outSignal);
		if (!status) {
			print_msg("Cannot write to output stream.");
			return false;
		}

		// vacía los vectores para reutilizarlos en el siguiente ciclo. 

		inSignal.clear();
		outSignal.clear();
	}
	return true;
}

bool
regression(ComplexVector outputVector, ifstream *rfs)
{
	string s;
	istringstream line;

	Complex <long double> regressionComplex;
	Complex <long double> aux;
	long double aux1=0;
	long double aux2=0;

	long double regressionError;

	getline((*rfs), s);

	line.str(s);


	for(size_t i; i < outputVector.size(); ++i) {
		
		line >> regressionComplex;
		aux= outputVector[i] - regressionComplex; 

		aux1 += aux.norm() ;
		aux1 *= aux1 ;
		aux2 += regressionComplex.norm() ;
		aux2 *= aux2 ;



	}

	regressionError += sqrt( aux1/aux2 );

	cout << regressionError << endl;

	return true;
}




