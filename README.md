# wavgen
command line .wav file generator

### Usage
To compile and run the program or use makefile provided:

```bash
$ gcc main.c hash.c notes.c -o wav_gen
$ ./wav_gen [-f <freq> | -n <note>] [-t <time>] [-r <rate>] [-v <volume>]  [-o <output file name>] 
```

### Input Flags  
```-f```: Set frequency in Hz.  
```-n```: Select a note (C4, D4, E4, etc.).  
```-t```: Set duration in seconds.  
```-r```: Set sample rate.  
```-v```: Set volume level.
```-o```: output file name (10 character limit).  

### Output
A WAV file named <output_filename>.wav.  

### Limitations  
- Supports sample rates between 3000 samples/second and 768,000 samples/second
- Volume levels must be within the range of 0 to 32,767
