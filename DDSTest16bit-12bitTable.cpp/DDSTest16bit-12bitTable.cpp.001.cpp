// DDSTest16bit-12bitTable.cpp.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>

#define _USE_MATH_DEFINES
#include <math.h>

/*********************************************************
phaseRegister   : 16bit
tunigWord       : 16bit
lookupTable     : 16bit (use 12bit)
**********************************************************/

#define SAMPLE_CLOCK	44100	// 44.1kHz
#define TABLE_SIZE	    0x0400  // Lookup Table Size = 1024

uint16_t phaseRegister;
uint16_t tuningWord;
int16_t waveValue;
uint16_t *lookupTable;

double frequency = 440.0;  // 440Hz
int period = 1323000;      // 30s
//int period = 128;

int _tmain(int argc, _TCHAR* argv[])
{
	_setmode(_fileno(stdout), _O_BINARY);

	// Lookup Table の作成
	lookupTable = (uint16_t*)malloc(TABLE_SIZE * sizeof(uint16_t));
	for (int i = 0; i < TABLE_SIZE; i++) {
		lookupTable[i] = (sin(2.0 * M_PI * i / TABLE_SIZE) / 2 + 0.5) * 0x0FFF;
		//printf("%d\n", lookupTable[i]);
	}
	//printf("\n*********************\n");


	tuningWord = frequency * pow(2.0, 16) / SAMPLE_CLOCK;
	//printf("tuningWord: %x\n", tuningWord);

	phaseRegister = 0x0000;
	for (int i = 0; i < period; i++) {
		int index;

		phaseRegister += tuningWord;
		//printf("phr: %x\n", phaseRegister);

		index = phaseRegister >> 6;
		//printf("index: %d\n", index);

		//printf("%d\n", lookupTable[index]);

		waveValue = (lookupTable[index] << 4) - 0x8000;

		//printf("%d\n", waveValue);

		// 16bit長の raw データとして出力
		fwrite(&waveValue, sizeof(waveValue), 1, stdout);
	}

	free(lookupTable);

	return 0;
}
