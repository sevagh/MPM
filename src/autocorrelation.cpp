#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <complex>
#include <pitch_detection.hpp>

extern "C" {
#include <xcorr.h>
}

static double get_acf_periodicity(std::vector<double> data)
{
	int size = data.size();
	int total_peak_bin_index = 0;
	int occurences = 0;

	for (int i = 1; i < size-1; i++) {
		if ((data[i] > data[i-1]) && (data[i]) > data[i+1]) {
			occurences += 1;
			total_peak_bin_index = i;
		}
	}

	return (double) (total_peak_bin_index) / (double) occurences;
}

double get_pitch_autocorrelation(std::vector<double> data, int sample_rate)
{
	int size = signed(data.size());
	int size2 = 2*size-1;

	std::vector<std::complex<double>> acf_complex(size2);
	std::vector<double> acf_real{};

	xcorr_fftw_r2c(&data[0], &data[0], &acf_complex[0], size);

	for (auto elem : acf_complex)
		acf_real.push_back(elem.real());

	double peak_bin_index_periodicity = get_acf_periodicity(acf_real);

	return (sample_rate/peak_bin_index_periodicity);
}
