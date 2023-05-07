/*
 * C128 CP/M 8564/8566 VIC-IIe screen functions.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <hitech.h>
#include <screen.h>
#include <vic.h>

/*
 * Clear color memory. Color memory is port mapped.
 */
void clearVicCol(screen *scr, uchar c) {
	fillVicMemCol((ushort) scr->scrColMem, scr->scrSize, c);
}
