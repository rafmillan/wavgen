# wavgen
command line .wav file generator

### Usage
To compile and run the program or use makefile provided:

```bash
$ gcc main.c hash.c notes.c -o wav_gen
$ ./wav_gen [-i <input file name> | [-f <freq> | -n <note>] [-t <time>]] [-r <rate>] [-v <volume>]  [-o <output file name>] 
```

### Input Flags  
```-i```: Select input file (See format below)  
```-f```: Set frequency in Hz.  
```-n```: Select a note (C4, D4, E4, etc.).  
```-t```: Set duration in seconds.  
```-r```: Set sample rate.  
```-v```: Set volume level.
```-o```: Output file name (10 character limit).  

### Input File Format
- The first three lines specify number of measures, beats per measure and milliseconds per beat, respectively.  
- the 4th line should contain the string "begin" to mark the beggining of the sequence.  
- the following lines contain the notes or chords to play  
  - If palying a single note, the line contains two tokens, separated by whitespace. The first token is the Note, followed by the duration (in beats).  
  - If playing a chord, the line begins the string "chord" and contains the notes in the chord separated by whitespace, followed by the duration (in beats).

#### Example: Playing Single Notes
The following file plays the C3 Major scale, with each note playing for 1 beat each or 500ms.  
There are 2 measures, and 4 beats per measure, resulting in 8 beats total.  
```
2
4
500
begin
C3 1
D3 1
E3 1
F3 1
G3 1
A3 1
B3 1
C4 1
```

### Example: Playing Chords
The following file plays an Em Am B chord progression follwed by single notes (E, B, A).  
```
3
6
1000
begin
chord E4 G4 B4 4
chord A4 C4 E4 2
chord E4 G4 B4 2
chord B4 Eb4 Gb4 4
E4 2
B4 2
A4 2
```
### Output
A WAV file named <output_filename>.wav.  

### Limitations  
- Supports sample rates between 3000 samples/second and 768,000 samples/second
- Volume levels must be within the range of 0 to 32,767
