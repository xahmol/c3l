/*
 * C128 CP/M C Library C3L
 *
 * Play 4 bit PCM files with nibble swapping feature.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <cia.h>
#include <cpm.h>
#include <hitech.h>
#include <stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys.h>

/*
 Display program help.
 */
void dispHelp() {
	puts("\nplaypcm4 {U:D:}filespec hertz {sn}");
	puts("playpcm4 filename.raw 8000 sn (swap nibbles before playing)");
	puts("playpcm4 filename.raw 15000   (no nibble swap)");
}

/*
 Set CIA 2 TOD clock.
 */
void setCiaTod(uchar hour, uchar min, uchar sec, uchar tenth) {
	/* Bit 7 = 0 sets TOD clock */
	outp(cia2CtrlRegB, inp(cia2CtrlRegB) & 0x7f);
	/* CIA 2 TOD hour */
	outp(cia2TodHrs, hour);
	/* CIA 2 TOD minute */
	outp(cia2TodMin, min);
	/* CIA 2 TOD second */
	outp(cia2TodSec, sec);
	/* CIA 2 TOD tenth second */
	outp(cia2TodTen, tenth);
}

/*
 * Convert bcd byte to 2 char base 10.
 */
void todToChar(uchar bcd, char *str) {
	str[0] = (bcd >> 4) + 48;
	str[1] = (bcd & 0x0f) + 48;
}

/*
 Display current time in SS:TT format using CIA 2's TOD clock.
 */
void dispTime() {
	char todStr[6];
	todToChar(inp(cia2TodSec), &todStr[0]);
	todStr[2] = ':';
	todToChar(inp(cia2TodTen), &todStr[3]);
	todStr[5] = '\0';
	printf("%s\n", todStr);
}

/*
 Return file size or 0 for error.
 */
ulong getFileSize(char *fileName) {
	struct stat statRec;
	if (stat(fileName, &statRec) == 0) {
		return statRec.st_size;
	} else {
		return 0;
	}
}

/*
 Swap nibbles in buffer for inverted RAWs.
 */
void swapNibbles(uchar *buffer, ushort len) {
	ushort i;
	printf("Swapping nibbles, ");
	setCiaTod(0, 0, 0, 0);
	for (i = 0; i < len; i++)
		buffer[i] = (buffer[i] << 4) | (buffer[i] >> 4);
	dispTime();
}

/*
 Clear ICR and start timer A in continuous mode using Hz value.
 */
void startTimer(ushort hz) {
	/* ciaMs is ~1 KHz */
	ushort timerA = ciaMs / (hz / 1000);
	/* CIA 2 Timer A lo */
	outp(cia2TimerALo, (uchar) timerA);
	/* CIA 2 Timer A hi */
	outp(cia2TimerAHi, (uchar) (timerA >> 8));
	/* Start CIA 2 Timer A */
	outp(cia2CtrlRegA, ciaCpuCont);
}

/*
 Play sample from buffer.
 */
void play(uchar *buffer, ushort len, ushort hz) {
	/* Start HZ timer */
	startTimer(hz);
	printf("Playing, ");
	setCiaTod(0, 0, 0, 0);
	/* Play sample */
	playPcm4Sid(buffer, len);
	dispTime();
}

/*
 Load file into buffer.
 */
void load(char *fileName, ulong fileSize, uchar *buffer) {
	FILE *rawFile;
	if ((rawFile = fopen(fileName, "rb")) != NULL) {
		printf("\nReading %s, %d bytes, ", fileName, fileSize);
		setCiaTod(0, 0, 0, 0);
		fread(buffer, sizeof(uchar), fileSize, rawFile);
		fclose(rawFile);
		dispTime();
	} else
		puts("\nUnable to open file.");
}

/*
 * Main function.
 */
main(int argc, char *argv[]) {
	uchar *buffer;
	ushort minHz = 3999, maxHz = 15001, hz, maxFileSize = 32768;
	ulong fileSize;
	/* Make sure we have 3 or more params */
	if (argc > 2) {
		/* Convert hz param to unsigned short */
		sscanf(argv[2], "%d", &hz);
		/* Check hz range */
		if (hz > minHz && hz < maxHz) {
			/* BDOS return and display error */
			bdos(45, 0x0fe);
			/* Get raw file size */
			fileSize = getFileSize(argv[1]);
			/* Check file size */
			if (fileSize > 0 && fileSize < maxFileSize) {
				/* Allocate buffer */
				buffer = (uchar*) malloc(fileSize);
				if (buffer != NULL) {
					load(argv[1], fileSize, buffer);
					if (strcmp(argv[3], "sn") == 0) {
						swapNibbles(buffer, fileSize);
					}
					initCia();
					play(buffer, fileSize, hz);
					free(buffer);
					doneCia();
				} else
					puts("\nUnable to allocate memory.");
			} else if (fileSize > 0)
				puts("\nFile too large.");
			else
				puts("\nUnable to open file.");
		} else
			puts("\nHz value must be >= 4000 and <= 15000.");
	} else
		dispHelp();
}
