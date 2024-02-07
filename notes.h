#ifndef NOTES_H
#define NOTES_H

#define NUM_NOTES 108
#define NOTE_KEY_SIZE 4

// Source: https://pages.mtu.edu/~suits/notefreqs.html
static const float C0   = 16.35;
static const float Db0  = 17.32;
static const float D0   = 18.35;
static const float Eb0  = 19.45;
static const float E0   = 20.60;
static const float F0   = 21.83;
static const float Gb0  = 23.12;
static const float G0   = 24.50;
static const float Ab0  = 25.96;
static const float A0   = 27.50;
static const float Bb0  = 29.14;
static const float B0   = 30.87;
static const float C1   = 32.70;
static const float Db1  = 34.65;
static const float D1   = 36.71;
static const float Eb1  = 38.89;
static const float E1   = 41.20;
static const float F1   = 43.65;
static const float Gb1  = 46.25;
static const float G1   = 49.00;
static const float Ab1 	= 51.91;
static const float A1	= 55.00;
static const float Bb1 	= 58.27;
static const float B1	= 61.74;
static const float C2	= 65.41;
static const float Db2 	= 69.30;
static const float D2	= 73.42;
static const float Eb2 	= 77.78;
static const float E2	= 82.41;
static const float F2	= 87.31;
static const float Gb2 	= 92.50;
static const float G2	= 98.00;
static const float Ab2 	= 103.83;
static const float A2	= 110.00;
static const float Bb2 	= 116.54;
static const float B2	= 123.47;
static const float C3	= 130.81;
static const float Db3 	= 138.59;
static const float D3	= 146.83;
static const float Eb3 	= 155.56;
static const float E3	= 164.81;
static const float F3	= 174.61;
static const float Gb3 	= 185.00;
static const float G3	= 196.00;
static const float Ab3 	= 207.65;
static const float A3	= 220.00;
static const float Bb3 	= 233.08;
static const float B3	= 246.94;
static const float C4	= 261.63;
static const float Db4 	= 277.18;
static const float D4	= 293.66;
static const float Eb4 	= 311.13;
static const float E4	= 329.63;
static const float F4	= 349.23;
static const float Gb4 	= 369.99;
static const float G4	= 392.00;
static const float Ab4 	= 415.30;
static const float A4	= 440.00;
static const float Bb4 	= 466.16;
static const float B4	= 493.88;
static const float C5	= 523.25;
static const float Db5 	= 554.37;
static const float D5	= 587.33;
static const float Eb5 	= 622.25;
static const float E5	= 659.25;
static const float F5	= 698.46;
static const float Gb5 	= 739.99;
static const float G5	= 783.99;
static const float Ab5 	= 830.61;
static const float A5	= 880.00;
static const float Bb5 	= 932.33;
static const float B5	= 987.77;
static const float C6	= 1046.50;
static const float Db6 	= 1108.73;
static const float D6	= 1174.66;
static const float Eb6 	= 1244.51;
static const float E6	= 1318.51;
static const float F6	= 1396.91;
static const float Gb6 	= 1479.98;
static const float G6	= 1567.98;
static const float Ab6 	= 1661.22;
static const float A6	= 1760.00;
static const float Bb6 	= 1864.66;
static const float B6	= 1975.53;
static const float C7	= 2093.00;
static const float Db7 	= 2217.46;
static const float D7	= 2349.32;
static const float Eb7 	= 2489.02;
static const float E7	= 2637.02;
static const float F7	= 2793.83;
static const float Gb7 	= 2959.96;
static const float G7	= 3135.96;
static const float Ab7 	= 3322.44;
static const float A7	= 3520.00;
static const float Bb7 	= 3729.31;
static const float B7	= 3951.07;
static const float C8	= 4186.01;
static const float Db8 	= 4434.92;
static const float D8	= 4698.63;
static const float Eb8 	= 4978.03;
static const float E8	= 5274.04;
static const float F8	= 5587.65;
static const float Gb8 	= 5919.91;
static const float G8	= 6271.93;
static const float Ab8 	= 6644.88;
static const float A8	= 7040.00;
static const float Bb8 	= 7458.62;
static const float B8	= 7902.13;

static char *keys[NUM_NOTES] = {
    "C0", "Db0", "D0", "Eb0", "E0", "F0", "Gb0", "G0", "Ab0", "A0", "Bb0", "B0",
    "C1", "Db1", "D1", "Eb1", "E1", "F1", "Gb1", "G1", "Ab1", "A1", "Bb1", "B1",
    "C2", "Db2", "D2", "Eb2", "E2", "F2", "Gb2", "G2", "Ab2", "A2", "Bb2", "B2",
    "C3", "Db3", "D3", "Eb3", "E3", "F3", "Gb3", "G3", "Ab3", "A3", "Bb3", "B3",
    "C4", "Db4", "D4", "Eb4", "E4", "F4", "Gb4", "G4", "Ab4", "A4", "Bb4", "B4",
    "C5", "Db5", "D5", "Eb5", "E5", "F5", "Gb5", "G5", "Ab5", "A5", "Bb5", "B5",
    "C6", "Db6", "D6", "Eb6", "E6", "F6", "Gb6", "G6", "Ab6", "A6", "Bb6", "B6",
    "C7", "Db7", "D7", "Eb7", "E7", "F7", "Gb7", "G7", "Ab7", "A7", "Bb7", "B7",
    "C8", "Db8", "D8", "Eb8", "E8", "F8", "Gb8", "G8", "Ab8", "A8", "Bb8", "B8"
};

static float vals[NUM_NOTES] = {
    16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87,
    32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49.0, 51.91, 55.0, 58.27, 61.74,
    65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98.0, 103.83, 110.0, 116.54, 123.47,
    130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0, 196.0, 207.65, 220.0, 233.08, 246.94,
    261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0, 415.3, 440.0, 466.16, 493.88,
    523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.0, 932.33, 987.77,
    1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.0, 1864.66, 1975.53,
    2093.0, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.0, 3729.31, 3951.07,
    4186.01, 4434.92, 4698.63, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040.0, 7458.62, 7902.13
};

#endif