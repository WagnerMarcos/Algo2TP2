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
static FourierTransform *transform;
static istream *iss;
static ostream *oss;
static ifstream *rfs;
static fstream ifs;
static fstream ofs;


static void
opt_input(string const &arg)
{
	if (arg == "-") {
		iss = &cin;
	}
	else {
		ifs.open(arg.c_str(), ios::in);
		iss = &ifs;
	}

	if (!iss->good()) {
		cerr << "Cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void
opt_output(string const &arg)
{
	if (arg == "-") {
		oss = &cout;
	} else {
		ofs.open(arg.c_str(), ios::out);
		oss = &ofs;
	}

	if (!oss->good()) {
		cerr << "Cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void
opt_regression(string const &arg)
{
	if(arg == "-") {
		rfs = nullptr;
		return;
	} 
	
	rfs->open(arg.c_str(), ios::in);
	if (!rfs->good()) {
		cerr << "Cannot open "
		     << arg
		     << "."
		     << endl;
		exit(1);
	}
}

static void
opt_error(string const &arg)
{
	if(arg != "-" ) {
		Complex<long double>::acceptable_delta(
			strtold(arg.c_str(),nullptr)
		);
	}

	//Si no ponen valor a error, ya tiene su valor por defecto.

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
		exit(1);
	}

	chosen_method = choose_method( read_method );

	if (chosen_method == nullptr) {
		cerr << "Not a posible method: "
		     << arg
		     << "."
		     << endl;
		opt_help();
		exit(1);
	}
	::transform = new FourierTransform(chosen_method);
}

static void
opt_help(string const & arg)
{
	cout << "Usage: "
	     << program_name
	     << " [-m FFT | IFFT | DFT | IDFT] [-i file] [-o file]"
	     << endl;
	exit(0);
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

	// Cuestiones de formato para la impresión:
	oss->setf(ios::fixed, ios::floatfield);
	oss->precision(6);

	bool status = process(*::transform, *iss, *oss, rfs);

	delete ::transform;
	::transform = nullptr;

	return status;
}
