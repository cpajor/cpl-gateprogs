#include "pajordef.h"
#include "gateprogs.h"
#include <stdlib.h>
//
#include <stdio.h>

gate_t* gates_d;
int gates_size = 0;

gateray_t* gaterays_d;
int gaterays_size = 0;

int g_ids = -1;

// l_user.c
extern int selectedId;
extern int clickedId;
extern char moveState;
extern int moveXoffset;
extern int moveYoffset;
extern int selectedType;
extern int targetId;

// w_progs.c
extern int Mposx;
extern int Mposy;
extern int Wwidth;
extern int Wheight;

int selectedX;
int selectedY;
int targetX;
int targetY;
int hookedX;
int hookedY;

int g_getNewId() {
	g_ids++;
	if (g_ids > 42069) g_ids = 0;
	//printf("%s%i\n", "GID: ", g_ids);
	return g_ids;
}

gate_t* g_getGates() {
	return gates_d;
}

int g_getGatesSize() {
	return gates_size;
}

gateray_t* g_getGaterays() {
	return gaterays_d;
}

int g_getGateraysSize() {
	return gaterays_size;
}

gate_t g_getGate() {
	gate_t gat;
	gat.id = g_getNewId();
	gat.inA = -1;
	gat.inB = -1;
	gat.inC = -1;
	gat.inAid = -1;
	gat.inBid = -1;
	gat.inCid = -1;
	gat.rayAid = -1;
	gat.rayBid = -1;
	gat.rayCid = -1;
	//
	gat.outInId = 3;
	return gat;
}

void g_updateRayById(int id, int idg, int x1, int y1) {
	for (int i = 0; i < gaterays_size; i++) {
		gateray_t* g = &gaterays_d[i];
		if (g->id == id) {
			if (g->id1 == idg) {
				g->r_x1 = x1;
				g->r_y1 = y1;
			}
			if (g->id2 == idg) {
				g->r_x2 = x1;
				g->r_y2 = y1;
			}
			return;
		}
	}
}

void g_updateEtc() {
	if (moveState == 1 && clickedId != -1) {
		for (int i = 0; i < gates_size; i++) {
			gate_t* g = &gates_d[i];
			if (g->id == clickedId) {
				g->r_x = Mposx + moveXoffset;
				g->r_y = Mposy + moveYoffset;
				// clipping
				if (g->r_y < 150) g->r_y = 150;
				if (g->r_y > Wheight - 50) g->r_y = Wheight - 50;
				if (g->r_x < 50) g->r_x = 50;
				if (g->r_x > Wwidth - 50) g->r_x = Wwidth - 50;
				//
				if (g->rayAid > -1) {
					g_updateRayById(g->rayAid, g->id, g->r_x, g->r_y);
				}
				if (g->rayBid > -1) {
					g_updateRayById(g->rayBid, g->id, g->r_x, g->r_y);
				}
				if (g->rayCid > -1) {
					g_updateRayById(g->rayCid, g->id, g->r_x, g->r_y);
				}
			}
			//

		}
	}
	if (selectedType > -1) {
		targetX = Mposx;
		targetY = Mposy;
	}
}

void g_updateSelected() {
	if (selectedId > -1) {
		for (int i = 0; i < gates_size; i++) {
			gate_t g = gates_d[i];
			if (g.id == selectedId) {
				selectedX = g.r_x;
				selectedY = g.r_y;
			}
		}
	}
}

void g_addRay(gateray_t gate) {
	if (!gaterays_size) {
		gaterays_d = malloc(sizeof(gateray_t));
		gaterays_d[0] = gate;
		gaterays_size = 1;
		return;
	}
	//
	gateray_t* _tmp = malloc(sizeof(gateray_t) * gaterays_size);
	for (int i = 0; i < gaterays_size; i++) {
		_tmp[i] = gaterays_d[i];
	}
	gaterays_size++;
	gaterays_d = malloc(sizeof(gateray_t) * gaterays_size);
	for (int i = 0; i < gaterays_size - 1; i++) {
		gaterays_d[i] = _tmp[i];
	}
	free(_tmp);
	gaterays_d[gaterays_size - 1] = gate;
}

