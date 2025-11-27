#ifndef GATEPROGS_H
#define GATEPROGS_H

/*
	types:
		0 - not
		1 - or
		2 - nor
		3 - and
		4 - nand
		5 - xor
		6 - xnor
		7 - 1to2

		11 - io input
		12 - io output
*/
typedef struct gate_s {
	int r_x;
	int r_y;
	/*
	types:
		0 - not
		1 - or
		2 - nor
		3 - and
		4 - nand
		5 - xor
		6 - xnor
		7 - 1to2

		11 - io input
		12 - io output
	*/
	int type;
	int id;//
	
	// 1 - A
	// 2 - B
	// 3 - C
	// types:
	// 0 to 6 - output
	// 7 - input
	// 11 - input
	// 12 - output
	int outInId;

	// type NOT - only A
	char inA;
	int inAid;
	int rayAid;

	char inB;
	int inBid;
	int rayBid;

	char inC;
	int inCid;
	int rayCid;
} gate_t;

typedef struct gateray_s {
	// src
	int id1;
	int r_x1;
	int r_y1;
	// 1 - A
	// 2 - B
	// 3 - C
	int type1;
	// dst
	int id2;
	int r_x2;
	int r_y2;
	//
	// 1 - A
	// 2 - B
	// 3 - C
	int type2;
	int id;
	char dead;
} gateray_t;

gate_t* g_getGates();

int g_getGatesSize();

gateray_t* g_getGaterays();

int g_getGateraysSize();

void g_process();

void g_refreshRays();

void cpl_display();

#endif