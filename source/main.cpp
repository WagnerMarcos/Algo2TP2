#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "main.h"

using namespace std;

static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{1, "r", "regression", "-", opt_regression, OPT_DEFAULT},
	{1, "e", "error", "1e-3", opt_error, OPT_DEFAULT},
	{1, "m", "method", "FFT", opt_method, OPT_DEFAULT},
	{0, "h", "help", nullptr, opt_help, OPT_DEFAULT},
	{0, },
};

static char *program_name;
static Configuration config;

static void
opt_input(string const &arg)
{
	if (arg == "-") {
		config.input = &cin;
	}
	else {
		config.input = new ifstream(arg.c_str(), ios::in);
	}
}

static void
opt_output(string const &arg)
{
	if (arg == "-") {
		config.output = &cout;
	}
	else {
		config.output = new ofstream(arg.c_str(), ios::out);
	}
}

static void
opt_regression(string const &arg)
{
	if(arg == "-") {
		config.regression = nullptr;
	} 
	else {
		config.regression = new ifstream(arg.c_str(), ios::in);
	}
}

static void
opt_error(string const &arg)
{
	if(arg != "-" ) {
		char * p;
		config.errorTreshold = strtold(arg.c_str(), p);
		if (*p && *p == '\n') {
			return;
		}
		else {
			cerr << "Invalid error treshold "
			     << arg
			     << ". Using default value instead."
			     << endl;
		}
		config.errorTreshold = DEFAULT_ERROR_TRESHOLD;
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
	}

	chosen_method = choose_method(read_method);

	if (chosen_method == nullptr) {
		cerr << "Not a posible method: "
		     << arg
		     << "."
		     << endl;
		opt_help();
	}
	config.transform = new FourierTransform(chosen_method);
}

static void
opt_help(string const & arg)
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

	Process process(config);

	// Cuestiones de formato para la impresión:
	oss->setf(ios::fixed, ios::floatfield);
	oss->precision(6);

	bool status;
	status = process.run();

	return status;
}