void g_removeRaysById(int id) {
	if (id == -1) return;
	for (int i = 0; i < gaterays_size; i++) {
		gateray_t* g = &gaterays_d[i];
		if (g->id == id) {
			g->dead = 1;
			break;
		}
	}
	for (int i = 0; i < gates_size; i++) {
		gate_t* g = &gates_d[i];
		if (g->rayAid == id) {
			g->rayAid = -1;
		}
		if (g->rayBid == id) {
			g->rayBid = -1;
		}
		if (g->rayCid == id) {
			g->rayCid = -1;
		}
	}
}

void g_removeRays(int gx, int gy) {
	int id = -1;
	for (int i = 0; i < gaterays_size; i++) {
		gateray_t *g = &gaterays_d[i];
		if ((g->r_x1 == gx && g->r_y1 == gy) || (g->r_x1 == gx && g->r_y1 == gy)) {
			id = g->id;
			g->dead = 1;
			break;
		}
	}
	if (id > -1)
	for (int i = 0; i < gates_size; i++) {
		gate_t* g = &gates_d[i];
		if (g->rayAid == id) {
			g->rayAid = -1;
		}
		if (g->rayBid == id) {
			g->rayBid = -1;
		}
		if (g->rayCid == id) {
			g->rayCid = -1;
		}
	}
}

void g_unpair(int id, int type) {
	if (id == -1) return;
	for (int i = 0; i < gates_size; i++) {
		gate_t* g = &gates_d[i];
		if (g->id == id) {
			if (type == 1) {
				g->inAid = -1;
				g->inA = -1;
				g_removeRaysById(g->rayAid);
			}
			if (type == 2) {
				g->inBid = -1;
				g->inB = -1;
				g_removeRaysById(g->rayBid);
			}
			if (type == 3) {
				g->inCid = -1;
				g->inC = -1;
				g_removeRaysById(g->rayCid);
			}
		}
	}
}

/*
	type: 
		1 - A 
		2 - B 
		3 - C
 */
int g_getVal(int id, int rid) {
	if (rid == -1) return -1;
	if (id == -1) return -1;
	for (int i = 0; i < gates_size; i++) {
		gate_t g = gates_d[i];
		if (g.id == id) {
			if (g.rayAid == rid) {
				return g.inA;
			}
			if (g.rayBid == rid) {
				return g.inB;
			}
			if (g.rayCid == rid) {
				return g.inC;
			}
		}
	}
	return -1;
}

