#ifndef _COMPLEX_H_INCLUDED_
#define _COMPLEX_H_INCLUDED_

#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>

// Función para la comparación con margen de error:
template<typename T>
inline const bool almostEqual(T a, T b);

template <typename T = long double>
class Complex {
	T x;
	T y;
public:
	Complex(T real = 0, T imag = 0) : x(real), y(imag) {}
	Complex(const Complex& C) : x(C.x), y(C.y) {}
	~Complex() {}

	T re() const { return x; }
	T im() const { return y; }

	const Complex conj() const {
		return Complex(x, -y);
	}
	const T norm() const {
		return sqrt(x*x + y*y);
	}
	const T arg() const {
		return std::atan2(y,x);
	}
	const Complex operator+() const {
		return Complex(+x, +y);
	}
	const Complex operator-() const {
		return Complex(-x, -y);
	}
	const Complex operator+(const Complex& c) const {
		return Complex(x + c.x, y + c.y);
	}
	const Complex operator-(const Complex& c) const {
		return Complex(x - c.x, y - c.y);
	}
	const Complex operator*(const Complex& c) const {
		return Complex(x*c.x - y*c.y, y*c.x + x*c.y);
	}
	const Complex operator/(const Complex& c) const {
		return Complex((x*c.x + y*c.y) / (c.x*c.x + c.y*c.y),
		               (y*c.x - x*c.y) / (c.x*c.x + c.y*c.y));
	}
	Complex& operator=(const Complex& c) {
		x = c.x;
		y = c.y;
		return *this;
	}
	Complex& operator+=(const Complex& c) {
		x += c.x;
		y += c.y;
		return *this;
	}
	Complex& operator-=(const Complex& c) {
		x -= c.x;
		y -= c.y;
		return *this;
	}
	Complex& operator*=(const Complex& c) {
		x = x*c.x - y*c.y;
		y = y*c.x + x*c.y;
		return *this;
	}
	Complex& operator/=(const Complex& c) {
		x = (x*c.x + y*c.y) / (c.x*c.x + c.y*c.y);
		y = (y*c.x - x*c.y) / (c.x*c.x + c.y*c.y);
		return *this;
	}
	bool operator==(const Complex & c) const {
		return almostEqual(x, c.x) && almostEqual(y, c.y);
	}
	bool operator!=(const Complex& c) const {
		return !almostEqual(x, c.x) || !almostEqual(y, c.y);
	}
	friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
		return os << '('
				    << c.x
				    << ','
				    << ' '
				    << c.y
				    << ')';
	}
	friend std::istream& operator>>(std::istream& is, Complex& c) {
		bool good = false;
		bool bad = false;
		T re = 0;
		T im = 0;
		char ch;
		if (is >> ch && ch == '(') {
			if (is >> re
			&& is >> ch
			&& ch == ','
			&& is >> im
			&& is >> ch
			&& ch == ')')
				good = true;
			else
				bad = true;
		} 
		else if (is.good()) {
			is.putback(ch);
			if (is >> re)
				good = true;
			else
				bad = true;
		}
		if (good) {
			c.x = re;
			c.y = im;
		}
		else if (bad)
			is.setstate(std::ios::badbit);
		return is;
	}
};

const Complex <long double> I(0, 1);
const long double Complex_acceptableDelta = 10e-6;

template <typename T> Complex <T>
exp(const Complex <T> & c)
{
	return typename Complex<T>::Complex( std::exp (c.re()) * std::cos(c.im()),
	       std::exp(c.re()) * std::sin(c.im()));
}

// Función para la comparación que comprueba si dos números difieren en lo
// suficientemente poco.
//
template <typename T> inline const bool 
almostEqual(T a, T b)
{
	const T absA = std::abs(a);
	const T absB = std::abs(b);
	const T absDelta = std::abs(a - b);
	const bool deltaIsAcceptable = absDelta <= Complex_acceptableDelta;

	if (a == b) // si son iguales, incluso inf
		return true;
	// si a o b son cero, o están lo suficientemente cerca
	//
	if (a == 0 || b == 0 || deltaIsAcceptable)
		return true;
	// sino, usar el error relativo
	return Complex_acceptableDelta >
	       absDelta / std::min<T>(absA + absB, std::numeric_limits<T>::max());
}

#endif	//_COMPLEX_H_INCLUDED_
