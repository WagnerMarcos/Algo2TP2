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
		cerr << "Error: Could not open input file." << endl;
		return false;
	}
	if (!_output || !_output->good()) {
		cerr << "Error: Could not open output file." << endl;
		return false;
	}
	if (_regression && !_regression->good()) {
		cerr << "Error: Could not open regression file." << endl;
		return false;
	}
	if (!_transform) {
		cerr << "Error: Could not set chosen transform." << endl;
		return false;
	}
	if (_errorTreshold == numeric_limits<long double>::infinity()) {
		cerr << "Error: Could not set error treshold." << endl;
		return false;
	}
	_output->precision(6);
	return true;
}

bool
Process::run()
{
	if (!validate_settings())
		return false;

	string s;
	bool status;
	bool aTestFailed = false;
	long double error;

	for (int lineNo = 1; getline(*_input, s); ++lineNo) {
		istringstream line(s);
		
		if (_input->bad()) {
			print_error_on_line(line.str(), lineNo);
			return false;
		}
		ComplexVector inSignal;
		status = load_signal(line, inSignal);
		if (!status) {
			print_error_on_line(line.str(), lineNo);
			return false;
		}

		ComplexVector outSignal;
		status = _transform->compute(fill_until_power_of_two(inSignal), outSignal);
		if (!status) {
			print_error_on_line(line.str(), lineNo);
			return false;
		}

		// Si hay que procesar el archivo de regresiones
		// se lee de este y se compara, calculando el
		// error relativo, con la transformada obtenida.
		//
		if (_regression) {
			getline(*_regression, s);
			if (_input->bad()) {
				print_error_on_line(line.str(), lineNo);
				return false;
			}
			line.str(s);
			line.clear();
			ComplexVector regSignal;
			status = load_signal(line, regSignal);
			if (!status) {
				print_error_on_line(line.str(), lineNo);
				return false;
			}
			error = relative_error(outSignal, regSignal);
			status = error <= _errorTreshold;
			if (!status)
				aTestFailed = true;
			_output->setf(ios::scientific, ios::floatfield);
			*_output << "test "
			         << lineNo
			         << (status ? " ok " : " error ")
			         << outSignal.size()
			         << " "
			         << error
			         << endl;

		}
		// Si no hay archivo de regresiones, imprimir
		// la transformada.
		//
		else {
			_output->setf(ios::fixed, ios::floatfield);
			status = print_signal(*_output, outSignal);
			if (!status) {
				cerr << "Cannot write to output stream."
				     << endl;
				return false;
			}
		}
	} // fin del ciclo for
	return aTestFailed; // si una prueba falló, devuelve true
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
	long double outputSquaredNormSum = 0;
	long double regSquaredNormSum = 0;
	for(size_t i = 0; i < result.size(); ++i) {
		outputSquaredNormSum += ((result[i] - regression[i]).norm() * (result[i] - regression[i]).norm());
		regSquaredNormSum += (regression[i].norm() * regression[i].norm());
	}
	return sqrt( outputSquaredNormSum / regSquaredNormSum);
}

