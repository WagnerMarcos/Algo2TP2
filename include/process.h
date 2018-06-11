#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include "fourier.h"

struct Configuration {
	istream* input;
	ostream* output;
	istream* regression;
	FourierTransform* transform;
	long double errorTreshold;
};

class Process {
	config_t const& conf;
	void configurate();
public:
	Process(config_t const& config);
	bool run();
}

#endif // _PROCESS_H_INCLUDED_
