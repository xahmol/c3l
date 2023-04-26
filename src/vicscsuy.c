/*
 * C128 CP/M 8564/8566 VIC-IIe screen functions.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <vic.h>
#include "hitech.h"

/*
 * Scroll line up given y1 and y2 lines. Use words instead of bytes for performance.
 */
void scrollVicUpY(screen *scr, uchar y1, uchar y2) {
	ushort i;
	ushort *scr16 = (ushort*) scr->scrMem;
	ushort scrWidth16 = scr->scrWidth >> 1;
	ushort dest = y1 * scrWidth16;
	ushort source = dest + scrWidth16;
	ushort len = (y2 - y1) * scrWidth16;
	for (i = 0; i < len; i++) {
		scr16[dest + i] = scr16[source + i];
	}
}
