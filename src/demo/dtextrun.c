/*
 * C128 CP/M real time key press decode.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#include <cia.h>
#include <console.h>
#include <hitech.h>
#include <screen.h>
#include <stdio.h>
#include <sys.h>

static char *LINE_80 =
		"01234567890123456789012345678901234567890123456789012345678901234567890123456789";
static char *LINE_40 = "0123456789012345678901234567890123456789";

/*
 * CP/M print speed.
 */
ushort cpmPrint(ushort lines) {
	ushort i;
	/* ADM-3A clear-home cursor */
	putchar(0x1a);
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printf(LINE_80);
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * CP/M print speed.
 */
ushort cpmScroll(ushort lines) {
	ushort i;
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printf("\n");
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VDC print speed.
 */
ushort vdcPrint(console *con, ushort lines) {
	ushort i;
	clearHomeCon(con);
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printCon(con, LINE_80);
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VDC scroll speed.
 */
ushort vdcScroll(console *con, ushort lines) {
	ushort i;
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printLineCon(con, "");
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VIC print speed.
 */
ushort vicPrint(console *con, ushort lines) {
	ushort i;
	clearHomeCon(con);
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printCon(con, LINE_40);
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VIC scroll speed.
 */
ushort vicScroll(console *con, ushort lines) {
	ushort i;
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printLineCon(con, "");
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VDC color print speed.
 */
ushort vdcColor(console *con, ushort lines) {
	ushort i;
	con->colorOn = 1;
	con->color = scrLightGreen;
	clearHomeCon(con);
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printCon(con, LINE_80);
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	con->colorOn = 0;
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VDC color scroll speed.
 */
ushort vdcColorScroll(console *con, ushort lines) {
	ushort i;
	con->colorOn = 1;
	con->color = scrLightGreen;
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printLineCon(con, "");
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	con->colorOn = 0;
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VIC color print speed.
 */
ushort vicColor(console *con, ushort lines) {
	ushort i;
	con->colorOn = 1;
	con->color = scrLightGreen;
	clearHomeCon(con);
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printCon(con, LINE_40);
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	con->colorOn = 0;
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * VIC color scroll speed.
 */
ushort vicColorScroll(console *con, ushort lines) {
	ushort i;
	con->colorOn = 1;
	con->color = scrLightGreen;
	/* Timer A counts milliseconds up to 65535 times or ~65 seconds */
	initCiaTimer(ciaMs, 0xffff);
	outp(cia2CtrlRegA, ciaCpuCont);
	for (i = 0; i < lines; i++) {
		printLineCon(con, "");
	}
	outp(cia2CtrlRegA, ciaStopTimer);
	con->colorOn = 0;
	return 0xffff - (inp(cia2TimerBHi) * 256 + inp(cia2TimerBLo));
}

/*
 * Run demo.
 */
void runTextDemo(console *vicCon, console *vdcCon, ushort cpmPrintMs, ushort cpmScrollMs, ushort lines) {
	char str[40];
	ushort vdcPrintMs = vdcPrint(vdcCon, lines);
	ushort vdcScrollMs = vdcScroll(vdcCon, lines);
	ushort vicPrintMs = vicPrint(vicCon, lines);
	ushort vicScrollMs = vicScroll(vicCon, lines);
	ushort vdcColPrintMs = vdcColor(vdcCon, lines);
	ushort vdcColScrollMs = vdcColorScroll(vdcCon, lines);
	ushort vicColPrintMs = vicColor(vicCon, lines);
	ushort vicColScrollMs = vicColorScroll(vicCon, lines);
	clearHomeCon(vicCon);
	sprintf(str, "CP/M printf         %04u ms per line", (cpmPrintMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "CP/M scroll         %04u ms per line", (cpmScrollMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VDC printCon        %04u ms per line", (vdcPrintMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VDC scrollCon       %04u ms per line", (vdcScrollMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VDC printCon color  %04u ms per line", (vdcColPrintMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VDC scrollCon color %04u ms per line", (vdcColScrollMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VIC printCon        %04u ms per line", ((vicPrintMs * 2) / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VIC scrollCon       %04u ms per line", (vicScrollMs / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VIC printCon color  %04u ms per line", ((vicColPrintMs * 2) / lines));
	printLineCon(vicCon, str);
	sprintf(str, "VIC scrollCon color %04u ms per line", (vicColScrollMs / lines));
	printLineCon(vicCon, str);
	waitKey(vicCon->scr);
}