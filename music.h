#pragma once
typedef struct track {
	int* data;
	int datacount;
	int datacup;
	int samplerate;
}track;
void music(track* song, int* argv, int argc, char* res);