/*
 * C128 CP/M real time key press decode.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <console.h>
#include <screen.h>
#include <stdio.h>
#include "demo.h"

/*
 * Run key scan demo.
 */
void runKeyDemo(console *con) {
	screen *scr = con->scr;
	char str[40];
	printWrapCon(con,
			"Low level key scan of standard and extended keyboard. You can also "
					"decode unshifted and shifted characters. CIA 1 interrupts are "
					"disabled, so as not to disrupt the key scan.");
	printLineCon(con, "");
	printLineCon(con, "");
	sprintf(str, "Chr mem: %04x", scr->chrMem);
	printLineCon(con, str);
	sprintf(str, "Scr mem: %04x", scr->scrMem);
	printLineCon(con, str);
	waitKey(scr);
	keyboard(scr);
	readLine(con);
}
