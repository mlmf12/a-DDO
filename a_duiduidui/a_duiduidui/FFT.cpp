/*
实现合成信号的FFT变换，并在图形界面上显示
程序中采用的是1024和256点DFT，基2FFT
*/

#include<FFT.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;
const int N = 1024;
const float PI = 3.1416;
#define pi 3.1415926

inline void swap(float &a, float &b)
{
    float t;
    t = a;
    a = b;
    b = t;
}

void bitrp(float xreal[], float ximag[], int n)
{
    // 位反转置换 Bit-reversal Permutation
    int i, j, a, b, p;
    for (i = 1, p = 0; i < n; i *= 2)
    {
        p++;
    }
    for (i = 0; i < n; i++)
    {
        a = i;
        b = 0;
        for (j = 0; j < p; j++)
        {
            b = (b << 1) + (a & 1);    // b = b * 2 + a % 2;
            a >>= 1;        // a = a / 2;
        }
        if (b > i)
        {
            swap(xreal[i], xreal[b]);
            swap(ximag[i], ximag[b]);
        }
    }
}

void FFT(float xreal[], float ximag[], int n)
{
    // 快速傅立叶变换，将复数 x 变换后仍保存在 x 中，xreal, ximag 分别是 x 的实部和虚部
    float wreal[N / 2], wimag[N / 2], treal, timag, ureal, uimag, arg;
    int m, k, j, t, index1, index2;

    bitrp(xreal, ximag, n);

    //计算蝶形运算需要用到的Wnk
    arg = -2 * PI / n;
    treal = cos(arg);
    timag = sin(arg);
    wreal[0] = 1.0;
    wimag[0] = 0.0;
    for (j = 1; j < n / 2; j++)//后项与前项的关系，证明过程在ipad里
    {
        wreal[j] = wreal[j - 1] * treal - wimag[j - 1] * timag;
        wimag[j] = wreal[j - 1] * timag + wimag[j - 1] * treal;
    }

    for (m = 2; m <= n; m *= 2)//m=2为第一层FFT，m/2为每次蝶形时两个数据点之间的间隔
    {
        for (k = 0; k < n; k += m)
        {
            for (j = 0; j < m / 2; j++)
            {
                //本次蝶形运算中参与运算的两个点的存储位置
                index1 = k + j;
                index2 = index1 + m / 2;

                //本次蝶形运算中参与运算的Wnk的存储位置
                t = n * j / m;

                //计算Wnk与X的乘积
                treal = wreal[t] * xreal[index2] - wimag[t] * ximag[index2];
                timag = wreal[t] * ximag[index2] + wimag[t] * xreal[index2];

                //不与Wnk相乘的那个X的值
                ureal = xreal[index1];
                uimag = ximag[index1];

                //计算本次蝶形运算得到的两个值的大小，由于每层蝶形运算中，
                //同一个存储单元内的值只参与一次运算，所以可以将结果原位
                //存储，因为本层之后的蝶形运算用不到这个单元里的值了
                xreal[index1] = ureal + treal;
                ximag[index1] = uimag + timag;
                xreal[index2] = ureal - treal;
                ximag[index2] = uimag - timag;
            }
        }
    }
}
void FFT_2(float xreal[], float ximag[])
{
   float value_real[1024] = { 0 };
   float value_imag[1024] = { 0 };
   for (int i = 0; i <= 1023; i++)
   {
       value_real[i] =xreal[i];
   }
    FFT(value_real, value_imag, 1024);
    for (int i = 0; i < 1024; i++)
    {

        ximag[i] = 2*(sqrt(((value_real[i])*(value_real[i]) + (value_imag[i])*(value_imag[i]))) / 1024);
        xreal[i] = 1000 * i / 1024;
    }
}
void get_FFT_data(double real_freq[1024],double amp[1024],double freq_a_radians,double freq_b_radians,double freq_c_radians)
{
    int FS = 1000;
    int N = 1024;
    int i;
    float value_real[1024] = { 0 };

    //对合成信号进行采样
    for (i = 0; i <= 1023; i++)
    {
        value_real[i] = sin(freq_a_radians*i/FS)+sin(freq_b_radians*i/FS)+cos(freq_c_radians*i/FS);
    }
    float value_imag[1024] = { 0 };

    //对合成信号进行FFT
    FFT(value_real, value_imag, N);
    for (i = 0; i < N; i++)
    {

        amp[i] = 2*(sqrt(((value_real[i])*(value_real[i]) + (value_imag[i])*(value_imag[i]))) / 1024);
        real_freq[i] = 1000 * i / 1024;
    }
}
