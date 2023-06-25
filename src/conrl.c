/*
 * C128 CP/M console read line.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <cia.h>
#include <console.h>
#include <screen.h>
#include <stdlib.h>
#include <sys.h>
#include <vic.h>

/*
 * Handle backspace for editing.
 */
void backSpace(console *con) {
	ushort scrOfs = offsetCon(con) - 1;
	char printBuf[3];
	printBuf[0] = con->curChar;
	printBuf[1] = ' ';
	printBuf[2] = 0;
	/* Calculate cursor position for backspace */
	setCurCon(con, scrOfs);
	con->curOn = 0;
	printCon(con, printBuf);
	con->curOn = 1;
	/* Calculate new cursor position */
	setCurCon(con, scrOfs);
}

/*
 * Use screen memory as simple input line. Only backspace supported, but insert
 * and delete could be added later.
 */
char* readLineCon(console *con, uchar len) {
	screen *scr = con->scr;
	char *str, printBuf[2];
	uchar strLen = 0, keyVal, lastKeyVal = 0, curOn = con->curOn, i;
	printBuf[1] = 0;
	/* Make sure cursor is on for input */
	con->curOn = 1;
	printBuf[0] = con->curChar;
	(scr->print)(scr, con->curX, con->curY, printBuf);
	do {
		keyVal = decodeKey();
		/* Debounce if current key equals last key */
		if (keyVal == lastKeyVal) {
			i = 0;
			do {
				/* ~1/60th second delay */
				while (inp(vicRaster) != 0x00)
					;
				keyVal = decodeKey();
				i++;
			} while ((keyVal == lastKeyVal) && (i < 8));
		}
		lastKeyVal = keyVal;
		/* Decoded key? */
		if (keyVal != 0x00) {
			/* Backspace? */
			if (keyVal == 0x7f) {
				if (strLen > 0) {
					strLen--;
					backSpace(con);
				}
			} else {
				if (keyVal != 0x0d && strLen < len) {
					strLen++;
					printBuf[0] = keyVal;
					printCon(con, printBuf);
				}
			}
		}
	} while (keyVal != 0x0d);
	/* Figure out string length based on current screen offset */
	str = (char*) malloc(strLen + 1);
	/* Screen to string */
	(scr->copyScrToStr)(scr, (con->curY * scr->scrWidth) + con->curX - strLen,
			str, strLen);
	/* Remove cursor from end of input */
	(scr->print)(scr, con->curX, con->curY, " ");
	con->curOn = curOn;
	return str;
}