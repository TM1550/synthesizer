#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define M_EXP 2.71828182845904523536
void lowfrequencyfilter(double* in, int sizein, double* out, double bandwidth, double transitionwidth) //it takes the source array, the size of the source array, the output array, the bandwidth, the transition width
{                                                                                                      //gives the output array filtered by a low-pass filter
	int N = (int)ceil(5.5 / (transitionwidth / 48000.0)); //filter length
	if (N % 2 == 1)
	{
		N++;
	}
	double Fd = 48000; //samplerate
	double* H = (double*)malloc(sizeof(double) * N); //Impulse response of the filter
	double* H_id = (double*)malloc(sizeof(double) * N); //Ideal impulse response
	double* W = (double*)malloc(sizeof(double) * N); //Weight function
	double Fc = (bandwidth + transitionwidth / 2.0) / Fd; //Calculation of the impulse response of the filter
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			H_id[i] = 2 * Fc;
		}
		else
		{
			H_id[i] = sin(2 * M_PI * i * Fc) / (M_PI * i);
		}
		// Blackman's weight function
		W[i] = 0.42 + 0.5 * cos((2 * M_PI * i) / (N - 1)) + 0.08 * cos((4 * M_PI * i) / (N - 1));
		H[i] = H_id[i] * W[i];
	}
	//Normalization of the impulse response
	double SUM = 0;
	for (int i = 0; i < N; i++)
	{
		SUM += H[i];
	}
	for (int i = 0; i < N; i++)
	{
		H[i] /= SUM;
	}//The sum of the coefficients is 1
	//Filtering the input data
	for (int i = 0; i < sizein; i++)
	{
		out[i] = 0;
		for (int j = 0; j < N - 1; j++)
		{
			if (i - j >= 0)
			{
				out[i] += H[j] * in[i - j];
			}
		}
	}
	free(H);
	free(H_id);
	free(W);
	return;
}
void highfrequencyfilter(double* in, int sizein, double* out, double bandwidth, double transitionwidth) //it takes the source array, the size of the source array, the output array, the bandwidth, the transition width
{                                                                                                       //gives the output array filtered by a high-pass filter
	int N = (int)ceil(5.5 / (transitionwidth / 48000.0));
	if (N % 2 == 1)
	{
		N++;
	}
	double Fd = 48000;
	double* H = (double*)malloc(sizeof(double) * N);
	double* H_id = (double*)malloc(sizeof(double) * N);
	double* W = (double*)malloc(sizeof(double) * N);
	double Fc = (bandwidth + transitionwidth / 2.0) / Fd;
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			H_id[i] = 1 - 2 * Fc;
		}
		else
		{
			H_id[i] = -sin(2 * M_PI * i * Fc) / (M_PI * i);
		}
		W[i] = 0.42 + 0.5 * cos((2 * M_PI * i) / (N - 1)) + 0.08 * cos((4 * M_PI * i) / (N - 1));
		H[i] = H_id[i] * W[i];
	}
	double SUM = 0;
	for (int i = 0; i < N; i++)
	{
		SUM += H[i];
	}
	for (int i = 0; i < N; i++)
	{
		H[i] /= SUM;
	}
	for (int i = 0; i < sizein; i++)
	{
		out[i] = 0;
		for (int j = 0; j < N - 1; j++)
		{
			if (i - j >= 0)
			{
				out[i] += H[j] * in[i - j];
			}
		}
	}
	free(H);
	free(H_id);
	free(W);
	return;
}
void bandpassfilter(double* in, int sizein, double* out, double bandwidth, double transitionwidth) //it takes the source array, the size of the source array, the output array, the bandwidth, the transition width
{                                                                                                  //gives the output array filtered by a bandpass filter
	int N = (int)ceil(5.5 / (transitionwidth / 48000.0));
	if (N % 2 == 0)
	{
		N++;
	}
	double Fd = 48000;
	double Fs = bandwidth / Fd;
	double Fx = (bandwidth + transitionwidth) / Fd;
	double* H = (double*)malloc(sizeof(double) * N);
	double* H_id = (double*)malloc(sizeof(double) * N);
	double* W = (double*)malloc(sizeof(double) * N);
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			H_id[i] = 2 * (Fx - Fs);
		}
		else
		{
			H_id[i] = sin(2 * M_PI * Fx * i) / (i * M_PI) - sin(2 * M_PI * Fs * i) / (i * M_PI);
		}
		W[i] = 0.42 + 0.5 * cos((2 * M_PI * i) / (N - 1)) + 0.08 * cos((4 * M_PI * i) / (N - 1));
		H[i] = H_id[i] * W[i];
	}
	double SUM = 0;
	for (int i = 0; i < N; i++)
	{
		SUM += H[i];
	}
	for (int i = 0; i < N; i++)
	{
		H[i] /= SUM;
	}
	for (int i = 0; i < sizein; i++)
	{
		out[i] = 0;
		for (int j = 0; j < N - 1; j++)
		{
			if (i - j >= 0)
			{
				out[i] += H[j] * in[i - j];
			}
		}
	}
	free(H);
	free(H_id);
	free(W);
	return;
}
void blockingfilter(double* in, int sizein, double* out, double bandwidth, double transitionwidth) //it takes the source array, the size of the source array, the output array, the bandwidth, the transition width
{                                                                                                  //gives the output array filtered by a blocking filter
	int N = (int)ceil(5.5 / (transitionwidth / 48000.0));
	if (N % 2 == 0)
	{
		N++;
	}
	double Fd = 48000;
	double Fs = bandwidth / Fd;
	double Fx = (bandwidth + transitionwidth) / Fd;
	double* H = (double*)malloc(sizeof(double) * N);
	double* H_id = (double*)malloc(sizeof(double) * N);
	double* W = (double*)malloc(sizeof(double) * N);
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			H_id[i] = 1 - 2 * (Fx - Fs);
		}
		else
		{
			H_id[i] = sin(2 * M_PI * Fs * i) / (i * M_PI) - sin(2 * M_PI * Fx * i) / (i * M_PI);
		}
		W[i] = 0.42 + 0.5 * cos((2 * M_PI * i) / (N - 1)) + 0.08 * cos((4 * M_PI * i) / (N - 1));
		H[i] = H_id[i] * W[i];
	}
	double SUM = 0;
	for (int i = 0; i < N; i++)
	{
		SUM += H[i];
	}
	for (int i = 0; i < N; i++)
	{
		H[i] /= SUM;
	}
	for (int i = 0; i < sizein; i++)
	{
		out[i] = 0;
		for (int j = 0; j < N - 1; j++)
		{
			if (i - j >= 0)
			{
				out[i] += H[j] * in[i - j];
			}
		}
	}
	free(H);
	free(H_id);
	free(W);
	return;
}
void filter(double* in, int sizein, double* out, double bandwidth, double transitionwidth, int type) //it takes the source array, the size of the source array, the output array, the bandwidth, the transition width, type of filter
{                                                                                                    //gives an output array filtered by the filter specified by the user or does not filter if the filter type is unknown
	if (type == 1)
	{
		lowfrequencyfilter(in, sizein, out, bandwidth, transitionwidth);
		return;
	}
	if (type == 2)
	{
		highfrequencyfilter(in, sizein, out, bandwidth, transitionwidth);
		return;
	}
	if (type == 3)
	{
		bandpassfilter(in, sizein, out, bandwidth, transitionwidth);
		return;
	}
	if (type == 4)
	{
		blockingfilter(in, sizein, out, bandwidth, transitionwidth);
		return;
	}
	for (int i = 0; i < sizein; i++)
	{
		out[i] = in[i];
	}
	return;
}