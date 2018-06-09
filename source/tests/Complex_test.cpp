#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <complex>
#include <gtest/gtest.h>

#include "Complex_test.h"

static char *program_name;
static size_t vectorSize;

static option_t options[] = {
	{1, "n", "number", DEFAULT_AMOUNT, opt_number, OPT_DEFAULT},
	{0, "h", "help", NULL, opt_help, OPT_DEFAULT},
	{0, },
};

static void
opt_number(std::string const &arg)
{
	std::istringstream iss(arg);

	if (!(iss >> vectorSize) || !iss.eof()) {
		std::cerr << "Not a possible amount: "
		          << arg
		          << "."
		          << std::endl;
		exit(1);
	}
	if (iss.bad()) {
		std::cerr << "Cannot read amount."
		          << std::endl;
		exit(1);
	}
}

static void
opt_help(std::string const &arg)
{
	std::cout << program_name
	          << " [-n amount]"
	          << std::endl;
	exit(0);
}

// Definidos ad hoc para estas pruebas:
//
template <typename T> inline bool
operator==(std::complex <T> const & std, Complex <T> const & own)
{
	// la conversión está pues necesita usar la comparación con tolerancia,
	// definida como Complex::operator==()
	Complex <T> own_from_std(std.real(), std.imag());
	return own == own_from_std;
}

template <typename T> inline bool
operator==(Complex <T> const & own, std::complex <T> const & std)
{
	Complex <T> own_from_std(std.real(), std.imag());
	return own_from_std == own;
}

// Google Test exige que las pruebas estén en un namespace sin nombre
//
namespace {
	// Clase a reutilizar en múltiples pruebas con llamados a TEST_F()
	class ComplexTest : public ::testing::Test {
	protected:
		ComplexTest() : stdComplex(vectorSize), myComplex(vectorSize) {
			srand(time(NULL));
			for (size_t i = 0; i < vectorSize; ++i) {
				long double randA = rand() * rand() * 10000;
				long double randB = rand() * rand() * 10000;
				stdComplex.push_back(std::complex <long double>(randA, randB));
				myComplex.push_back(Complex <long double>(randA, randB));
			}
		}
		std::vector <std::complex <long double> > stdComplex;
		std::vector <Complex <long double> > myComplex;
	};

	// Probar que Complex::Complex() funcione correctamente
	TEST_F(ComplexTest, Constructor) {
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_EQ(stdComplex[i], myComplex[i]);
	}

	// Probar que operator+() funcione
	TEST_F(ComplexTest, Addition) {
		for (size_t i = 0, j = vectorSize -1; i < vectorSize && j >= 0; ++i, --j)
			EXPECT_EQ(stdComplex[i] + stdComplex[j], myComplex[i] + myComplex[j]);
	}

	// Probar que operator-() funcione
	TEST_F(ComplexTest, Substraction) {
		for (size_t i = 0, j = vectorSize -1; i < vectorSize && j >= 0; ++i, --j)
			EXPECT_EQ(stdComplex[i] - stdComplex[j], myComplex[i] - myComplex[j]);
	}

	// Probar que operator*() funcione
	TEST_F(ComplexTest, Multiplication) {
		for (size_t i = 0, j = vectorSize -1; i < vectorSize && j >= 0; ++i, --j)
			EXPECT_EQ(stdComplex[i] * stdComplex[j], myComplex[i] * myComplex[j]);
	}

	// Probar que operator/() funcione
	TEST_F(ComplexTest, Division) {
		for (size_t i = 0, j = vectorSize-1; i < vectorSize && j >= 0; ++i, --j)
			if(stdComplex[j] != 0.0l && myComplex[j] != 0.0l)
				EXPECT_EQ(stdComplex[i] / stdComplex[j],
				          myComplex[i] / myComplex[j]);
	}

	// Probar que Complex::conj() funcione
	TEST_F(ComplexTest, Conj) {
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_EQ(std::conj(stdComplex[i]), myComplex[i].conj());
	}

	// Probar que Complex::norm() funcione
	TEST_F(ComplexTest, Norm) {
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_DOUBLE_EQ(std::abs(stdComplex[i]), myComplex[i].norm());
	}

	// Probar que Complex::arg() funcione
	TEST_F(ComplexTest, Arg) {
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_DOUBLE_EQ(std::arg(stdComplex[i]), myComplex[i].arg());
	}

	// Probar que exp() funcione
	TEST_F(ComplexTest, Exp) {
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_EQ(std::exp(stdComplex[i]), exp(myComplex[i]));
	}

	// Probar que la representación polar es el mismo número
	TEST_F(ComplexTest, Polar) {
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_EQ(myComplex[i], Complex<long double>(myComplex[i].norm())
			          * exp(I * Complex<long double>(myComplex[i].arg())));
	}
}  // namespace

int main(int argc, char **argv) {
	program_name = argv[0];
	cmdline cmdl(options);
	cmdl.parse(argc, argv);

	std::cerr << "Esta prueba crea dos vectores de "
	          << vectorSize
	          << " números complejos pseudo-aleatorios"
	          << "(la cantidad de elementos puede ser cambiada llamando "
	          << program_name
	          << " -n <cantidad>): uno de ellos usando la "
	          << "clase std::complex y el otro usando la clase Complex "  
	          << "diseñada para el TP."
	          << std::endl
	          << "El programa aplica las diversas funciones asociadas "
	          << "a la clase a cada número y compara lo obtenido "
	          << "con lo obtenido del llamado de las funciones análogas"
	          << " asociadas a la clase std::complex."
		       << std::endl;

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
