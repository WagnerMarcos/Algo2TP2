#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include "fourier.h"
#include "Vector.h"
#include "Complex.h"
#include "io.h"

class Process {
public:
	~Process();
	// Función que procesa los datos
	// según lo que haya sido configurado.
	// Devuelve false si algo sale mal o los
	// datos de la estructura de configuración
	// son inválidos.
	//
	bool run();

	// Getters
	//
	std::istream* input() const { return  _input; }
	std::ostream* output() const { return _output; }
	std::istream* regression() const { return _regression; }
	FourierTransform* transform() const { return _transform; }
	long double error_treshold() const { return _errorTreshold; }

	// Setters
	//
	std::istream* input(std::istream* in) {
		_input = in;
		return  _input;
	}
	std::ostream* output(std::ostream* out) {
		_output = out;
		return _output;
	}
	std::istream* regression(std::istream* reg) {
		_regression = reg;
		return _regression;
	}
	FourierTransform* transform(FourierTransform* t) { 
		_transform = t;
		return _transform;
	}
	long double error_treshold(double treshold) {
		_errorTreshold = treshold;
		return _errorTreshold;
	}

private:
	std::istream* _input = &std::cin;
	std::ostream* _output = &std::cout;
	std::istream* _regression = nullptr;
	FourierTransform* _transform = nullptr;
	long double _errorTreshold = 1e-3;
	bool validate_settings();
	// Función que calcula el error relativo
	// entre lo transformado y el archivo
	// de regresiones.
	//
	long double relative_error(ComplexVector const&, ComplexVector const&);

};

#endif // _PROCESS_H_INCLUDED_
