#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define M_EXP 2.71828182845904523536
double sine(int index, double frequency) //takes the position and frequency, gives the value of the sine wave
{
	return sin(frequency * index); 
}
double saw(int index, double frequency) //takes the position and frequency, gives the value of the saw wave
{
	return 2.0 * (index * frequency - floor(index * frequency)) - 1.0;
}
double remains, quet;
double triangle(int index, double frequency) //takes the position and frequency, gives the value of the triangle wave
{
	remains = modf(index * frequency, &quet);
	if (remains < 0.25)
	{
		return 4 * remains;
	}
	if (remains < 0.75)
	{
		return 2 - 4 * remains;
	}
	return 4 * (remains - 1);
}
double flat(int index, double frequency) //takes the position and frequency, gives the value of the flat wave
{
	if (sin(frequency * index) > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
double noice() //takes the position and frequency, gives the value of the noice
{
	return (double)rand() / (double)RAND_MAX * 2.0 - 1.0;
}
double wave(int index, double frequency1, double frequency2, int type) //takes the position and frequency, type of wave, gives the value of the sound
{
	if (frequency1 <= 0.000417)
	{
		frequency1 = 0.000417;
	}
	if (frequency2 <= 0.000417)
	{
		frequency2 = 0.000417;
	}
	if (frequency1 >= 0.41667)
	{
		frequency1 = 0.41667;
	}
	if (frequency2 >= 0.41667)
	{
		frequency2 = 0.41667;
	}
	if (type == 1)
	{
		return sine(index, frequency1);
	}
	if (type == 2)
	{
		return flat(index, frequency1);
	}
	if (type == 3)
	{
		return triangle(index, frequency2);
	}
	if (type == 4)
	{
		return saw(index, frequency2);
	}
	if (type == 5)
	{
		return noice();
	}
	return 0;
}
double sublength(double a, double b, double t) //takes the value of the two extreme amplitudes and the time, gives a multiplier for the sound to weaken it
{
	return (exp(1.0 - t) - 1.0) / (M_EXP - 1.0) * (a - b) + b;
}
double addlength(double a, double b, double t) //takes the value of the two extreme amplitudes and the time, gives a multiplier for the sound to stronger it
{
	return 6.0 * pow(log(t + 1.0), 2.0) / (pow(log(2.0), 2.0) * (5.0 * t + 1.0)) * (b - a) + a;
}
double timeot;
double adbssr(double a, double d, double b, double s1, double s2, double r, double index, double start, double sustainvalue, int samplerate) //takes the value of attack time, decay time, slop time, sustain time, release time, breakpoint, sustain point, position, start, samplerate
{                                                                                                                                            //gives a multiplier for changing the sound
	timeot = index - start;
	if (timeot == 0)
	{
		return 1.0;
	}
	if (timeot <= a * samplerate)
	{
		return addlength(0, 1.0, timeot / (a * samplerate));
	}
	timeot -= a * samplerate;
	if (timeot <= d * samplerate)
	{
		return sublength(1.0, b, timeot / (d * samplerate));
	}
	timeot -= d * samplerate;
	if (timeot <= s1 * samplerate)
	{
		if (b < sustainvalue)
		{
			return addlength(b, sustainvalue, timeot / (s1 * samplerate));
		}
		else
		{
			return sublength(b, sustainvalue, timeot / (s1 * samplerate));
		}
	}
	timeot -= s1 * samplerate;
	if (timeot <= s2 * samplerate)
	{
		return sustainvalue;
	}
	timeot -= s2 * samplerate;
	return sublength(sustainvalue, 0, timeot / (r * samplerate));
}