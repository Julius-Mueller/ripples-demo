#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>

#include "Teich.h"

coord_vector_t get_indices(const std::string& coordinates_string)
{
	std::string xy;
	coord_t coord;
	coord_vector_t coord_vector;

	size_t len = coordinates_string.length();
	for (size_t i = 0; 3 * i < len; i++) {
		xy = coordinates_string.substr(3 * i, 2);
		coord[0] = 3 * (xy[0] - 'a');
		coord[1] = 2 * std::stoi(xy.substr(1, 1));
		coord_vector.push_back(coord);
	}

	return coord_vector;
}

void Teich::reset_() {
	for (int i = 0; i < n_osc_; i++) {
		amplitude_[i] = acceleration_[i] = velocity_[i] = amplitude_change_[i] = 0;
	}
}

void Teich::colorize_() {

	for (int i = 0; i < n_osc_; i++) {
		double color_float = amplitude_[i];
		int color_integer = (int) std::round(color_float);

		if (color_integer > 4)
			color_integer = 4;

		if (color_integer < -4)
			color_integer = -4;

		image_[i] = colors_.at(color_integer);
	}
}

void Teich::print_image() const {
	std::cout << "\033[0;0H";	// move cursor to position 0;0 and overwrite from there
	for (int i = 0; i < n_osc_; i++) {
		int x = i % (3 * 26);
		int y = i / (3 * 26);

		if (x == 0)

			if (y % 2 == 1)		// Ticks on every other line
				std::cout << "    ";
			else
				std::cout << y / 2 << "   ";
		std::cout << image_[i];

		if (x == (3 * 26 - 1))
			std::cout << std::endl;
	};
	std::cout << std::endl;
	std::cout << "    a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z" << std::endl;
	std::cout << std::endl;
}

void Teich::disturb(const coord_vector_t& coord_vector) {
	reset_();

	for (size_t i = 0; i < coord_vector.size(); i++) {
		int x = (coord_vector[i])[0];
		int y = (coord_vector[i])[1];

		amplitude_[x + (x_len_ * y)] = dist_amplitude_;
	}

	colorize_();
}

void Teich::advance_(double step_time, int n_nei, double spring_rate) {
	double mass_osc = 1.0;

	for (int i = 0; i < n_osc_; i++) {
		double total_force = 0.0;

		// Iterating over nearest n_nei neighbors (per dimension and direction)
		for (int y_rel = -n_nei; y_rel <= n_nei; y_rel++) {

			for (int x_rel = -n_nei; x_rel <= n_nei; x_rel++) {
				int x_i = i % x_len_;
				int y_i = i / x_len_;
				int x_abs = x_i + x_rel;
				int y_abs = y_i + y_rel;
				double r = std::sqrt(std::pow(x_rel, 2) + std::pow(y_rel, 2));

				// Check if coordinates are in bounds, r > 0, and calculate total force
				if (x_abs >= 0 && x_abs < 3 * 26 && y_abs >= 0 && y_abs < 2 * 10 && r > 0)
					total_force -= spring_rate * (amplitude_[i] - amplitude_[i + x_rel + 3 * 26 * y_rel]) / std::pow(r, 2);
			}
		}
		
		// Calculate resulting change in amplitude
		acceleration_[i] = total_force / mass_osc;
		velocity_[i] = acceleration_[i] * step_time + velocity_[i];
		amplitude_change_[i] = 0.5 * velocity_[i] * step_time;
	}
	// Apply change in amplitude
	for (int i = 0; i < n_osc_; i++)
		amplitude_[i] = amplitude_[i] + amplitude_change_[i];

	colorize_();
}

void Teich::play(time_in_seconds play_time, bool show_stats) {
	auto strt_time = std::chrono::steady_clock::now();
	auto prev_time = std::chrono::steady_clock::now();
	auto curr_time = std::chrono::steady_clock::now();
	time_in_seconds elap_time = 0;

	do {
		// Sleep for a moment
		// std::this_thread::sleep_for(std::chrono::milliseconds(16));

		print_image();
		curr_time = std::chrono::steady_clock::now();
		double step_time = std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - prev_time).count() / 1000.0;
		prev_time = curr_time;
		elap_time = (time_in_seconds) std::chrono::duration_cast<std::chrono::seconds>(curr_time - strt_time).count();

		if (show_stats) {
			std::cout << "\033[0K";		// Clear previous text in line
			std::cout << "    timer: -" << (play_time - elap_time) << "\t" << "fps: " << (int)(1 / step_time) << " ";
		}

		advance_(step_time);

	} while (elap_time < play_time);

	std::cout << "\033[1M";		// Delete 1 line (current line)
}

double Teich::amplitude_sum_() const {
	double s = 0;
	for (int i = 0; i < n_osc_; i++)
		s = s + amplitude_[i];
	return s;
}