
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <map>

#include "Teich.h"

int main()
{
	Teich pond = Teich();
	pond.print_image();

	std::string input_string;

	while (true) {
		std::cout << "Geben Sie einige Koordinaten ein (z.B. 'f3 k5 q8'), oder '0' um abzubrechen: ";
		std::getline(std::cin, input_string);

		if (input_string == "0")
			return 0;

		coord_vector_t input_vector = get_indices(input_string);
		pond.disturb(input_vector);
		pond.play(60, true);
	}
}

