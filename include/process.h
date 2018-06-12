#ifndef _PROCESS_H_INCLUDED_
#define _PROCESS_H_INCLUDED_

#include "fourier.h"
#include "Vector.h"
#include "Complex.h"
#include "io.h"

class Process {
public:
	struct Configuration {
		// Estructura de configuración.
		// Los valores por defecto
		// se declaran acá.
		//
		std::istream* input = &std::cin;
		std::ostream* output = &std::cout;
		std::istream* regression = nullptr;
		FourierTransform* transform = nullptr;
		long double errorTreshold = 1e-3;

		// El destructor se encarga de llamar
		// a delete en todo lo que haya sido 
		// asignado con memoria dinámica.
		//
		~Configuration();
	};
	
	Process(Configuration const& config);

	// Función que procesa los datos
	// según lo que haya sido configurado.
	// Devuelve false si algo sale mal o los
	// datos de la estructura de configuración
	// son inválidos.
	//
	bool run();
private:
	Configuration const& conf;
	void validate_settings();
	bool settingsAreValid;
};

#endif // _PROCESS_H_INCLUDED_
