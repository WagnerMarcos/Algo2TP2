#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>

#include "process.h"

using namespace std;


bool
Process::validate_settings()
{
	if (!_input || !_input->good()) {
		cerr << "Could not open input file." << endl;
		return false;
	}
	if (!_output || !_output->good()) {
		cerr << "Could not open output file." << endl;
		return false;
	}
	if (_regression && !_regression->good()) {
		cerr << "Could not open regression file." << endl;
		return false;
	}
	if (!_transform) {
		cerr << "Could not set transform file." << endl;
		return false;
	}

	_output->setf(ios::fixed, ios::floatfield);
	_output->precision(6);

	return true;
}

bool
Process::run()
{
	if (!validate_settings())
		return false;

	ComplexVector inSignal;
	ComplexVector outSignal;
	ComplexVector regSignal;
	istringstream line;
	string s;
	bool status;
	long double error;

	for (int lineNo = 1; getline(*_input, s); ++lineNo) {
		
		if (_input->bad()) {
			print_error_on_line(line.str(), lineNo);
			return false;
		}
		
		line.str(s);
		line.clear(); // vacía los flags del istringstream
		
		status = load_signal(line, inSignal);
		
		if (!status) {
			print_error_on_line(line.str(), lineNo);
			return false;
		}
		
		outSignal.reserve(inSignal.size());
		status = _transform->compute(inSignal, outSignal);
		
		if (!status) {
			print_error_on_line(line.str(), lineNo);
			return false;
		}

		// si hay que procesar el archivo de regresiones
		if (_regression) {
			regSignal.reserve(outSignal.size());
			status = load_signal(*_regression, regSignal);
			error = relative_error(outSignal, regSignal);
			cout << "test "
			     << lineNo
			     << " ";
			if (error <= _errorTreshold) {
				cout << "ok";
			}
			else {
				cout << "error";
			}

			cout << " "
			     << outSignal.size()
			     << " "
			     << ios::scientific
			     << error
			     << endl;
		}
		else { // si no, imprimir la señal.
			status = print_signal(*_output, outSignal);
		}

		if (!status) {
			cerr << "Cannot write to output stream."
			     << endl;
			return false;
		}

		// vacía los vectores para reutilizarlos en el siguiente ciclo. 
		//
		inSignal.clear();
		outSignal.clear();
		regSignal.clear();
	}
	return true;
}

Process::~Process()
{
	if (_input != &cin)
		delete _input;
	if (_output != &cout)
		delete _output;
	delete _regression;
	delete _transform;
}

long double
Process::relative_error(ComplexVector const& result, ComplexVector const& regression)
{
	long double aux = 0;
	long double regNorm = 0;
	for(size_t i=0; i < result.size(); ++i) {
		aux = (result[i] - regression[i]).norm();
		aux *= aux; 
		regNorm += regression[i].norm();
	}
	return sqrt(aux / (regNorm * regNorm));
}

