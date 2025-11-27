#include "pajordef.h"
#include "gateprogs.h"
#include <GLFW/glfw3.h>

int selectedId = -1;
int targetId = -1;
int clickedId = -1;
/*
	0 - input C / output
	1 - input A
	2 - input B
*/
int selectedType = -1;
/*
	0 - input C / output
	1 - input A
	2 - input B
*/
int targetType = 0;

//
char targetNot = 0;
char selectedNot = 0;

//
char moveState = 0;
int moveXoffset = 0;
int moveYoffset = 0;

extern void g_updateSelected();

// r_window.c
extern int hoverMenuID;
extern char r_showAllRays;

// w_progs.c
extern int Mposx;
extern int Mposy;
extern int Wwidth;

// l_gates.c
extern int selectedX;
extern int selectedY;
extern int targetX;
extern int targetY;
extern int hookedX;
extern int hookedY;

// l_gates.c
extern void g_remove(int id);
extern gate_t g_getGate();
extern void g_add(gate_t gate);
extern void g_pair(int sid, int tid, int st, int tt);
extern void g_setoutput(int id, int type);
extern void g_update(int id, int aid);

void g_clickedIdM(int cX, int cY) {
	if (cY <= 100) {
		return;
	}
	//
	if (selectedType == -1)
	for (int i = 0; i < g_getGatesSize(); i++) {
		gate_t g = g_getGates()[i];
		//
		if (g.r_x - 50 < cX && g.r_x + 50 > cX && g.r_y - 50 < cY && g.r_y + 50 > cY) {
			selectedId = g.id;
			clickedId = -1;
			//found = 1;
			if (g.type == 0) { // NOT
				selectedNot = 1;
			}
			else {
				selectedNot = 0;
			}
			moveXoffset = -(cX - g.r_x);
			moveYoffset = -(cY - g.r_y);
			return;
		}
	}
	/*
	if (!found && cY > 100) {
		selectedId = -1;
	}
	*/
	selectedId = -1;
	targetId = -1;
	selectedType = -1;
	targetType = -1;
}

