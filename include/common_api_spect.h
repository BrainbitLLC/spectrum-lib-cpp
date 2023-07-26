#ifndef COMMON_API_SPECT_H
#define COMMON_API_SPECT_H

#include "lib_export.h"
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct _RawSpectrumData {
		int all_bins_nums;
		double* all_bins_values;		
		double total_raw_pow;
	} RawSpectrumData;

	typedef struct _WavesSpectrumData {
		double delta_raw, theta_raw, alpha_raw, beta_raw, gamma_raw; // absolute waves values
		double delta_rel, theta_rel, alpha_rel, beta_rel, gamma_rel; // relative waves values
	} WavesSpectrumData;
	
	typedef struct _SpectrumMath SpectrumMath;

	SDK_SHARED SpectrumMath* createSpectrumMath(int sampl_rate, int fft_window, int process_win_freq);
	SDK_SHARED void freeSpectrumMath(SpectrumMath*);

	SDK_SHARED void SpectrumMathInitParams(SpectrumMath* spectrumMathPtr, int up_border_frequency, bool normalize_spect_by_bandwidth);
	SDK_SHARED void SpectrumMathSetWavesCoeffs(SpectrumMath* spectrumMathPtr, double d_coef, double t_coef, double a_coef, double b_coef, double g_coef);
	SDK_SHARED void SpectrumMathSetHanningWinSpect(SpectrumMath* spectrumMathPtr);
	SDK_SHARED void SpectrumMathSetHammingWinSpect(SpectrumMath* spectrumMathPtr);
	SDK_SHARED void SpectrumMathSetSquaredSpect(SpectrumMath* spectrumMathPtr, bool fl);

	SDK_SHARED void SpectrumMathPushData(SpectrumMath* spectrumMathPtr, const double* samples, size_t sampleCount);
	SDK_SHARED void SpectrumMathProcessData(SpectrumMath* spectrumMathPtr);

	SDK_SHARED void SpectrumMathComputeSpectrum(SpectrumMath* spectrumMathPtr, double* vals_arr, int arr_size);

	SDK_SHARED double SpectrumMathGetFFTBinsFor1Hz(SpectrumMath* spectrumMathPtr);

	SDK_SHARED void SpectrumMathReadRawSpectrumInfo(SpectrumMath* spectrumMathPtr, RawSpectrumData* raw_spect_data);
	SDK_SHARED void SpectrumMathReadWavesSpectrumInfo(SpectrumMath* spectrumMathPtr, WavesSpectrumData* waves_spect_data);

	SDK_SHARED uint32_t SpectrumMathReadSpectrumArrSize(SpectrumMath* spectrumMathPtr);
	SDK_SHARED void SpectrumMathReadRawSpectrumInfoArr(SpectrumMath* spectrumMathPtr, RawSpectrumData* raw_spect_data_arr, uint32_t* arr_size);
	SDK_SHARED void SpectrumMathReadWavesSpectrumInfoArr(SpectrumMath* spectrumMathPtr, WavesSpectrumData* waves_spect_data_arr, uint32_t* arr_size);

	SDK_SHARED void SpectrumMathSetNewSampleSize(SpectrumMath* spectrumMathPtr);

	SDK_SHARED void SpectrumMathClearData(SpectrumMath* spectrumMathPtr);

	SDK_SHARED void SpectrumMathReleaseNativeArray(double* arrayPtr);

#ifdef __cplusplus
}
#endif

#endif // COMMON_API_SPECT_H
