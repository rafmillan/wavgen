#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include "hash.h"
#include "notes.h"

#define WAV_HEADER_LEN 44

#define DEFAULT_INPUT_NAME "s_input.txt"
#define DEFAULT_FILE_NAME "result.wav"
#define MAX_FILE_NAME_SIZE 10

#define DEFAULT_DURATION 5
#define DEFAULT_AMPLITUDE 1000
#define DEFAULT_SAMPLE_RATE 8000

#define SAMPLE_RATE_CD 44100
#define SAMPLE_RATE_DAT 48000
#define SAMPLE_RATE_MAX 768000
#define SAMPLE_RATE_MIN 3000

#define AMPLITUDE_MAX 32767

#define MAX_BEAT_COUNT 8
#define MAX_CHORD_SIZE 8

#define KEY_BEGIN "begin"
#define KEY_END "end"
#define KEY_CHORD "chord"


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

int generateWaveFromInput(short int *buff, int buffsize, char **beats, int numBeats, int sampleRate, int amp) {
    int buffIdxPerBeat = buffsize / numBeats;
    printf("buffer slices per beat: %d (%d/%d)\n", buffIdxPerBeat, buffsize, numBeats);
    
    int buffIdx = 0;
    for (int noteIdx = 0; noteIdx < numBeats; noteIdx++) {
        float freq, period;
        if (beats[noteIdx] == NULL) {
            printf("writing rest (0.0Hz)...\n");
            freq = 0.0;
        } else {
            freq = get(beats[noteIdx]);
            printf("writing %s (%fHz)...\n", beats[noteIdx], freq);
        }

        period = (2 * M_PI * freq) / sampleRate;
        for (int i = 0; i < buffIdxPerBeat; i++) {
            buff[buffIdx] = (short int)(amp * cos(period * buffIdx));
            buffIdx++;
        }
    }
}

float getPlaythroughPercent(int spb, int curr) {
    float pct = (float)curr / (float)spb;
    return pct;
}

float getCurve(float x) {
    float v = 2.0f * sqrt((float)(powf(0.5f, 2) - powf((x - 0.5f), 2)));
    return v;
}

float expDecay(float a, int x) {
    return pow(a, x);
}

float harmonic(int n) {
    if (n <= 1) return 1;
    float res = ((float)(harmonic(n - 1)) + (1 / (float)n));
    return res;
}

int writeNote(short int *buff, int *buffIdx, int buffsize, int spb, float freq, int sampleRate, int amp) {
    if (*buffIdx > (buffsize - 1)) {
        fprintf(stderr, "Error: buffindex > buffsize!");
        return -1;
    }
        
    float period = (2 * M_PI * freq) / sampleRate;
    int vol = amp;
    for (int i = 0; i < spb; i++) {
        float pct = getPlaythroughPercent(spb, i); 
        vol = amp * getCurve(pct); 
        buff[*buffIdx] = (short int)(vol * cos(period * (*buffIdx)));
        //for (int j = 1; j <= 6; j++) {
        //    float period = (2 * M_PI * (freq * j)) / sampleRate;
        //    buff[*buffIdx] += (short int)((vol * expDecay((float)0.8, j)) * cos(period * i));
        //}

        (*buffIdx)++;
    }

    return 0;
}

int writeChord(short int *buff, int *buffIdx, int buffsize, int spb, float *freqs, int freqCount, int sampleRate, int amp) {
    if (*buffIdx > (buffsize - 1)) {
        fprintf(stderr, "Error: buffindex > buffsize!");
        return -1;
    }
    
    int startIdx = *buffIdx;
    int vol = amp;
    for (int i = 0; i < freqCount; i++) {
        *buffIdx = startIdx;
        float period = (2 * M_PI * freqs[i]) / sampleRate;
        for (int j = 0; j < spb; j++) {
            float pct = getPlaythroughPercent(spb, j); 
            vol = amp * getCurve(pct);
            buff[*buffIdx] += (short int)(vol * cos(period * (j)));
            (*buffIdx)++;
        }
    }
}

