/*
	Copyright (c) 2016 CurlyMo <curlymoo1@gmail.com>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>   
	
#include "../../soc/soc.h"
#include "../../wiringX.h"	
#include "../platform.h"	
#include "bananapim2.h"			

struct platform_t *bananapim2 = NULL;

/*
3v|5v
 8|5v
 9|0v
 7|15
0v|16
 0| 1
 2|0v
 3| 4
3v| 5
12|0v
13| 6
14|10
0v|11

0v|0v
21|0v
22|23
24|0v
25|26
27|28
0v|29
*/

static int map[] = {
	/* 	PG7,	PH10,	PG6		PG9		*/
			113,	126,	112,	115,
	/*	PH11, PH12,	PG8,	PH9		*/
			127,	128,	114,	125,
	/*  PH19, PH18,	PG13,	PG12	*/
			135,	134,	119,	118,
	/* 	PG15,	PG16,	PG14,	 PE4	*/
			121,	122,	120,	 88,
	/* 	PE5  										*/
			 89,	 -1,   -1,   -1,
	/* 				PB0,	 PB1,	 PB7	*/
			 -1,	 28,	 29,	 35,
	/* 	 PB2, PB3,	PE6,	PB4 	*/
			 30,	 31,	 90,	 32,
	/* 	PE7,	PM2,	PG10,	PG11	*/
			 91,	155,	116,	117
			
};

// SYSFS GPIO 132 LED D3 (red)

static int bananapiM2ValidGPIO(int pin) {
	if(pin >= 0 && pin < (sizeof(map)/sizeof(map[0]))) {
		if(map[pin] == -1) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

static int bananapiM2PinMode(int i, enum pinmode_t mode) {
	if(map[i] == -1) {
		return -1;
	}
	if((i == 0 || i == 1) && (mode != PINMODE_OUTPUT)) {
		wiringXLog(LOG_ERR, "The %s pin %d can only be used as output", bananapim2->name, i);
		return -1;
	}
	return bananapim2->soc->pinMode(i, mode);
}

static int bananapiM2DigitalWrite(int i, enum digital_value_t value) {
	if(map[i] == -1) {
		return -1;
	}	
	return bananapim2->soc->digitalWrite(i, value);	
}

void bananapiM2Init(void) {
	if((bananapim2 = malloc(sizeof(struct platform_t))) == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(EXIT_FAILURE);
	}
	bananapim2->nralias = 1;
	if((bananapim2->name = malloc(bananapim2->nralias*sizeof(char *))) == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(EXIT_FAILURE);
	}
	if((bananapim2->name[0] = strdup("bananapi_m2")) == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(EXIT_FAILURE);
	}

	bananapim2->soc = soc_get("Allwinner", "A31s");
	bananapim2->soc->setMap(map);

	bananapim2->digitalRead = bananapim2->soc->digitalRead;
	bananapim2->digitalWrite = &bananapiM2DigitalWrite;
	bananapim2->pinMode = &bananapiM2PinMode;
	bananapim2->setup = bananapim2->soc->setup;

	bananapim2->isr = NULL;
	bananapim2->waitForInterrupt = NULL;

	bananapim2->selectableFd = bananapim2->soc->selectableFd;
	bananapim2->gc = bananapim2->soc->gc;

	bananapim2->validGPIO = &bananapiM2ValidGPIO;

	platform_register(bananapim2);
}