#include <stdio.h>
#include <math.h>

#define ORDER 8
#define FILTER_LEN (ORDER*2+1)

static double a[FILTER_LEN];
static double b[FILTER_LEN];

// Filter state
static double x[FILTER_LEN];
static double y[FILTER_LEN];

void calculate_coefficients(double sampleRate, double lowCutoff, double highCutoff)
{
    double low_omega = 2.0 * M_PI * lowCutoff / sampleRate;
    double high_omega = 2.0 * M_PI * highCutoff / sampleRate;
    double low_c = 1.0 / tan(low_omega / 2.0);
    double high_c = tan((high_omega / 2.0) * (1 + high_c) / (1 - high_c));

    double low_c_squared = low_c * low_c;
    double high_c_squared = high_c * high_c;
    double low_c_pow = low_c_squared;
    double high_c_pow = high_c_squared;

    a[0] = 1.0;
    b[0] = low_c_pow * high_c_pow;
    for (int i = 1; i <= ORDER * 2; i++)
    {
        a[i] = 2.0 * cos(i * M_PI / (2.0 * ORDER)) / (1 + low_c * high_c) * a[i - 1];
        low_c_pow *= low_c_squared;
        high_c_pow *= high_c_squared;
        b[i] = -low_c_pow * high_c_pow;
    }

    double sum_a = 0.0, sum_b = 0.0;
    for (int i = 0; i <= ORDER * 2; i++)
    {
        sum_a += a[i];
        sum_b += b[i];
    }

    for (int i = 0; i <= ORDER * 2; i++)
    {
        a[i] /= sum_a;
        b[i] /= sum_b;
    }

    // Reset the filter state
    for (int i = 0; i <= ORDER * 2; i++) {
        x[i] = 0.0;
        y[i] = 0.0;
    }
}

inline int16_t apply_filter(int16_t sample)
{
    double sum = 0.0;
    for (int j = 0; j < ORDER * 2; j++)
    {
        sum += a[j + 1] * x[j] - b[j + 1] * y[j];
    }
    // sum += b[0] * (double)sample;
    y[ORDER * 2] = sum;

    // Update the filter state
    for (int j = ORDER * 2; j > 0; j--)
    {
        x[j] = x[j - 1];
        y[j - 1] = y[j];
    }
    x[0] = (int16_t)sample;

    return (int16_t)y[0];
}