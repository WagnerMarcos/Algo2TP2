#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <complex>
#include <gtest/gtest.h>

#include "Complex_test.h"

#define TEST_UNARY_OP(OP) { \
	for (int i = 0; i < vectorSize; ++i) \
			EXPECT_EQ(OP(stdComplex[i]), myComplex[i].OP()); \
}

#define TEST_BINARY_OP(OP) { \
		for (int i = 0, j = vectorSize-1; i < vectorSize && j >= 0; ++i,--j) \
			EXPECT_EQ( \
				OP(stdComplex[i], stdComplex[j]), \
				myComplex[i].OP(myComplex[j]) \
			); \
}
using namespace std;

static char *program_name;
static int vectorSize;

static option_t options[] = {
	{1, "n", "number", DEFAULT_AMOUNT, opt_number, OPT_DEFAULT},
	{0, "h", "help", NULL, opt_help, OPT_DEFAULT},
	{0, nullptr, nullptr, nullptr, nullptr, 0}
};

void
opt_number(string const &arg)
{
	istringstream iss(arg);

	if (!(iss >> vectorSize) || !iss.eof()) {
		cerr << "Not a possible amount: "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
	if (iss.bad()) {
		cerr << "Cannot read amount."
		     << endl;
		exit(1);
	}
}

void
opt_help(string const &)
{
	cout << program_name
	     << " [-n amount]"
	     << endl;
	exit(0);
}

void
print_explanation()
{
	cerr << endl
	     << "Esta prueba crea dos vectores de "
	     << vectorSize
	     << " números complejos pseudo-aleatorios"
	     << "(la cantidad de elementos puede ser cambiada llamando "
	     << program_name
	     << " -n <cantidad>): uno de ellos usando la "
	     << "clase std::complex y el otro usando la clase Complex "  
	     << "diseñada para el TP."
	     << endl
	     << "El programa aplica las diversas funciones asociadas "
	     << "a la clase a cada número y compara lo obtenido "
	     << "con lo obtenido del llamado de las funciones análogas"
	     << " asociadas a la clase std::complex."
	     << endl;
}

// Definidos ad hoc para estas pruebas:
//
template <typename T> inline bool
operator==(complex <T> const & std, Complex <T> const & own)
{
	// La conversión está pues necesita usar la comparación con tolerancia,
	// la cual está definida como Complex::operator==()
	Complex <T> own_from_std(std.real(), std.imag());
	return own == own_from_std;
}

template <typename T> inline bool
operator==(Complex <T> const & own, complex <T> const & std)
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
			for (int i = 0; i < vectorSize; ++i) {
				long double randA = rand() * rand() * 10000;
				long double randB = rand() * rand() * 10000;
				stdComplex.push_back(complex <long double>(randA, randB));
				myComplex.push_back(Complex <long double>(randA, randB));
			}
		}
		vector <complex <long double> > stdComplex;
		vector <Complex <long double> > myComplex;
	};

	// Probar que Complex::Complex() haya funcionado correctamente
	TEST_F(ComplexTest, Constructor) {
		for (int i = 0; i < vectorSize; ++i)
			EXPECT_EQ(stdComplex[i], myComplex[i]);
	}

	// Probar que operator+() funcione
	TEST_F(ComplexTest, Addition) {
		TEST_BINARY_OP(operator+)
	}

	// Probar que operator-() funcione
	TEST_F(ComplexTest, Substraction) {
		TEST_BINARY_OP(operator-)
	}

	// Probar que operator*() funcione
	TEST_F(ComplexTest, Multiplication) {
		TEST_BINARY_OP(operator*)
	}

	// Probar que operator/() funcione
	TEST_F(ComplexTest, Division) {
		for (int i = 0, j = vectorSize-1; i < vectorSize && j >= 0; ++i, --j)
			if(stdComplex[j] != 0.0l && myComplex[j] != 0.0l)
				EXPECT_EQ(stdComplex[i] / stdComplex[j],
				          myComplex[i] / myComplex[j]);
	}

	// Probar que Complex::conj() funcione
	TEST_F(ComplexTest, Conjugate) {
		TEST_UNARY_OP(conj)
	}

	// Probar que Complex::norm() funcione
	TEST_F(ComplexTest, Norm) {
		for (int i = 0; i < vectorSize; ++i)
			EXPECT_DOUBLE_EQ(abs(stdComplex[i]), myComplex[i].norm());
	}

	// Probar que Complex::arg() funcione
	TEST_F(ComplexTest, Argument) {
		for (int i = 0; i < vectorSize; ++i)
			EXPECT_DOUBLE_EQ(arg(stdComplex[i]), myComplex[i].arg());
	}

	// Probar que exp() funcione
	TEST_F(ComplexTest, Exponential) {
		for (int i = 0; i < vectorSize; ++i)
			EXPECT_EQ(exp(stdComplex[i]), exp(myComplex[i]));
	}

	// Probar que la representación polar es el mismo número
	TEST_F(ComplexTest, Polar) {
		for (int i = 0; i < vectorSize; ++i)
			EXPECT_EQ(myComplex[i], Complex<long double>(myComplex[i].norm())
			          * exp(I * myComplex[i].arg()));
	}
}  // namespace

int
main(int argc, char **argv)
{
	program_name = argv[0];

	cmdline cmdl(options);
	cmdl.parse(argc, argv);

	print_explanation();

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
