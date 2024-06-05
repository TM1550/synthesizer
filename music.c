#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wave.h"
#include "filter.h"
#define M_PI 3.14159265358979323846
typedef struct track { 
	int* data;
	int datacount;
	int datacup;
	int samplerate;
}track; //structure for an audio file
double distort(double result, double treshold) //takes the value of the sound, the treshold coefficient, to give a sound whose amplitude has been cut off if the amplitude is greater than the treshold coefficient
{
	if (result > treshold)
	{
		result = treshold;
	}
	if (result < -treshold)
	{
		result = -treshold;
	}
	return result / treshold;
}
double getnote(int key, int octave) //takes a note, gives the frequency of the note
{
	return 27.5 * pow(2.0, (key - 10 + octave * 12.0) / 12.0);
}
int notes(char* str, int* note, int* octave) //takes a string, a pointer to a note in an octave, a pointer to an octave, gives the value 1 and sets the note if the string is a note, gives the value 0 if the string is not a note
{
	if ((str[0] == 'C' || str[0] == 'D' || str[0] == 'E' || str[0] == 'F' || str[0] == 'G' || str[0] == 'A' || str[0] == 'B') && ((int)str[1] >= 48 && (int)str[1] <= 57 || str[1] == '#' && (int)str[2] >= 48 && (int)str[2] <= 57))
	{
		if (str[0] == 'C')
		{
			*note = 1;
		}
		if (str[0] == 'D')
		{
			*note = 3;
		}
		if (str[0] == 'E')
		{
			*note = 5;
		}
		if (str[0] == 'F')
		{
			*note = 6;
		}
		if (str[0] == 'G')
		{
			*note = 8;
		}
		if (str[0] == 'A')
		{
			*note = 10;
		}
		if (str[0] == 'B')
		{
			*note = 12;
		}
		if (str[1] == '#')
		{
			(*note)++;
		}
		if ((int)str[1] >= 48 && (int)str[1] <= 57)
		{
			*octave = (int)str[1] - 48;
			return 1;
		}
		*octave = (int)str[2] - 48;
		return 1;
	}
	return 0;
}
void music(track* song, int* args, int argc, char* res) //takes a pointer to the structure, an array with indexes of the beginning of words, the size of this array, an array of characters of the input file
{                                                       //processes the commands that the user entered in his command file, writes the sound value to the data array
	double temp = 1, volume = 1;
	double dtime, length = 0, position = 0, frequency, frequency1, frequency2, result, treshold=1.0;
	double timeattack = 0, timedecay = 0, timesustain = 0.5, timerelease = 0, shifttime = 0, timeslope = 0.0, silence, breakpoint = 1.0, sustain = 1.0;
	double bandwidth = 1200, transitionwidth = 500; int typefilter = 0;
	int index = 0, note = 0, octave = 0, samplerate = 48000, numberoscilators = 32;
	double* results = (double*)malloc((numberoscilators + 1) * sizeof(double));
	double* volumes = (double*)malloc((numberoscilators + 1) * sizeof(double));
	double* coefficents = (double*)malloc((numberoscilators + 1) * sizeof(double));
	int* types = (int*)malloc((numberoscilators + 1) * sizeof(int));
	double value1; int value3, value4;
	double* input; double* output;
	song->datacount = 0;
	song->samplerate = samplerate;
	song->datacup = 2 * song->samplerate;
	song->data = (int*)malloc(sizeof(int) * song->datacup);
	for (int i = 1; i <= numberoscilators; i++)
	{
		volumes[i] = 1;
		coefficents[i] = 1;
		types[i] = 0;
	}
	types[1] = 1;
	while (index <argc)
	{
		if (strcmp(&res[args[index]], "volumes")==0)
		{
			index++;
			value3 = atoi(&res[args[index]]);
			index++;
			if (value3 >= 1 && value3 <= numberoscilators)
			{
				value1 = strtod(&res[args[index]], NULL);
				index++;
				if (value1 >= 0 && value1 <= 1)
				{
					volumes[value3] = value1;
				}
			}
			continue;
		}
		if (strcmp(&res[args[index]], "types") == 0)
		{
			index++;
			value3 = atoi(&res[args[index]]);
			index++;
			if (value3 >= 1 && value3 <= numberoscilators)
			{
				value4 = atoi(&res[args[index]]);
				index++;
				volumes[value3] = value4;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "coefficiencts") == 0)
		{
			index++;
			value3 = atoi(&res[args[index]]);
			index++;
			if (value3 >= 1 && value3 <= numberoscilators)
			{
				value1 = strtod(&res[args[index]], NULL);
				index++;
				if (value1 >= 0.01 && value1 <=100)
				{
					volumes[value3] = value1;
				}
			}
			continue;
		}
		if (strcmp(&res[args[index]], "volume") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1 <= 1)
			{
				volume = value1;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "temp") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0.015625 && value1 <= 32)
			{
				temp = 1.0 / value1;
				timeattack *= temp; timedecay *= temp; timerelease *= temp;
				timeslope *= temp; timesustain *= temp; shifttime *= temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "attack") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1<=60.0)
			{
				timeattack = value1 * temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "decay") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1<=60.0)
			{
				timedecay = value1 * temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "sustaintime") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1<=60.0)
			{
				timesustain = value1 * temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "slope") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1<=60.0)
			{
				timeslope = value1 * temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "release") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1<=60.0)
			{
				timerelease = value1 * temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "silence") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1<=60.0)
			{
				silence = value1;
				length += silence * song->samplerate;
				if (song->datacup <= song->datacount)
				{
					song->datacup*=2;
					song->data = (int*)malloc(sizeof(int) * song->datacount);
				}
				for (int i = (int)position; i <= (int)length; i++)
				{
					song->data[i] = 0;
				}
				position = length;
				song->datacount = (int)position;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "shift") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1<length && value1>=0)
			{
				shifttime = value1 * temp;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "breakpoint") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1 <= 1)
			{
				breakpoint = value1;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "sustain") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 0 && value1 <= 1)
			{
				sustain = value1;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "typefilter") == 0)
		{
			index++;
			value3 = atoi(&res[args[index]]);
			index++;
			typefilter = value3;
			continue;
		}
		if (strcmp(&res[args[index]], "bandwidth") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 >= 20 && value1 <= 20000)
			{
				bandwidth = value1;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "transiditionwidth") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1+bandwidth >= 20 && value1+bandwidth <= 20000)
			{
				transitionwidth = value1;
			}
			continue;
		}
		if (strcmp(&res[args[index]], "treshold") == 0)
		{
			index++;
			value1 = strtod(&res[args[index]], NULL);
			index++;
			if (value1 > 0.1 && value1 <= 1)
			{
				treshold = value1;
			}
			continue;
		}
		if (notes(&res[args[index]], &note, &octave) == 1) //records sound
		{
			index++;
			frequency = getnote(note, octave);
			frequency1 = frequency * 2 * M_PI / song->samplerate;
			frequency2 = frequency / song->samplerate;
			dtime = timeattack + timedecay + timerelease + timesustain + timeslope;
			length += dtime * song->samplerate;
			if (song->datacup / 2 < length)
			{
				song->datacup *= 2;
				song->data = (int*)realloc(song->data, sizeof(int) * song->datacup + song->samplerate * 1);
			}
			input = (double*)malloc(sizeof(double) * ((int)length - (int)position));
			output = (double*)malloc(sizeof(double) * ((int)length - (int)position));
			for (int i = (int)position; i < (int)length; i++)
			{
				result = 0;
				for (int j = 1; j <= numberoscilators; j++)
				{
					results[j] = wave(i, frequency1 * coefficents[j], frequency2 * coefficents[j], types[j]) * volumes[j];
					result += results[j];
				}
				result *= adbssr(timeattack, timedecay, breakpoint, timeslope, timesustain, timerelease, i, position, sustain, song->samplerate);
				input[i - (int)position] = result;
			}
			filter(input, (int)length - (int)position, output, bandwidth, transitionwidth, typefilter);
			for (int i = 0; i < (int)length - (int)position; i++)
			{
				output[i] = distort(output[i], treshold);
			}
			for (int i = (int)position; i < (int)length; i++)
			{
				output[i - (int)position] *= INT_MAX * volume;
				if (output[i - (int)position] > INT_MAX)
				{
					output[i - (int)position] = INT_MAX;
				}
				if (output[i - (int)position] < INT_MIN)
				{
					output[i - (int)position] = INT_MIN;
				}
				song->data[i] = (int)output[i - (int)position];
			}
			length -= shifttime * song->samplerate;
			position = length;
			song->datacount = (int)length;
			free(input);
			free(output);
			continue;
		}
		index++;
	}
	length += shifttime * song->samplerate;
	song->datacount = (int)length;
	free(results);
	free(volumes);
	free(types);
	return;
}
