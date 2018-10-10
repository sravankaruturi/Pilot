#include "SaveSceneHelpers.h"

namespace pe_helpers {


	void store_strings(std::string _input, std::ofstream& _out) {

		int number_of_characters = _input.length();
		const char * input_chars = _input.c_str();

		_out.write((char*)&number_of_characters, sizeof(int));
		_out.write((char*)input_chars, number_of_characters);

	}

	void read_strings(std::string& _output, std::ifstream& _in) {

		// Clear the existing string of any values.
		(_output).clear();

		// Load the String Length from the File.
		int string_length = 0;
		_in.read((char*)&string_length, sizeof(int));

		// Now Create a new Char array that you can read the Data into.
		char * temp_char_string = new char[string_length];

		// Read Data to this var.
		_in.read((char*)temp_char_string, string_length);

		(_output) = temp_char_string;

		_output.resize(string_length);

		delete[] temp_char_string;

	}

}