// cmdline - procesamiento de opciones en la l�nea de comando.
//
// $Date: 2012/09/14 13:08:33 $
//
#include <string>
#include <cstdlib>
#include <iostream>

#include "cmdline.h"

using namespace std;

cmdline::cmdline()
{
}

cmdline::cmdline(option_t *table) : option_table(table)
{
	/* 
	- Lo mismo que hacer:

	option_table = table;

	Siendo "option_table" un atributo de la clase cmdline
	y table un puntero a objeto o struct de "option_t".
	
	Se estar�a contruyendo una instancia de la clase cmdline
	cargandole los datos que se hayan en table (la table con
	las opciones, ver el c�digo en main.cc)

	*/
}

void
cmdline::parse(int argc, char * const argv[])
{
#define END_OF_OPTIONS(p)       \
	((p)->short_name == 0   \
	 && (p)->long_name == 0 \
	 && (p)->parse == 0)
	for (option_t *op = option_table; !END_OF_OPTIONS(op); ++op)
		op->flags &= ~OPT_SEEN;
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] != '-') {
			cerr << "Invalid non-option argument: "
			     << argv[i]
			     << endl;
			exit(1);
		}
		if (argv[i][1] == '-'
		    && argv[i][2] == 0)
			break;
		if (argv[i][1] == '-')
			i += do_long_opt(&argv[i][2], argv[i + 1]);
		else
			i += do_short_opt(&argv[i][1], argv[i + 1]);
	}
	for (option_t *op = option_table; !END_OF_OPTIONS(op); ++op) {
#define OPTION_NAME(op) \
	(op->short_name ? op->short_name : op->long_name)
		if (op->flags & OPT_SEEN)
			continue;
		if (op->flags & OPT_MANDATORY) {
			cerr << "Option "
			     << "-"
			     << OPTION_NAME(op)
			     << " is mandatory."
			     << "\n";
			exit(1);
		}
		if (op->def_value == 0)
			continue;
		op->parse(string(op->def_value));
	}
}

int
cmdline::do_long_opt(const char *opt, const char *arg)
{
	for (option_t *op = option_table; op->long_name != 0; ++op) {
		if (string(opt) == string(op->long_name)) {
			op->flags |= OPT_SEEN;

			if (op->has_arg) {
				if (arg == 0) {
					cerr << "Option requires argument: "
					     << "--"
					     << opt
					     << "\n";
					exit(1);
				}
				op->parse(string(arg));
				return 1;
			} else {
				op->parse(string(""));
				return 0;
			}
		}
	}
	cerr << "Unknown option: "
	     << "--"
	     << opt
	     << "."
	     << endl;
	exit(1);
	// Algunos compiladores se quejan con funciones que 
	// l�gicamente no pueden terminar, y que no devuelven
	// un valor en esta �ltima parte.
	//
	return -1;
}

int
cmdline::do_short_opt(const char *opt, const char *arg)
{
	option_t *op;
	for (op = option_table; op->short_name != 0; ++op) {
		if (string(opt) == string(op->short_name)) {
			op->flags |= OPT_SEEN;
			if (op->has_arg) {
				if (arg == 0) {
					cerr << "Option requires argument: "
					     << "-"
					     << opt
					     << "\n";
					exit(1);
				}
				op->parse(string(arg));
				return 1;
			} else {
				op->parse(string(""));
				return 0;
			}
		}
	}
	cerr << "Unknown option: "
	     << "-"
	     << opt
	     << "."
	     << endl;
	exit(1);
	// Algunos compiladores se quejan con funciones que 
	// l�gicamente no pueden terminar, y que no devuelven
	// un valor en esta �ltima parte.
	//
	return -1;
}