int generateWave(short int *buff, float freq, int amp, int sampleRate, int buffsize) {
    for (int i = 0; i < buffsize; i++) {
        for (int j = 1; j <= 6; j++) {
            float period = (2 * M_PI * (freq * j)) / sampleRate;
            buff[i] += (short int)((amp * expDecay((float)0.7, j)) * cos(period * i));
        }
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

void parseArgs(int argc, char **argv, float *f, char *n, int *d, int *r, short int *v, char *filename, char *input, int *flag_i) {
    int opt;
    int flag_f = 0;
    int flag_n = 0;
    while ((opt = getopt(argc, argv, "f:n:t:r:v:o:i:h")) != -1) {
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
            case 'o':
                if (strlen(optarg) > MAX_FILE_NAME_SIZE || optarg == NULL) {
                    fprintf(stderr, "Error: Invalid file name (max 10 characters)\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(filename, optarg);
                strcat(filename, ".wav");
                break;
            case 'i':
                if (strlen(optarg) > MAX_FILE_NAME_SIZE || optarg == NULL) {
                    fprintf(stderr, "Error: Invalid file name (max 10 characters)\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(input, optarg);
                *flag_i = 1;
                break;
            case 'h':
                printf("Usage: ./wav_gen [-f <freq> | -n <note>] [-t <time>] [-r <rate>] [-v <volume>]  [-o <output file name>]\n");
                printf("Use -f to specify frequency in Hz OR -n to select a note, eg: C4.\n");
                printf("Use -t to specify duration in seconds.\n");
                printf("Use -r to specify a sample rate.\n");
                printf("Use -v to specify a volume level.\n");
                printf("Use -o to specify a custom output file name (max 10 characters).\n");
                exit(EXIT_SUCCESS);
                break;
            case '?':
                if (optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument [tone frequency (Hz)].\n", optopt);
                else if (isprint(optopt))
                    fprintf (stderr, "Unknown option `-%c', use -h for help.\n", optopt);
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

int fileExists(char *filename)
{
    struct stat buffer;   
    return (stat(filename, &buffer) == 0);
}

int main(int argc, char** argv) {
    int opt;
    float freq = (float)C4;
    char note[4];
    int input_flag = 0;
    char input[15] = DEFAULT_INPUT_NAME;
    char filename[15] = DEFAULT_FILE_NAME;
    int _duration = DEFAULT_DURATION;
    int _sample = DEFAULT_SAMPLE_RATE;
    short int _amplitude = DEFAULT_AMPLITUDE;

    // Initialize Hash Table with Notes
    initHash();

    parseArgs(argc, argv, &freq, note, &_duration, &_sample, &_amplitude, filename, input, &input_flag);

    if (input_flag) {
        printf("Reading from file: %s\n", input);
        if (!fileExists(input)) {
            fprintf(stderr, "File does not exists!\n");
            exit(EXIT_FAILURE);
        }

        FILE *inputfp;
        inputfp = fopen(input, "r");
        if (inputfp == NULL) {
            fprintf(stderr, "Error opening file!\n");
            exit(EXIT_FAILURE);
        }

        int lineNum = 1;
        int numMeasures, beatsPerMeasure, msPerBeat;
        char beginKey[] = KEY_BEGIN;
        char beginFlag[6];
        char chordKey[] = KEY_CHORD;
        char chordFlag[6];

        // Read numMeasures
        if (fscanf(inputfp, "%d", &numMeasures) != 1) {
            fprintf(stderr, "Error reading measure to play!\n");
            exit(EXIT_FAILURE);
        }
        lineNum++;

        // Read bpm
        if  (fscanf(inputfp, "%d", &beatsPerMeasure) != 1) {
            fprintf(stderr, "Error reading beats per measure!\n");
            exit(EXIT_FAILURE);
        }
        lineNum++;

        // Read msPerBeat
        if (fscanf(inputfp, "%d", &msPerBeat) != 1) {
            fprintf(stderr, "Error reading ms per beat!\n");
            exit(EXIT_FAILURE);
        }
        lineNum++;

        // Read begin token
        if (fscanf(inputfp, "%s", beginFlag) != 1) {
            fprintf(stderr, "Error reading begin token!\n");
            exit(EXIT_FAILURE);
        } else if(strcmp(beginFlag, beginKey)) {
            printf("Error (%d): Expected %s, actual %s\n", lineNum, beginKey, beginFlag);
            exit(EXIT_FAILURE);
        }
        lineNum++;

        const int headerSize = sizeof(struct WavHeader);
        const int sampleRate = _sample;
        const int duration = (msPerBeat * beatsPerMeasure * numMeasures) / 1000;
        const int buffsize = duration * sampleRate;
        int totalBeats = numMeasures * beatsPerMeasure;
        int buffIdxPerBeat = buffsize / totalBeats;
        short int buff[buffsize];
        memset(buff, 0, sizeof(buff));

        printf("measures to play: %d\n", numMeasures);
        printf("beats per measure: %d\n", beatsPerMeasure);
        printf("ms per beat: %d\n", msPerBeat);
        printf("total beats: %d\n", totalBeats);
        printf("total duration: %ds\n", duration);
        printf("buffsize: %d\n\n", buffsize);

        int beatCount = 0;
        char _note[4];
                
        //Read Notes/Chords
        char line[512];
        char *token;
        char *delim = " \n";
        int buffIndex = 0;

        while (fgets(line, sizeof(line), inputfp)) {
            if (!strcmp(line, "\n")) {
                lineNum++;
                continue;
            }

            // Strip \n
            line[strcspn(line, "\n")] = '\0';

            // Check if chord
            if (strncmp(line, KEY_CHORD, 5) == 0) {
                int _beats = 0;
                int _noteCount = 0;
                char chord[MAX_CHORD_SIZE][4];
                float chordFreqs[MAX_CHORD_SIZE] = {0.0};

                for (int i = 0; i < MAX_CHORD_SIZE; i++) {
                    strcpy(chord[i], "ABC");
                }

                // Scan for notes in chord
                token = strtok(line, delim);
                token = strtok(NULL, delim);
                while (token != NULL) {
                    if (isNumber(token)) {
                        _beats = atoi(token);
                        beatCount += _beats;
                    } else {
                        if (get(token) < 0) {
                            fflush(stdout);
                            printf("\n");
                            fprintf(stderr, "Error (%d): Invalid Note (%s)!\n", lineNum, token);
                            exit(EXIT_FAILURE);
                        } 
                        strcpy(chord[_noteCount], token);
                        chordFreqs[_noteCount] = get(token);
                        _noteCount++;
                    }
                    token = strtok(NULL, delim);
                }
                for (int i = 0; i < _beats; i++) {
                    printf("writing ");
                    for (int j = 0; j < _noteCount; j++) {
                        printf("%s (%.2f)", chord[j], chordFreqs[j]);
                    }
                    printf("\n");
                    writeChord(buff, &buffIndex, buffsize, buffIdxPerBeat, chordFreqs, _noteCount, sampleRate, _amplitude);
                }
            } else {
                // Assume single note
                int _beats = 0;
                float freq = 0.0;
                token = strtok(line, delim); 
                while (token != NULL) {
                    if (isNumber(token)) {
                        _beats = atoi(token);
                        beatCount += _beats;
                    } else {
                        if (get(token) < 0) {
                            fflush(stdout);
                            printf("\n");
                            fprintf(stderr, "Error (%d): Invalid Note (%s)!\n", lineNum, token);
                            exit(EXIT_FAILURE);
                        } else {
                            strcpy(_note, token);
                            freq = get(_note);
                        }
                    }
                    token = strtok(NULL, delim);
                }
                for (int i = 0; i < _beats; i++) {
                    printf("writing %s (%.2f)...\n", _note, freq);
                    writeNote(buff, &buffIndex, buffsize, buffIdxPerBeat, freq, sampleRate, _amplitude);
                }
            }
            lineNum++;
        }
        fclose(inputfp);

        if (beatCount > (numMeasures * beatsPerMeasure)) {
            fprintf(stderr, "Error: total input of %d beats is greater than total number of beats available (%d)!\n", beatCount, (numMeasures * beatsPerMeasure));
            exit(EXIT_FAILURE);
        }
        
        //Create .wav
        struct WavHeader wavHeader;
        generateHeader(&wavHeader, 1, sampleRate, buffsize);
        generateFile(filename, &buff, buffsize, &wavHeader, headerSize);
        printf("\nDone!\n");

    } else {
        const int headerSize = sizeof(struct WavHeader);
        const int sampleRate = _sample;
        const int duration = _duration;
        const int buffsize = duration * sampleRate;
        short int buff[buffsize];
        memset(buff, 0, sizeof(buff));

        // Create .wav header
        struct WavHeader wavHeader;
        generateHeader(&wavHeader, 1, sampleRate, buffsize);

        // Generate tone signal
        generateWave(buff, freq, _amplitude, sampleRate, buffsize);

        // Create .wav file
        printf("Generating %s...\n[Freq: %.2fHz, Duration: %ds, Sample Rate: %d, Volume: %d]\n", filename, freq, duration, sampleRate, _amplitude);
        generateFile(filename, &buff, buffsize, &wavHeader, headerSize);
    }

    cleanup();

    return 0;
}