void g_update(int id, int aid) {
	if (id == -1) return;
	for (int i = 0; i < gates_size; i++) {
		gate_t* g = &gates_d[i];
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
		if (g->id == id) {
			if (g->type == 0) { 
				if (g->outInId == 1 && (g->rayAid == aid || aid == -1) && g->rayAid != -1) { // A input
					int v = g_getVal(g->inAid, g->rayAid);
					int h = v > 0 ? 1 : 0;
					g->inA = h;

					g->inB = !h;
					g->inC = !h;
					//
					g_update(g->inBid, g->rayBid);
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayBid == aid || aid == -1) && g->rayBid != -1) { // B input
					int v = g_getVal(g->inBid, g->rayBid);
					int h = v > 0 ? 1 : 0;
					g->inB = h;

					g->inA = !h;
					g->inC = !h;
					//
					g_update(g->inAid, g->rayAid);
					g_update(g->inCid, g->rayCid);
				}
				if (g->outInId == 3 && (g->rayCid == aid || aid == -1) && g->rayCid != -1) { // C input
					int v = g_getVal(g->inCid, g->rayCid);
					int h = v > 0 ? 1 : 0;
					g->inC = h;

					g->inA = !h;
					g->inB = !h;
					//
					g_update(g->inAid, g->rayAid);
					g_update(g->inBid, g->rayBid);
				}
				return;
			}

			if (g->type == 1) {
				if (g->outInId == 1 && (g->rayBid == aid || g->rayCid == aid || aid == -1)) { // A output
					int v1 = g_getVal(g->inBid, g->rayBid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inB = v1;
					g->inC = v2;

					int h = (v1 || v2) > 0 ? 1 : 0;
					g->inA = h;
					//
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayAid == aid || g->rayCid == aid || aid == -1)) { // B output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inA = v1;
					g->inC = v2;

					int h = (v1 || v2) > 0 ? 1 : 0;
					g->inB = h;
					//
					if (id != g->inBid)
						g_update(g->inBid, g->rayBid);
				}
				if (g->outInId == 3 && (g->rayAid == aid || g->rayBid == aid || aid == -1)) { // C output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inBid, g->rayBid);
					g->inA = v1;
					g->inB = v2;

					int h = (v1 || v2) > 0 ? 1 : 0;
					g->inC = h;
					//
					g_update(g->inAid, g->rayAid);
				}
				return;
			}

			if (g->type == 2) {
				if (g->outInId == 1 && (g->rayBid == aid || g->rayCid == aid || aid == -1)) { // A output
					int v1 = g_getVal(g->inBid, g->rayBid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inB = v1;
					g->inC = v2;

					int h = (v1 || v2) > 0 ? 1 : 0;
					g->inA = !h;
					//
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayAid == aid || g->rayCid == aid || aid == -1)) { // B output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inA = v1;
					g->inC = v2;

					int h = (v1 || v2) > 0 ? 1 : 0;
					g->inB = !h;
					//
					g_update(g->inBid, g->rayBid);
				}
				if (g->outInId == 3 && (g->rayAid == aid || g->rayBid == aid || aid == -1)) { // C output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inBid, g->rayBid);
					g->inA = v1;
					g->inB = v2;

					int h = (v1 || v2) > 0 ? 1 : 0;
					g->inC = !h;
					//
					g_update(g->inAid, g->rayAid);
				}
				return;
			}

			if (g->type == 3) {
				if (g->outInId == 1 && (g->rayBid == aid || g->rayCid == aid || aid == -1)) { // A output
					int v1 = g_getVal(g->inBid, g->rayBid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inB = v1;
					g->inC = v2;

					int h = (v1 && v2) > 0 ? 1 : 0;
					g->inA = h;
					//
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayAid == aid || g->rayCid == aid || aid == -1)) { // B output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inA = v1;
					g->inC = v2;

					int h = (v1 && v2) > 0 ? 1 : 0;
					g->inB = h;
					//
					g_update(g->inBid, g->rayBid);
				}
				if (g->outInId == 3 && (g->rayAid == aid || g->rayBid == aid || aid == -1)) { // C output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inBid, g->rayBid);
					g->inA = v1;
					g->inB = v2;

					int h = (v1 && v2) > 0 ? 1 : 0;
					g->inC = h;
					//
					g_update(g->inAid, g->rayAid);
				}
				return;
			}

			if (g->type == 4) {
				if (g->outInId == 1 && (g->rayBid == aid || g->rayCid == aid || aid == -1)) { // A output
					int v1 = g_getVal(g->inBid, g->rayBid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inB = v1;
					g->inC = v2;

					int h = (v1 && v2) > 0 ? 1 : 0;
					g->inA = !h;
					//
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayAid == aid || g->rayCid == aid || aid == -1)) { // B output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inA = v1;
					g->inC = v2;

					int h = (v1 && v2) > 0 ? 1 : 0;
					g->inB = !h;
					//
					g_update(g->inBid, g->rayBid);
				}
				if (g->outInId == 3 && (g->rayAid == aid || g->rayBid == aid || aid == -1)) { // C output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inBid, g->rayBid);
					g->inA = v1;
					g->inB = v2;

					int h = (v1 && v2) > 0 ? 1 : 0;
					g->inC = !h;
					//
					g_update(g->inAid, g->rayAid);
				}
				return;
			}

			if (g->type == 5) {
				if (g->outInId == 1 && (g->rayBid == aid || g->rayCid == aid || aid == -1)) { // A output
					int v1 = g_getVal(g->inBid, g->rayBid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inB = v1;
					g->inC = v2;

					int h = (v1 && !v2) > 0 ? 1 : 0;
					g->inA = h;
					//
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayAid == aid || g->rayCid == aid || aid == -1)) { // B output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inA = v1;
					g->inC = v2;

					int h = (v1 && !v2) > 0 ? 1 : 0;
					g->inB = h;
					//
					g_update(g->inBid, g->rayBid);
				}
				if (g->outInId == 3 && (g->rayAid == aid || g->rayBid == aid || aid == -1)) { // C output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inBid, g->rayBid);
					g->inA = v1;
					g->inB = v2;

					int h = (v1 && !v2) > 0 ? 1 : 0;
					g->inC = h;
					//
					g_update(g->inAid, g->rayAid);
				}
				return;
			}

			if (g->type == 6) {
				if (g->outInId == 1 && (g->rayBid == aid || g->rayCid == aid || aid == -1)) { // A output
					int v1 = g_getVal(g->inBid, g->rayBid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inB = v1;
					g->inC = v2;

					int h = (v1 && !v2) > 0 ? 1 : 0;
					g->inA = !h;
					//
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayAid == aid || g->rayCid == aid || aid == -1)) { // B output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inCid, g->rayCid);
					g->inA = v1;
					g->inC = v2;

					int h = (v1 && !v2) > 0 ? 1 : 0;
					g->inB = !h;
					//
					g_update(g->inBid, g->rayBid);
				}
				if (g->outInId == 3 && (g->rayAid == aid || g->rayBid == aid || aid == -1)) { // C output
					int v1 = g_getVal(g->inAid, g->rayAid);
					int v2 = g_getVal(g->inBid, g->rayBid);
					g->inA = v1;
					g->inB = v2;

					int h = (v1 && !v2) > 0 ? 1 : 0;
					g->inC = !h;
					//
					g_update(g->inAid, g->rayAid);
				}
				return;
			}
			//
			//
			if (g->type == 7) {
				if (g->outInId == 1 && (g->rayAid == aid || aid == -1)) { // A input
					int v = g_getVal(g->inAid, g->rayAid);
					int h = v > 0 ? 1 : 0;
					g->inA = h;

					g->inB = h;
					g->inC = h;
					//
					g_update(g->inBid, g->rayBid);
					g_update(g->inCid, g->rayCid);

				}
				if (g->outInId == 2 && (g->rayBid == aid || aid == -1)) { // B input
					int v = g_getVal(g->inBid, g->rayBid);
					int h = v > 0 ? 1 : 0;
					g->inB = h;

					g->inA = h;
					g->inC = h;
					//
					g_update(g->inAid, g->rayAid);
					g_update(g->inCid, g->rayCid);
				}
				if (g->outInId == 3 && (g->rayCid == aid || aid == -1)) { // C input
					int v = g_getVal(g->inCid, g->rayCid);
					int h = v > 0 ? 1 : 0;
					g->inC = h;

					g->inA = h;
					g->inB = h;
					//
					g_update(g->inAid, g->rayAid);
					g_update(g->inBid, g->rayBid);
				}
				return;
			}

			if (g->type == 12) {
				if (g->inAid > -1) g->inA = g_getVal(g->inAid, g->rayAid) > 0 ? 1 : 0;
				if (g->inBid > -1) g->inB = g_getVal(g->inBid, g->rayBid) > 0 ? 1 : 0;
				if (g->inCid > -1) g->inC = g_getVal(g->inCid, g->rayCid) > 0 ? 1 : 0;
			}
		}
	}
}

