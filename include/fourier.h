#ifndef _FOURIER_H_INCLUDED_
#define  _FOURIER_H_INCLUDED_

#include "Complex.h"
#include "Vector.h"

using ComplexVector = Vector <Complex <long double> >;

class FourierAlgorithm {
public:
	virtual bool compute(ComplexVector const & input, ComplexVector & output) = 0;
	virtual ~FourierAlgorithm() {}
};

class FourierTransform {
public:
	FourierTransform(FourierAlgorithm *method) : _method(method) {}
	virtual ~FourierTransform() {
		delete _method;
	}
	bool compute(ComplexVector const & input, ComplexVector & output) {
		return _method? _method->compute(input, output) : false;
	}
private:
	FourierAlgorithm *_method;
};

class Discrete : public FourierAlgorithm {
public:
	bool compute(ComplexVector const & input, ComplexVector & output);
protected:
	virtual const Complex<> _coefficient(int const i, int const j, int const n) = 0;
};

class DFT : public Discrete {
	const Complex <> _coefficient(int const i, int const j, int const n) override {
		return exp(-I * 2.0 * M_PI * i * j / n);
	}
};

class IDFT : public Discrete {
	const Complex <> _coefficient(int const i, int const j, int const n) override {
		return exp(I * 2.0 * M_PI * i * j / n) / n;
	}
};

class Fast : public FourierAlgorithm {
public:
	bool compute(ComplexVector const & input, ComplexVector & output);
protected:
	virtual const Complex <> _coefficient(int const k, int const n) = 0;
	virtual const size_t _divisor(size_t const n) = 0;
private:
	ComplexVector _compute(ComplexVector const & input);
};

class FFT : public Fast {
	const Complex <> _coefficient(int const k, int const n) override {
		return exp(-I * 2.0 * M_PI * k / n);
	}
	const size_t _divisor(size_t const n) {
		return 1;
	}
};

class IFFT : public Fast {
	const Complex <> _coefficient(int const k, int const n) override {
		return exp(I * 2.0 * M_PI * k / n);
	}
	const size_t _divisor(size_t const n) {
		return n;
	}
};


#endif	// _FOURIER_H_INCLUDED_