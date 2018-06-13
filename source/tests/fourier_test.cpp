#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <gtest/gtest.h>

#include "fourier_test.h"

using ComplexVector = Vector <Complex <long double> >;

using namespace std;

static char *program_name;
static size_t vectorSize;
static const size_t file_amount = 28;
static FourierTransform* ft;
static FourierTransform* ift;
static FourierAlgorithm* chosen_method;
static FourierAlgorithm* chosen_inverse_method;
static const string test_files[file_amount] = {
	"testfiles/Frecuencia1.txt",
	"testfiles/TFrecuencia1.txt",
	"testfiles/Frecuencia1B.txt",
	"testfiles/TFrecuencia1B.txt",
	"testfiles/Frecuencia2.txt",
	"testfiles/TFrecuencia2.txt",
	"testfiles/Frecuencia2B.txt",
	"testfiles/TFrecuencia2B.txt",
	"testfiles/Frecuencia3.txt",
	"testfiles/TFrecuencia3.txt",
	"testfiles/Frecuencia3B.txt",
	"testfiles/TFrecuencia3B.txt",
	"testfiles/Frecuencia4.txt",
	"testfiles/TFrecuencia4.txt",
	"testfiles/Frecuencia4B.txt",
	"testfiles/TFrecuencia4B.txt",
	"testfiles/Frecuencia5.txt",
	"testfiles/TFrecuencia5.txt",
	"testfiles/Frecuencia5B.txt",
	"testfiles/TFrecuencia5B.txt",
	"testfilees/Pulso.txt",
	"testfilees/TPulso.txt",
	"testfilees/PulsoB.txt",
	"testfilees/TPulsoB.txt",
	"testfilees/dwavfs11025.txt",
	"testfilees/Tdwavfs11025.txt",
	"testfilees/gwavfs11025.txt",
	"testfilees/Tgwavfs11025.txt"
};

static option_t options[] = {
	{1, "n", "number", DEFAULT_AMOUNT, opt_number, OPT_DEFAULT},
	{1, "m", "method", DEFAULT_METHOD, opt_method, OPT_DEFAULT},
	{0, "h", "help", nullptr, opt_help, OPT_DEFAULT},
	{0, nullptr, nullptr, nullptr, nullptr, 0}
};

static void
opt_number(std::string const & arg)
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
opt_method(std::string const & arg)
{
	if (arg == "fast") {
		chosen_method = new FFT;
		chosen_inverse_method = new IFFT;
	}
	else if (arg == "discrete") {
		chosen_method = new DFT;
		chosen_inverse_method = new IDFT;
	}
	else {
		std::cerr << "Not a possible method: "
		          << arg
		          << "."
		          << std::endl;
		opt_help();
		exit(1);
	}
}

static void
opt_help(std::string const &)
{
	std::cout << "Usage: "
	          << program_name
	          << " [-n amount]"
	          << " [-m fast | discrete]"
	          << std::endl;
	exit(0);
}

static void
print_explanation()
{
	cout << "En la primera prueba se crea un vector de "
	     << vectorSize
	     << " números complejos pseudo-aleatorios "
	     << "(la cantidad de elementos puede ser cambiada llamando "
	     << program_name
	     << " -n <cantidad>). "
	     << endl
	     << "Luego se le aplica la transformada elegida "
	     << "(por defecto se usan la FFT e IFFT; "
	     << "la transformada a utilizar puede  ser cambiada llamando "
	     << program_name
	     << " -m <fast|discrete>), y al vector resultante se le aplica "
	     << "la antitransformada."
	     << endl
	     << "Por último, se comprueba que el vector original y "
	     << "la antitransformada de su transformada sean iguales."
	     << endl;

	cout << "En la segunda prueba se lee vectores y sus transformadas "
	     << "de archivos y luego se los compara a los valores obtenidos "
	     << "al aplicarles la transformada elegida."
	     << endl
	     << "Se considera que dos números son iguales si "
	     << "el módulo de su diferencia es menor o igual a "
	     << Complex<long double>::acceptable_delta()
	     << "."
	     << endl;
}

// Google Test exige que las pruebas estén en un namespace sin nombre
//
namespace {
	class RandomVectors : public ::testing::Test {
	protected:
		RandomVectors() : OrigVector(vectorSize),
		                  FTVector(vectorSize),
		                  FinalVector(vectorSize)
		{
			srand(time(NULL));
			for (size_t i = 0; i < vectorSize; ++i) {
				long double randA = rand() * rand() * 10000;
				long double randB = rand() * rand() * 10000;
				OrigVector.push_back(Complex <long double>(randA, randB));
			}
		}
		~RandomVectors() {
		}
		ComplexVector OrigVector;
		ComplexVector FTVector;
		ComplexVector FinalVector;
	};

	class VectorsFromFiles : public ::testing::Test {
	protected:
		VectorsFromFiles() : i(0) {
		}
		void read_vectors_from_files() {
			ifs.open(test_files[i], ios::in);
			if (!load_signal(ifs, originalVector))
				exit(1);
			ifs.close();
			++i;
			ifs.open(test_files[i], ios::in);
			if (!load_signal(ifs, transformedVector))
				exit(1);
			ifs.close();
			++i;
		}
		~VectorsFromFiles() {
		}
		size_t i;
		ifstream ifs;
		ComplexVector originalVector;
		ComplexVector transformedVector;
		ComplexVector FTOutput;
		ComplexVector IFTOutput;
	};

	TEST_F(RandomVectors, FTandIFT) {
		ft->compute(OrigVector, FTVector);
		ift->compute(FTVector, FinalVector);
		for (size_t i = 0; i < vectorSize; ++i)
			EXPECT_EQ(OrigVector[i], FinalVector[i]);
	}

	TEST_F(VectorsFromFiles, FTandIFT) {
		while (i < file_amount) {
			read_vectors_from_files();	
			if (!ft->compute(originalVector, FTOutput))
				exit(1);
			for (size_t j = 0 ; j < originalVector.size(); ++j)
				EXPECT_EQ(transformedVector[j], FTOutput[j]);
			if (!ift->compute(transformedVector, IFTOutput))
				exit(1);
			for (size_t j = 0 ; j < originalVector.size(); ++j)
				EXPECT_EQ(originalVector[j], IFTOutput[j]);
			transformedVector.clear();
			originalVector.clear();
			FTOutput.clear();
			IFTOutput.clear();
		}
	}

}  // namespace

int
main(int argc, char **argv)
{
	program_name = argv[0];
	cmdline cmdl(options);
	cmdl.parse(argc, argv);

	int test_result;

	::testing::InitGoogleTest(&argc, argv);

	ft = new FourierTransform(chosen_method);
	ift = new FourierTransform(chosen_inverse_method);
	
	print_explanation();

	test_result = RUN_ALL_TESTS();
	
	delete ft;
	delete ift;
	return test_result;
}