void g_clickedId(int cX, int cY) {
	//
	if (cY <= 100) {
		selectedId = -1;
		targetId = -1;
		selectedType = -1;
		targetType = -1;
		//
		if (cX > 0 && cX < 800) {
			moveState = 1;
			gate_t g = g_getGate();
			clickedId = g.id;
			int typ = (int)(cX / 100);
			g.type = typ;
			if (g.type == 0 || g.type == 7) {
				g.outInId = 1;
			}
			g_add(g);
		}
		if (cX > 800 && cX < 900) {
			// output
			moveState = 1;
			gate_t g = g_getGate();
			g.type = 11;
			clickedId = g.id;
			g.inA = 0;
			g.inB = 0;
			g.inC = 0;
			g_add(g);
		}
		if (cX > 900 && cX < 1000) {
			// input
			moveState = 1;
			gate_t g = g_getGate();
			g.type = 12;
			clickedId = g.id;
			g.inA = 0;
			g.inB = 0;
			g.inC = 0;
			g_add(g);
		}
		if (cX > Wwidth - 100) {
			r_showAllRays = !r_showAllRays;
		}
		return;
	}
	/*
	if (selectedId > -1 && selectedType > 0 && targetType > -1) {
		if (targetX < Mposx && targetX + 120 > Mposx && targetY < Mposy && targetY + 20 > Mposy) {
			targetType = 1;
			g_pair(selectedId, targetId, targetType, selectedType);
			selectedId = -1;
			targetId = -1;
			selectedType = -1;
			targetType = -1;
			return;
		}
		if (targetX < Mposx && targetX + 120 > Mposx && targetY + 21 < Mposy && targetY + 40 > Mposy) {
			targetType = 2;
			g_pair(selectedId, targetId, selectedType, targetType);
			selectedId = -1;
			targetId = -1;
			selectedType = -1;
			targetType = -1;
			return;
		}
	}
	*/
	targetType = -1;
	//
	if (selectedId > -1 && selectedType == -1) {
		if (selectedX < Mposx && selectedX + 120 > Mposx && selectedY + 60 < Mposy && selectedY + 80 > Mposy) {
			g_remove(selectedId);
			selectedType = -1;
			selectedId = -1;
			clickedId = -1;
			return;
		}
		for (int i = 1; i < 4; i++) {
			int sb = i + hoverMenuID - 2;
			if (selectedX + 120 < Mposx && selectedX + 220 > Mposx && selectedY + (sb * 20) < Mposy && selectedY + 19 + (sb * 20) - 1 > Mposy) {
				// output
				if (hoverMenuID == 1) {
					selectedType = i;
				}
				if (hoverMenuID == 2) {
					g_setoutput(selectedId, i);
					selectedId = -1;
				}
				return;
			}
		}
	}
	if (selectedId > -1 && selectedType > -1 && targetType == -1 && targetId == -1) {
		//
		for (int i = 0; i < g_getGatesSize(); i++) {
			gate_t g = g_getGates()[i];
			if (g.r_x - 50 < cX && g.r_x + 50 > cX && g.r_y - 50 < cY && g.r_y + 50 > cY) {
				if (selectedId == g.id) {
					selectedType = -1;
					return;
				}
				hookedX = g.r_x;
				hookedY = g.r_y;
				targetId = g.id;
				return;
			}
		}
		//
		return;
	}
	//
	if (selectedId > -1 && selectedType > -1 && targetType == -1 && targetId > -1) {
		//
		for (int i = 0; i < 3; i++) {
			if (hookedX < Mposx && hookedX + 100 > Mposx && hookedY + (i * 20) < Mposy && hookedY + 19 + (i * 20) > Mposy) {
				// output
				targetType = i + 1;
				//
				g_pair(selectedId, targetId, selectedType, targetType);
				//
				selectedId = -1;
				targetId = -1;
				selectedType = -1;
				targetType = -1;
				return;
			}
		}
		//
		targetId = -1;
		selectedId = -1;
		selectedType = -1;
	}
	//
	char found = 0;
	for (int i = 0; i < g_getGatesSize(); i++) {
		gate_t* g = &g_getGates()[i];
		if (g->r_x - 50 < cX && g->r_x + 50 > cX && g->r_y - 50 < cY && g->r_y + 50 > cY) {
			selectedId = -1;
			clickedId = g->id;
			moveState = 1;
			found = 1;
			//
			moveXoffset = -(cX - g->r_x);
			moveYoffset = -(cY - g->r_y);
		
			if (g->type == 11) {
				if (g->r_x - 30 - 10 < cX && g->r_x - 30 + 10 > cX && g->r_y + 10 < cY && g->r_y + 30 > cY) {
					g->inA = g->inA > 0 ? 0 : 1;
					if (g->inAid > -1) g_update(g->inAid, -1);
				}
				if (g->r_x - 10 < cX && g->r_x + 10 > cX && g->r_y + 10 < cY && g->r_y + 30 > cY) {
					g->inB = g->inB > 0 ? 0 : 1;
					if (g->inBid > -1) g_update(g->inBid, -1);
				}
				if (g->r_x + 30 - 10 < cX && g->r_x + 30 + 10 > cX && g->r_y + 10 < cY && g->r_y + 30 > cY) {
					g->inC = g->inC > 0 ? 0 : 1;
					if (g->inCid > -1) g_update(g->inCid, -1);
				}
			}
		}
	}
	
	if (!found) {
		clickedId = -1;
		selectedId = -1;
	}
	//

}

void cpl_mouse_click(int key, int state, int cX, int cY) {
	if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS) {
		//
		g_clickedIdM(cX, cY);
		g_updateSelected();
		//
		
	}
	if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS) {
		g_clickedId(cX, cY);
	}
	if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE) {
		clickedId = -1;
		moveState = 0;
	}
}