#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <limits>

#include "main.h"

using namespace std;

static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{1, "r", "regression", "-", opt_regression, OPT_DEFAULT},
	{1, "e", "error", "1e-3", opt_error, OPT_DEFAULT},
	{1, "m", "method", "FFT", opt_method, OPT_DEFAULT},
	{0, "h", "help", nullptr, opt_help, OPT_DEFAULT},
	{0, nullptr, nullptr, nullptr, nullptr, 0}
};

static char *program_name;

// Estructura de configuración del proceso.
// las distintas funciones llamadas por
// cmdline configurarán sus campos acorde
// a lo leído por línea de comandos.
// 
static Process process;

static void
opt_input(string const &arg)
{
	if (arg != "-") {
		process.input(new ifstream(arg.c_str(), ios::in));
	}
}

static void
opt_output(string const &arg)
{
	if (arg != "-") {
		process.output(new ofstream(arg.c_str(), ios::out));
	}
}

static void
opt_regression(string const &arg)
{
	if(arg != "-") {
		process.regression(new ifstream(arg.c_str(), ios::in));
	}
}

static void
opt_error(string const &arg)
{
	if(arg != "-" ) {
		long double read_value;
		char * nextChar;
		read_value = strtold(arg.c_str(), &nextChar);
		if (nextChar && *nextChar == '\0') { // si fue válido
			process.error_treshold(read_value);
		}
		else {
			process.error_treshold(numeric_limits<long double>::infinity());
		}
	}
}

static void
opt_method(string const &arg)
{
	FourierAlgorithm *chosen_method;
	istringstream iss(arg);
	string read_method;

	if (!(iss >> read_method) || iss.bad()) {
		cerr << "Cannot read method."
		     << endl;
		return;
	}

	chosen_method = choose_method(read_method);

	if (chosen_method == nullptr) {
		cerr << "Not a posible method: "
		     << arg
		     << "."
		     << endl;
		opt_help();
		return;
	}
	process.transform(new FourierTransform(chosen_method));
}

static void
opt_help(string const &)
{
	cout << "Usage: "
	     << program_name
	     << " [-m FFT | IFFT | DFT | IDFT] [-i file] [-o file]"
	     << endl;
}

FourierAlgorithm *
choose_method (string read_method)
{
	if (read_method == "FFT")
		return new FFT;
	if (read_method == "IFFT")
		return new IFFT;
	if (read_method == "DFT")
		return new DFT;
	if (read_method == "IDFT")
		return new IDFT;
	return nullptr;
}

int
main(int argc, char * const argv[])
{
	program_name = argv[0];

	cmdline cmdl(options);
	cmdl.parse(argc, argv);

	bool status;
	status = process.run();

	return !status;
}
