#include <iostream>
#include <sstream>

#include "process.h"

using namespace std;

Configuration::~Configuration() {
	if (input != &cin)
		delete input;
	if (output != &cout)
		delete output;
	delete regression;
	delete transform;
}

Process::Process(Configuration const& config)  : conf(config) {
	configurate();
}

void Process::configurate() {
	if (!conf.input || !conf.input->good())
		cerr << "Could not open input file." << endl;
	if (!conf.output || !conf.output->good())
		cerr << "Could not open output file." << endl;
	if (conf.regression && !conf.regression->good())
		cerr << "Could not open regression file." << endl;
	conf.output->setf(ios::fixed, ios::floatfield);
	conf.output->precision(6);
}

bool
Process::run() {
	ComplexVector inSignal;
	ComplexVector outSignal;
	istringstream line;
	string s;
	bool status;

	for (int lineNo = 1; getline(*conf.input, s); ++lineNo) {
		
		if (conf.input->bad()) {
			cerr << "An error occured while processing line "
			     << lineNo
			     << "."
			     << endl;
			return false;
		}
		
		line.str(s);
		line.clear(); // vacía los flags del istringstream
		
		status = load_signal(line, inSignal);
		
		if (!status) {
			cerr << "Error processing \""
			     << line.str()
			     << "\" (line "
			     << lineNo
			     << ")."
			     << endl;
			return false;
		}

		status = conf.transform->compute(inSignal, outSignal);
		
		if (!status) {
			cerr << "An error occured while performing the requested operation."
			     << endl;
			return false;
		}
		
		status = print_signal(*conf.output, outSignal);
		
		if (!status) {
			cerr << "Cannot write to output stream."
			     << endl;
			return false;
		}

		// vacía los vectores para reutilizarlos en el siguiente ciclo. 

		inSignal.clear();
		outSignal.clear();
	}
	return true;
}