void g_setoutput(int id, int type) {
	for (int i = 0; i < gates_size; i++) {
		gate_t* g = &gates_d[i];
		if (g->id == id) {
			g->inA = -1;
			g->inB = -1;
			g->inC = -1;
			g->inAid = -1;
			g->inBid = -1;
			g->inCid = -1;
			g->outInId = type;
			g_unpair(id, 1);
			g_unpair(id, 2);
			g_unpair(id, 3);
		}
	}
}

char g_check(int sid, int tid, int st, int tt) {
	if (sid < 0 || tid < 0) return;
	int soutid = -1;
	int toutid = -1;
	int stype = -1;
	int ttype = -1;
	for (int i = 0; i < gates_size; i++) {
		gate_t g = gates_d[i];
		if (g.id == sid) {
			stype = g.type;
			soutid = g.outInId;
		}
		if (g.id == tid) {
			ttype = g.type;
			toutid = g.outInId;
		}
	}
	//
	char sinv = stype;

	return 0;
}

void g_remove(int id) {
	if (!gates_size) {
		return;
	}
	//
	g_unpair(id, 1);
	g_unpair(id, 2);
	g_unpair(id, 3);
	//
	gate_t* _tmp = malloc(sizeof(gate_t) * gates_size);
	for (int i = 0; i < gates_size; i++) {
		_tmp[i] = gates_d[i];
	}
	gates_size--;
	gates_d = malloc(sizeof(gate_t) * gates_size);
	int j = 0;
	for (int i = 0; i < gates_size + 1; i++) {
		if (_tmp[i].id == id) {
			continue;
		}
		gates_d[j] = _tmp[i];
		j++;
	}
	free(_tmp);
}

