#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include "fourier.h"
#include "Vector.h"
#include "Complex.h"
#include "io.h"

class Process {
public:
	struct Configuration {
		std::istream* input = &std::cin;
		std::ostream* output = &std::cout;
		std::istream* regression = nullptr;
		FourierTransform* transform = nullptr;
		long double errorTreshold = 1e-3;
		~Configuration();
	};
	Process(Configuration const& config);
	bool run();
private:
	Configuration const& conf;
	void validate_settings();
	bool settingsAreValid;
};

#endif // _PROCESS_H_INCLUDED_
