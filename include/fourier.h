#ifndef FOURIER_H
#define FOURIER_H

#include <cmath>

#include "Complex.h"
#include "Vector.h"

using ComplexVector = Vector <Complex <long double> >;

class FourierAlgorithm {
public:
	virtual bool compute(ComplexVector const & input, ComplexVector & output) const = 0;
	virtual ~FourierAlgorithm() {}
};

class FourierTransform {
public:
	explicit FourierTransform(FourierAlgorithm *method) : _method(method) {}
	virtual ~FourierTransform() {
		delete _method;
	}
	FourierTransform() = delete;
	FourierTransform(FourierTransform const&) = delete;
	FourierTransform& operator=(FourierTransform const&) = delete;
	bool compute(ComplexVector const& input, ComplexVector& output) const {
		return _method? _method->compute(input, output) : false;
	}
private:
	FourierAlgorithm *_method;
};

class Discrete : public FourierAlgorithm {
public:
	bool compute(ComplexVector const& input, ComplexVector& output) const;
protected:
	virtual Complex<> _coefficient(int i, int j, int n) const = 0;
};

class DFT : public Discrete {
	Complex <> _coefficient(int i, int j, int n) const override {
		return exp(-I * 2.0 * M_PI * i * j / n);
	}
};

class IDFT : public Discrete {
	Complex <> _coefficient(int i, int j, int n) const override {
		return exp(I * 2.0 * M_PI * i * j / n) / n;
	}
};

class Fast : public FourierAlgorithm {
public:
	bool compute(ComplexVector const & input, ComplexVector & output) const;
protected:
	virtual Complex <> _coefficient(int k, int n) const = 0;
	virtual size_t _divisor(size_t n) const = 0;
private:
	ComplexVector _compute(ComplexVector const & input) const;
};

class FFT : public Fast {
	Complex <> _coefficient(int k, int n) const override {
		return exp(-I * 2.0 * M_PI * k / n);
	}
	size_t _divisor(size_t const) const override {
		return 1;
	}
};

class IFFT : public Fast {
	Complex <> _coefficient(int k, int n) const override {
		return exp(I * 2.0 * M_PI * k / n);
	}
	size_t _divisor(size_t n) const override {
		return n;
	}
};

inline ComplexVector& fill_until_power_of_two(ComplexVector& v) {
	size_t n = v.size();
	if (n & (n - 1)) { // si el tamaño no es una potencia de dos...
		while (n & (n - 1)) {
			v.push_back(0); // ...rellenar con ceros hasta que lo sea.
			++n;
		}
	}
	return v;
}


#endif	// FOURIER_H