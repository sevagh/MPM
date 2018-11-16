### Pitch detection algorithms

A collection of autocorrelation-based C++ pitch detection algorithms.

* [McLeod Pitch Method](http://miracle.otago.ac.nz/tartini/papers/A_Smarter_Way_to_Find_Pitch.pdf)
* [YIN](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf)
* Autocorrelation

Visualization of McLeod pitch method (and the advantages over autocorrelation) here: https://github.com/sevagh/mcleod

### API breaking change - 15/11/2018

**N.B.** `get_pitch_yin`, `get_pitch_mpm`, `get_pitch_autocorrelation` have become `pitch::yin`, `pitch::mpm`, `pitch::autocorrelation`.

### Tests

To run the tests, you need [googletest](https://github.com/google/googletest), and run `make test && ./bin/test`:

```
[----------] 5 tests from MpmInstrumentTest
[ RUN      ] MpmInstrumentTest.Violin_A4_44100
[       OK ] MpmInstrumentTest.Violin_A4_44100 (234 ms)
[ RUN      ] MpmInstrumentTest.Piano_B4_44100
[       OK ] MpmInstrumentTest.Piano_B4_44100 (101 ms)
[ RUN      ] MpmInstrumentTest.Piano_D4_44100
[       OK ] MpmInstrumentTest.Piano_D4_44100 (116 ms)
[ RUN      ] MpmInstrumentTest.Acoustic_E2_44100
[       OK ] MpmInstrumentTest.Acoustic_E2_44100 (4 ms)
[ RUN      ] MpmInstrumentTest.Classical_FSharp4_48000
[       OK ] MpmInstrumentTest.Classical_FSharp4_48000 (239 ms)
[----------] 5 tests from MpmInstrumentTest (695 ms total)

[----------] 5 tests from YinInstrumentTest
[ RUN      ] YinInstrumentTest.Violin_A4_44100
[       OK ] YinInstrumentTest.Violin_A4_44100 (7416 ms)
[ RUN      ] YinInstrumentTest.Piano_B4_44100
[       OK ] YinInstrumentTest.Piano_B4_44100 (1153 ms)
[ RUN      ] YinInstrumentTest.Piano_D4_44100
[       OK ] YinInstrumentTest.Piano_D4_44100 (2446 ms)
[ RUN      ] YinInstrumentTest.Acoustic_E2_44100
[       OK ] YinInstrumentTest.Acoustic_E2_44100 (6 ms)
[ RUN      ] YinInstrumentTest.Classical_FSharp4_48000
[       OK ] YinInstrumentTest.Classical_FSharp4_48000 (7353 ms)
[----------] 5 tests from YinInstrumentTest (18374 ms total)
```

### Bench

To run the bench, you need [google benchmark](https://github.com/google/benchmark), and run `make bench && ./bin/bench`:

```
--------------------------------------------------------------------
Benchmark                             Time           CPU Iterations
--------------------------------------------------------------------
BM_Yin_Sinewave_128k_samples  984091850 ns  982514163 ns          1
BM_Mpm_Sinewave_128k_samples   37793353 ns   37492225 ns         19
```

My implementation of YIN is using time-domain autocorrelation vs. FFT and the disadvantage is obvious from the benchmark.

### Install

Depends on [ffts](https://github.com/anthonix/ffts). Input vectors of size power-of-2 will perform better due to FFTS optimizations.

Instructions are generated from a bare Fedora 28 Docker container.

With ffts:

```
$ dnf install git cmake make gcc gcc-c++ gflags
$ git clone https://github.com/anthonix/ffts
$ cd ffts
$ mkdir build && cd build && cmake .. && make && make install
$ cd ../../
$ git clone https://github.com/sevagh/pitch-detection
$ cd pitch-detection
$ make
$ sudo make install
$ make example
```

Without ffts:

```
$ dnf install git make gcc gcc-c++ gflags
$ git clone https://github.com/sevagh/pitch-detection
$ cd pitch-detection 
$ FFT_FLAG="-DPORTABLE_XCORR" make # build with no FFTS
$ sudo make install
$ make example
```

Use with `#include <pitch_detection.h>` and the flag `-lpitch_detection`.