void g_add(gate_t gate) {
	if (!gates_size) {
		gates_d = malloc(sizeof(gate_t));
		gates_d[0] = gate;
		gates_size = 1;
		return;
	}
	gate_t* _tmp = malloc(sizeof(gate_t) * gates_size);
	for (int i = 0; i < gates_size; i++) {
		_tmp[i] = gates_d[i];
	}
	gates_size++;
	gates_d = malloc(sizeof(gate_t) * gates_size);
	for (int i = 0; i < gates_size - 1; i++) {
		gates_d[i] = _tmp[i];
	}
	free(_tmp);
	gates_d[gates_size - 1] = gate;
}

/*
	
	st / tt

	1 - input A
	2 - input B
	3 - input C
	
	selected id, target id, selected type, target type,
*/
void g_pair(int sid, int tid, int st, int tt) {
	if (g_check(sid, tid, st, tt)) return;
	gateray_t gr;
	gr.id = g_getNewId();
	gr.dead = 0;
	g_unpair(sid, st);
	g_unpair(tid, tt);
	for (int i = 0; i < gates_size; i++) {
		gate_t* g = &gates_d[i];
		if (g->id == sid) {
			gr.r_x1 = g->r_x;
			gr.r_y1 = g->r_y;
			gr.id1 = sid;
			gr.type1 = st;
			
			if (st == 1) {
				g->inAid = tid;
				g->rayAid = gr.id;
			}

			if (st == 2) {
				g->inBid = tid;
				g->rayBid = gr.id;
			}

			if (st == 3) {
				g->inCid = tid;
				g->rayCid = gr.id;
			}

		}
		if (g->id == tid) {
			gr.r_x2 = g->r_x;
			gr.r_y2 = g->r_y;
			gr.id2 = tid;
			gr.type2 = tt;

			if (tt == 1) {
				g->inAid = sid;
				g->rayAid = gr.id;
			}
			
			if (tt == 2) {
				g->inBid = sid;
				g->rayBid = gr.id;
			}

			if (tt == 3) {
				g->inCid = sid;
				g->rayCid = gr.id;
			}
			
		}
	}
	
	//printf("PAIR S:%i T:%i ST:%i TT:%i \n", sid, tid, st, tt);

	g_addRay(gr);
}

void g_init0() {
	gates_d = malloc(sizeof(gate_t));
	gaterays_d = malloc(sizeof(gateray_t));
}