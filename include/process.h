#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include "fourier.h"
#include "Vector.h"
#include "Complex.h"
#include "io.h"

#define DEFAULT_ERROR_TRESHOLD 1e-3

struct Configuration {
	std::istream* input;
	std::ostream* output;
	std::istream* regression;
	FourierTransform* transform;
	long double errorTreshold;
	~Configuration();
};

class Process {
	Configuration const& conf;
	void configurate();
public:
	Process(Configuration const& config);
	bool run();
};

#endif // _PROCESS_H_INCLUDED_
