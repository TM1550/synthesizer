#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "music.h"
void savefile(char* filename, int* bytes, unsigned int samplerate, unsigned int sizefile) //takes the name of the output file, the audio data array, the sampling frequency, the length of the array, writes the audio data array to the file
{
	char* error = "Error opening file: wavfile";
	FILE* soundfile = fopen(filename, "wb");
	if (soundfile == NULL)
	{
		perror(error);
	}
	unsigned int chunksize = 36 + 4 * sizefile;
	unsigned int sizeformat = 16;
	short pcm = 1;
	short numchannels = 1;
	unsigned int frequency = samplerate;
	unsigned int byterate = 4 * samplerate;
	short blockalign = 4;
	short bitspersample = 32;
	unsigned int sizedata = sizefile * 4;
	fprintf(soundfile, "RIFF");
	fwrite(&chunksize, 4, 1, soundfile);
	fprintf(soundfile, "WAVE");
	fprintf(soundfile, "fmt ");
	fwrite(&sizeformat, 4, 1, soundfile);
	fwrite(&pcm, 2, 1, soundfile);
	fwrite(&numchannels, 2, 1, soundfile);
	fwrite(&frequency, 4, 1, soundfile);
	fwrite(&byterate, 4, 1, soundfile);
	fwrite(&blockalign, 2, 1, soundfile);
	fwrite(&bitspersample, 2, 1, soundfile);
	fprintf(soundfile, "data");
	fwrite(&sizedata, 4, 1, soundfile);
	fwrite(bytes, sizeof(int), sizefile, soundfile);
	fclose(soundfile);
	return;
}
int main(int argc, char* argv[])
{
	char* outfile = (char*)malloc(sizeof(char) * 200);
	char* inputfile = (char*)malloc(sizeof(char) * 200);
	if (argc < 3)
	{
		printf("Enter the name of the txt file with the notes\n");
		scanf("%s", inputfile);
		printf("Enter the name of the wav file\n");
		scanf("%s", outfile);
	}
	if (argc == 3)
	{
		inputfile = argv[1];
		outfile = argv[2];
	}
	strcat(inputfile, ".txt");
	strcat(outfile, ".wav");
	FILE* f = freopen(inputfile, "rt", stdin);
	char* error = "Error opening file: txtfile";
	if (f == NULL)
	{
		perror(error);
	}
	int n = 10000;
	int k = 1000000;
	track newsong;
	char* res = (char*)malloc(sizeof(char) * k);
	int s = 0;
	int* arr = (int*)malloc(sizeof(int) * 2);
	while (fgets(res + s, n, stdin) != NULL)
	{
		s += strlen(res + s);
	}
	int res_len = 0;
	while (res[res_len] != 0)
	{
		res_len++;
	}
	int arr_len = 0;
	char flag = 1;
	int cup = 2;
	for (int i = 0; i < res_len; i++)
	{
		if (res[i] == ' ' || res[i] == '\n')
		{
			res[i] = 0;
			flag = 1;
		}
		else
		{
			if (flag)
			{
				if (cup == arr_len)
				{
					cup *= 2;
					arr = (int*)realloc(arr, cup * sizeof(int));
				}
				arr[arr_len] = i;
				arr_len += 1;
				flag = 0;
			}
		}
	}
	music(&newsong, arr, arr_len, res);
	savefile(outfile, newsong.data, newsong.samplerate, newsong.datacount);
	free(res);
	free(arr);
	free(outfile);
	free(inputfile);
	fclose(f);
	return 0;
}