#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include "hash.h"
#include "notes.h"

#define WAV_HEADER_LEN 44

#define C4_FREQ 256 

#define DEFAULT_DURATION 5
#define DEFAULT_AMPLITUDE 1000
#define DEFAULT_SAMPLE_RATE 8000

#define SAMPLE_RATE_CD 44100
#define SAMPLE_RATE_DAT 48000
#define SAMPLE_RATE_MAX 768000
#define SAMPLE_RATE_MIN 3000

#define AMPLITUDE_MAX 32767

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

int isNumber(char *str) {
    if (str == NULL) return 0;

    int decimal_count = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (!isdigit(str[i])) {
            if (str[i] == '.' && decimal_count == 0) {
                decimal_count++;
            } else {
                return 0;
            }
        }
        i++;
    }

    return 1;
}

void parseArgs(int argc, char **argv, float *f, char *n, int *d, int *r, short int *v) {
    int opt;
    int flag_f = 0;
    int flag_n = 0;
    while ((opt = getopt(argc, argv, "f:n:t:r:v:")) != -1) {
        switch (opt) {
            case 'f':
                if(!isNumber(optarg)) {
                    fprintf (stderr, "Option -%c requires an a number (frequency in Hz).\n", opt);
                    exit(EXIT_FAILURE);
                }
                *f = atof(optarg);
                flag_f = 1;
                break;
            case 'n':
                float ff = get(optarg);
                if (strlen(optarg) >= NOTE_KEY_SIZE || ff < 0) {
                    fprintf(stderr, "Error: Invalid note\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(n, optarg);
                *f = ff;
                flag_n = 1;
                break;
            case 't':
                if(!isNumber(optarg)) {
                    fprintf (stderr, "Option -%c requires an a number (duration time in seconds).\n", opt);
                    exit(EXIT_FAILURE);
                }
                *d = atoi(optarg);
                break;
            case 'r':
                if(!isNumber(optarg)) {
                    fprintf (stderr, "Option -%c requires an a number (sample rate).\n", opt);
                    exit(EXIT_FAILURE);
                } else if (atoi(optarg) < SAMPLE_RATE_MIN) {
                    fprintf (stderr, "Option -%c requires a sample rate >= %d.\n", opt, SAMPLE_RATE_MIN);
                    exit(EXIT_FAILURE);
                } else if (atoi(optarg) > SAMPLE_RATE_MAX) {
                    fprintf (stderr, "Option -%c requires a sample rate <= %d.\n", opt, SAMPLE_RATE_MAX);
                    exit(EXIT_FAILURE);
                }
                *r = atoi(optarg);
                break;
            case 'v':
                if(!isNumber(optarg)) {
                    fprintf (stderr, "Option -%c requires an a number (volume).\n", opt);
                    exit(EXIT_FAILURE);
                } else if (atoi(optarg) > AMPLITUDE_MAX) {
                    fprintf (stderr, "Option -%c requires a amplitude rate <= %d.\n", opt, AMPLITUDE_MAX);
                    exit(EXIT_FAILURE);
                }
                *v = atoi(optarg);
                break;
            case '?':
                if (optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument [tone frequency (Hz)].\n", optopt);
                else if (isprint(optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Usage: %s\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (flag_f && flag_n) {
        fprintf(stderr, "Error: both -f and -n were defined. Only one is allowed per command.\n");
        exit(EXIT_FAILURE);
    }
}

void initHash() {
    inithashtab();
    for (int i = 0; i < NUM_NOTES; i++)
        install(keys[i], vals[i]);
}

int main(int argc, char** argv) {
    int opt;
    float freq = (float)C4_FREQ;
    char note[4];
    int _duration = DEFAULT_DURATION;
    int _sample = DEFAULT_SAMPLE_RATE;
    short int _amplitude = DEFAULT_AMPLITUDE;

    initHash();

    parseArgs(argc, argv, &freq, note, &_duration, &_sample, &_amplitude);

    const int headerSize = sizeof(struct WavHeader);
    const int sampleRate = _sample;
    const int duration = _duration;
    const int buffsize = duration * sampleRate;
    short int buff[buffsize];
    memset(buff, 0, sizeof(buffsize));

    // Create .wav header
    struct WavHeader wavHeader;
    generateHeader(&wavHeader, 1, sampleRate, buffsize);

    // Generate tone signal
    generateWave(buff, freq, _amplitude, sampleRate, buffsize);

    // Create .wav file
    printf("Generating .wav file [Freq: %.2fHz, Duration: %ds, Sample Rate: %d, Volume: %d]\n", freq, duration, sampleRate, _amplitude);
    generateFile("result.wav", &buff, buffsize, &wavHeader, headerSize);
    
    cleanup();

    return 0;
}