#ifndef PITCH_DETECTION_PRIV_H
#define PITCH_DETECTION_PRIV_H

/* MPM configs */
#define MPM_CUTOFF 0.93
#define MPM_SMALL_CUTOFF 0.5
#define MPM_LOWER_PITCH_CUTOFF 80

/* YIN configs */
#define YIN_DEFAULT_THRESHOLD 0.20

#include <complex>
#include <pitch_detection.h>
#include <vector>

std::pair<double, double>
parabolic_interpolation(const std::vector<double> &, double);

void
acorr_r(const std::vector<double> &, PitchAlloc *);

#endif /* PITCH_DETECTION__PRIV_H */
