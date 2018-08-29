#pragma once

// Helps when we include this over and over again.
#ifndef PE_HELPERS
#define PE_HELPERS
#include <fstream>

namespace pe_helpers {

	void store_strings(std::string _input, std::ofstream& _out);

	void read_strings(std::string& _output, std::ifstream& _in);

}
#endif