#include "pitch_detection.h"
#include "pitch_detection_priv.h"
#include <complex>
#include <tuple>
#include <vector>

namespace
{
namespace yin_consts
{
template <typename T> static const T Threshold = static_cast<T>(0.20);
}
} // namespace

template <typename T>
static int
absolute_threshold(const std::vector<T> &yin_buffer)
{
	ssize_t size = yin_buffer.size();
	int tau;
	for (tau = 2; tau < size; tau++) {
		if (yin_buffer[tau] < yin_consts::Threshold<T>) {
			while (tau + 1 < size && yin_buffer[tau + 1] < yin_buffer[tau]) {
				tau++;
			}
			break;
		}
	}
	return (tau == size || yin_buffer[tau] >= yin_consts::Threshold<T>) ? -1
	                                                                    : tau;
}

template <typename T>
void
difference(const std::vector<T> &audio_buffer, pitch_alloc::Yin<T> *ya)
{
	acorr_r(audio_buffer, ya);

	for (int tau = 0; tau < ya->N / 2; tau++)
		ya->yin_buffer[tau] = 2 * ya->out_real[0] - 2 * ya->out_real[tau];
}

template <typename T>
void
cumulative_mean_normalized_difference(std::vector<T> &yin_buffer)
{
	double running_sum = 0.0f;

	yin_buffer[0] = 1;

	for (int tau = 1; tau < signed(yin_buffer.size()); tau++) {
		running_sum += yin_buffer[tau];
		yin_buffer[tau] *= tau / running_sum;
	}
}

template <typename T>
T
pitch_alloc::yin(const std::vector<T> &audio_buffer, int sample_rate,
    pitch_alloc::Yin<T> *ya)
{
	int tau_estimate;

	difference(audio_buffer, ya);

	cumulative_mean_normalized_difference(ya->yin_buffer);
	tau_estimate = absolute_threshold(ya->yin_buffer);

	auto ret = (tau_estimate != -1)
	               ? sample_rate / std::get<0>(parabolic_interpolation(
	                                   ya->yin_buffer, tau_estimate))
	               : -1;

	ya->clear();
	return ret;
}

template <typename T>
T
pitch::yin(const std::vector<T> &audio_buffer, int sample_rate)
{

	pitch_alloc::Yin<T> ya(audio_buffer.size());
	return pitch_alloc::yin(audio_buffer, sample_rate, &ya);
}

template class pitch_alloc::Yin<double>;
template class pitch_alloc::Yin<float>;

template double
pitch::yin<double>(const std::vector<double> &audio_buffer, int sample_rate);
template double
pitch_alloc::yin<double>(const std::vector<double> &audio_buffer,
    int sample_rate, pitch_alloc::Yin<double> *ya);

template float
pitch::yin<float>(const std::vector<float> &audio_buffer, int sample_rate);
template float
pitch_alloc::yin<float>(const std::vector<float> &audio_buffer, int sample_rate,
    pitch_alloc::Yin<float> *ya);

template static int
absolute_threshold<double>(const std::vector<double> &yin_buffer);
template static int
absolute_threshold<float>(const std::vector<float> &yin_buffer);

template void
cumulative_mean_normalized_difference<double>(std::vector<double> &yin_buffer);
template void
cumulative_mean_normalized_difference<float>(std::vector<float> &yin_buffer);

template void
difference<double>(
    const std::vector<double> &audio_buffer, pitch_alloc::Yin<double> *ya);
template void
difference<float>(
    const std::vector<float> &audio_buffer, pitch_alloc::Yin<float> *ya);
