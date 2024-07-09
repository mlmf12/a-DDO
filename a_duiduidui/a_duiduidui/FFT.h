#ifndef FFT_H
#define FFT_H
void get_FFT_data(double real_freq[1024],double amp[1024],double freq_a_radians,double freq_b_radians,double freq_c_radians);
void FFT_2(float xreal[], float ximag[]);
void FFT(float xreal[], float ximag[], int n);
#endif // FFT_H
