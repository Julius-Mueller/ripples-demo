#pragma once

#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <array>

using coord_t = std::array<int, 2>;
using coord_vector_t = std::vector<coord_t>;
using time_in_seconds = int;

coord_vector_t get_indices(const std::string& coordinates_string);

class Teich
{
public:

	Teich() {
		colorize_();
	}

	~Teich() {
	}

	void print_image() const;

	void disturb(const coord_vector_t& coord_vector);

	void play(time_in_seconds playtime_in_seconds, bool show_stats = false);

private:
	inline static constexpr double dist_amplitude_ = -75;		// starting value for each disturbance
	inline static constexpr int x_len_ = 3 * 26;
	inline static constexpr int y_len_ = 2 * 10;
	inline static constexpr int n_osc_ = x_len_ * y_len_;			// (2*10) x (3*26) element grid
	double* amplitude_ = new double[n_osc_] {};
	char* image_ = new char[n_osc_];
	double* acceleration_ = new double[n_osc_] {};
	double* velocity_ = new double[n_osc_] {};
	double* amplitude_change_ = new double[n_osc_] {};
	inline static const std::map<int, char> colors_ = {			// char array with offset indices?
		{ -4, ' ' },
		{ -3, ' ' },
		{ -2, '-' },
		{ -1, '~' },
		{  0, '+' },
		{  1, 'o' },
		{  2, '#' },
		{  3, '%' },
		{  4, '@' }
	};

	void reset_();

	void colorize_();

	void advance_(double step_time, int n_nei = 5, double spring_rate = 5);

	double amplitude_sum_() const;
};

