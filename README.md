# Mathematical library for calculating the EEG signal spectrum
The main functionality is the calculation of raw spectrum values and the calculation of EEG spectrum values.
Working with the library is possible in iterative mode (adding new data to the internal buffer, calculating spectrum values) and in one-time spectrum calculation mode for a given array. When working in the iterative mode, the spectrum is calculated with the frequency set during initialization.

## Install

### Windows (cpp)

Download from [GitHub](https://github.com/BrainbitLLC/spectrum-lib-cpp) and add .dll from folder 'windows' to your project by your preferred way.

### Linux (cpp)

Download from [GitHub](https://github.com/BrainbitLLC/spectrum-lib-cpp) and add .so from folder 'linux_x86_64' to your project by your preferred way.

Library buit on Astra Linux CE 2.12.46, kernel 5.15.0-70-generic. Arch: x86_64 GNU/Linux

```
user@astra:~$ ldd --version
ldd (Debian GLIBC 2.28-10+deb10u1) 2.28
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Written by Roland McGrath and Ulrich Drepper.
```

That library depends from others library, for instance:
```
linux-vdso.so.1
libblkid.so.1
libc++.so.1
libc++abi.so.1 
libc.so.6
libdl.so.2
libffi.so.6
libgcc_s.so.1
libgio-2.0.so.0
libglib-2.0.so.0
libgmodule-2.0.so.0
libgobject-2.0.so.0
libm.so.6
libmount.so.1
libpcre.so.3
libpthread.so.0
libresolv.so.2 
librt.so.1
libselinux.so.1
libstdc++.so.6
libudev.so.1
libuuid.so.1
libz.so.1
ld-linux-x86-64.so.2
```

If you are using a OS other than ours, these dependencies will be required for the library. You can download dependencies from [here](https://github.com/BrainbitLLC/linux_neurosdk2/tree/main/dependencies).

## Initialization
### Main parameters
1. Raw signal sampling frequency. Need to be >= 1.
2. Spectrum calculation frequency. Need to be <= 16 kHz.
3. Spectrum calculation window length. Need to be <= signal sampling frequency.
### Optional parameters
1. Upper bound of frequencies for spectrum calculation. Default value is sampling_rate / 2.
2. Normalization of the EEG spectrum by the width of the wavebands. Disabled by default.
3. Weight coefficients for alpha, beta, theta, gamma, delta waves. By default has 1.0 value.
## Creation
```cpp
int sampling_rate = 250; // raw signal sampling frequency
int fft_window = 1000; // spectrum calculation window length
int process_win_rate = 5; // spectrum calculation frequency
SpectrumMath* tSpectMathPtr = createSpectrumMath(sampling_rate, fft_window, process_win_rate); 
```
## Optional initialization
1. Additional spectrum settings:
```cpp
int bord_frequency = 50; // upper bound of frequencies for spectrum calculation
bool normalize_spect_by_bandwidth = true; // normalization of the EEG spectrum by the width of the wavebands
SpectrumMathInitParams(tSpectMathPtr, bord_frequency, normalize_spect_by_bandwidth); 
```
2. Waves coefficients:
```cpp
double delta_coef = 0.0;
double theta_coef = 1.0;
double alpha_coef = 1.0;
double beta_coef = 1.0;
double gamma_coef = 0.0;
SpectrumMathSetWavesCoeffs(tSpectMathPtr, delta_coef, theta_coef, alpha_coef, beta_coef, gamma_coef);
```
3. Setting the smoothing of the spectrum calculation by Henning (by default) or Hemming window:
```cpp
SpectrumMathSetHanningWinSpect(tSpectMathPtr); // by Hanning (by default)

SpectrumMathSetHammingWinSpect(tSpectMathPtr); // by Hamming
```
## Initializing a data array for transfer to the library
Array of double values with length less or equals then 15 * saignal sampling frequency.
## Types
#### RawSpectrumData
Structure containing the raw spectrum values (with boundary frequency taken into library).

Fields:
- all_bins_nums - Integer value. Number of FFT bars. Contained only in the C++ interface.
- all_bins_values - Double array. Raw FFT bars values.
- total_raw_pow - Double value. Total raw spectrum power.

#### WavesSpectrumData
Structure containing the waves values.

Absolute frequency values (double type):
- delta_raw
- theta_raw
- alpha_raw
- beta_raw
- gamma_raw

Relative (percent) values (double type):
- delta_rel
- theta_rel
- alpha_rel
- beta_rel
- gamma_rel

## FFT band resolution
The library automatically matches the optimal buffer length (degree 2) to calculate the FFT during initialization, depending on the specified window length. Receiving resolution for the FFT bands (number of FFT bands per 1 Hz):
```cpp
double fftBinsFor1Hz = SpectrumMathGetFFTBinsFor1Hz(tSpectMathPtr);
```
## Spectrum calculation in iterative mode
1. Adding and process data:
```cpp
double* raw_data = new double[SIGNAL_SAMPLES_COUNT];
SpectrumMathPushData(tSpectMathPtr, raw_data, SIGNAL_SAMPLES_COUNT);
SpectrumMathProcessData(tSpectMathPtr);
```
2. Getting the results:
```cpp
uint32_t arr_sz = SpectrumMathReadSpectrumArrSize(tSpectMathPtr);

RawSpectrumData* raw_spect_data = new RawSpectrumData[arr_sz];
SpectrumMathReadRawSpectrumInfoArr(tSpectMathPtr, raw_spect_data, &arr_sz);

WavesSpectrumData* waves_spect_data = new WavesSpectrumData[arr_sz];
SpectrumMathReadWavesSpectrumInfoArr(tSpectMathPtr, waves_spect_data, &arr_sz);
```
4. Updating the number of new samples. Is necessary for correct calculation of elements in the array of obtained structures, if a large portion of data is added to the library all at once.
```cpp
SpectrumMathSetNewSampleSize(tSpectMathPtr);
```
## Spectrum calculation for a single array
1. Compute spectrum:
```cpp
double* vals_arr = new double[arr_size];
SpectrumMathComputeSpectrum(tSpectMathPtr, vals_arr, arr_size);
```
2. Getting the results:
```cpp
RawSpectrumData raw_spect_data;
SpectrumMathReadRawSpectrumInfo(tSpectMathPtr, &raw_spect_data);

WavesSpectrumData waves_spect_data;
SpectrumMathReadWavesSpectrumInfo(tSpectMathPtr, &waves_spect_data)
```
## Finishing work with the library
```cpp
SpectrumMathClearData(tSpectMathPtr);
```	
