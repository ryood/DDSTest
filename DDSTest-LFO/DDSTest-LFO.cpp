// DDSTest-LFO.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

/*********************************************************
phaseRegister   : 16bit
tunigWord       : 16bit
lookupTable     : 16bit (use 12bit)

Phase Modulation LFO
lfoAmount		: 8bit

Sampling Rate   : 15625Hz
**********************************************************/

#define SAMPLE_CLOCK	15625	// 15.625kHz
#define LFO_CLOCK		3125	// 3.125kHz
#define TABLE_SIZE	    0x0400  // Lookup Table Size = 1024

uint16_t *lookupTable;

uint16_t phaseRegister;
uint16_t tuningWord;
int16_t  waveValue;

uint16_t lfoPhaseRegister;
uint16_t lfoTuningWord;
uint8_t  lfoAmount = 255;

// frequency > SAMPLE_CLOCK / 2^16
double frequency = 440.0;
double lfoFrequency =0.5;

int period = 10 * SAMPLE_CLOCK;

int _tmain(int argc, _TCHAR* argv[])
{
	_setmode(_fileno(stdout), _O_BINARY);

	// Lookup Table の作成
	lookupTable = (uint16_t*)malloc(TABLE_SIZE * sizeof(uint16_t));

	// Sine Wave
	for (int i = 0; i < TABLE_SIZE; i++) {
		lookupTable[i] = (sin(2.0 * M_PI * i / TABLE_SIZE) / 2 + 0.5) * 0x0FFF;
		//printf("%d\n", lookupTable[i]);
	}
	//printf("\n*********************\n\n");

	lfoTuningWord = lfoFrequency * pow(2.0, 16) / LFO_CLOCK;
	//printf("lfoTuningWord: %d\n", lfoTuningWord);

	tuningWord = frequency * pow(2.0, 16) / SAMPLE_CLOCK;
	//printf("tuningWord: %d\n", tuningWord);

	phaseRegister = 0x0000;
	lfoPhaseRegister = 0x0000;

	for (int i = 0; i < period; i++) {
		uint16_t index;
		uint16_t lfoIndex;
		int16_t  lfoValue = 0;
		int16_t  waveValue;

		if (i % (SAMPLE_CLOCK / LFO_CLOCK) == 0) {

			lfoPhaseRegister += lfoTuningWord;
			//printf("lfoPhaseRegister: %d\n", lfoPhaseRegister);

			lfoIndex = lfoPhaseRegister >> 6;
			//printf("lfoIndex: %d\n", lfoIndex);

			// lookupTable(12bit) * lfoAmount(8bit) : 20bit -> 16bit
			lfoValue = (((int32_t)lookupTable[lfoIndex]) - 2048) * lfoAmount >> 4;
			//printf("lfoValue: %d ->\t", lfoValue);

			// tuningWord(16bit) * lfoValue(15bit + 1bit) : (31bit + 1bit) -> 16bit
			lfoValue = (((int32_t)tuningWord) * lfoValue) >> 15;
			//printf("%d\n", lfoValue);
		
		}

		phaseRegister += tuningWord + lfoValue;
		//printf("phaseRegister: %d\n", phaseRegister);

		index = phaseRegister >> 6;
		//printf("index: %d\n", index);

		waveValue = (lookupTable[index] << 4) - 0x8000;
		//printf("%d\n", waveValue);

		// 16bit長の raw データとして出力
		fwrite(&waveValue, sizeof(waveValue), 1, stdout);
	}

	free(lookupTable);

	return 0;
}

