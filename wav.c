#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define WAV_HEADER_LEN 44
#define SAMPLE_RATE_CD 44100
#define SAMPLE_RATE_DAT 48000
#define C4_FREQ 256 

// Source: https://docs.fileformat.com/audio/wav/
struct WavHeader {
    char riff[4];
    int32_t fileSize;
    char wave[4];
    char fmt[4];
    int32_t lenFormatData;
    int16_t formatTag;
    int16_t numChannels;
    int32_t sampleRate;
    int32_t bytesPerSec;
    int16_t bytesPerSample;
    int16_t bitsPerSample;
    char data[4];
    int32_t dataSize;
};

int generateHeader(struct WavHeader *header, int numCh, int sampleRate, int buffsize) {
    // Hard-coded strings in header
    strncpy(header->riff, "RIFF", 4);
    strncpy(header->wave, "WAVE", 4);
    strncpy(header->fmt, "fmt ", 4);
    strncpy(header->data, "data", 4);

    // Header Variables
    header->lenFormatData = 16;
    header->formatTag = 1;
    header->numChannels = 1;
    header->sampleRate = sampleRate;
    header->bitsPerSample = 16;
    header->bytesPerSample = header->bitsPerSample / 8 * header->numChannels;
    header->bytesPerSec = header->sampleRate * header->bytesPerSample;
    header->dataSize = buffsize * header->bytesPerSample;
    header->fileSize = header->dataSize + WAV_HEADER_LEN;

    return 0;
}

int generateWave(short int *buff, int freq, int amp, int sampleRate, int buffsize) {
    float period = (2 * M_PI * freq) / sampleRate;
    for (int i = 0; i < buffsize; i++) {
        buff[i] = (short int)(amp * cos(period * i));
    }
}

int generateFile(char *name, const void *buff, int buffSize, struct WavHeader *header, int headerSize) {
    FILE* fp = fopen(name, "w");
    fwrite(header, 1, headerSize, fp);
    fwrite(buff, 2, buffSize, fp);
    fclose(fp);
}

int main(int argc, char** argv) {
    const int headerSize = sizeof(struct WavHeader);
    const int sampleRate = 8000;
    const int duration = 10;
    const int buffsize = duration * sampleRate;
    short int buff[buffsize];
    memset(buff, 0, sizeof(buffsize));

    // Create .wav header
    struct WavHeader wavHeader;
    generateHeader(&wavHeader, 1, sampleRate, buffsize);

    // Generate tone signal
    short int amplitude = 1000;
    generateWave(buff, C4_FREQ, amplitude, sampleRate, buffsize);

    // Create .wav file
    generateFile("result.wav", &buff, buffsize, &wavHeader, headerSize);

    return 0;
}