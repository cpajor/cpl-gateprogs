#include "pajordef.h"
#include "gateprogs.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

// l_user.c
extern int selectedId;
extern int targetId;
extern int selectedType;
extern char selectedNot;
//
extern int moveXoffset;
extern int moveYoffset;

// l_gates.c
extern int selectedX;
extern int selectedY;
extern int targetX;
extern int targetY;
extern int hookedX;
extern int hookedY;

// w_progs.c
extern int Mposx;
extern int Mposy;
extern int Wwidth;

int hoverID = 0;
int hoverMenuID = 0;
int hoverLast = 0;

//
char r_showAllRays = 1;

char hover1Left = 0;

void r_updateMouse() {
	hoverID = 0;
	hoverMenuID = 0;
	//
	hover1Left = 0;

	for (int i = 0; i < 4; i++) {
		if (selectedX < Mposx && selectedX + 120 > Mposx && selectedY + (i * 20) < Mposy && selectedY + 19 + (i * 20) > Mposy && hoverMenuID == 0) {
			hoverID = i + 1;
		}
		//
		if (i > 1) continue;
		if (selectedX + 120 < Mposx && selectedX + 220 > Mposx && selectedY + (i * 20) < Mposy && selectedY + 59 + (i * 20) > Mposy && hoverLast == (i + 1)) {
			hoverMenuID = i + 1;
			hoverID = 0;
			continue;
		}
		if (selectedX + 100 < Mposx && selectedX + 120 > Mposx && selectedY + (i * 20) < Mposy && selectedY + 19 + (i * 20) > Mposy && hoverID == i + 1) {
			//hoverMenuID = i + 1;
			hoverLast = i + 1;
		}
		
	} 

	if (Wwidth - 100 < Mposx && Mposx < Wwidth && Mposy < 100 && Mposy > 0) {
		hover1Left = 1;
	}

	/*
	
	int hovid = 0;
	int hovmid = 0;

	for (int i = 0; i < 4; i++) {
		if (selectedX < Mposx && selectedX + 120 > Mposx && selectedY + (i * 20) < Mposy && selectedY + 19 + (i * 20) > Mposy && hoverMenuID == 0) {
			//hoverID = i + 1;
			hovid = i + 1;
		}
		if (i > 1) continue;
		if (selectedX + 120 < Mposx && selectedX + 220 > Mposx && selectedY + (i * 20) < Mposy && selectedY + 59 + (i * 20) > Mposy) {
			//hoverMenuID = i + 1;
			//hoverID = 0;
			hovmid = i + 1;
			continue;
		}
		if (selectedX + 100 < Mposx && selectedX + 120 > Mposx && selectedY + (i * 20) < Mposy && selectedY + 19 + (i * 20) > Mposy && hoverID == i + 1) {
			//hoverMenuID = i + 1;
			//hoverID = 0;
			hovmid = i + 1;
		}
		
	}

	if (hovid ) {
		hoverID = hovid;
	}
	else {
		hoverID = 0;
	}
	*/
	
	//
	if (targetX > -1 && targetY > -1) {
		if (targetX < Mposx && targetX + 120 > Mposx && targetY < Mposy && targetY + 20 > Mposy) {
			hoverID = 1;
		}
		if (targetX < Mposx && targetX + 120 > Mposx && targetY + 21 < Mposy && targetY + 40 > Mposy) {
			hoverID = 2;
		}
		//if (!targetNot)
		///if (targetX < Mposx && targetX + 120 > Mposx && targetY + 41 < Mposy && targetY + 60 > Mposy) {
		//	hoverID = 3;
		//}
	}
}

// TODO fixed position
void r_drawRay(gateray_t g) {
	if (g.dead) return;
	int x1 = g.r_x1;
	int x2 = g.r_x2;
	int y1 = g.r_y1 + 35;
	int y2 = g.r_y2 + 35;
	if (g.type1 == 1) {
		x1 -= 35;
	}
	if (g.type1 == 3) {
		x1 += 35;
	}
	if (g.type2 == 1) {
		x2 -= 35;
	}
	if (g.type2 == 3) {
		x2 += 35;
	}
	glColor3f(0, 0, 0);
	glLineWidth(4.F);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	glLineWidth(1.75F);
	glBegin(GL_LINES);
	glColor3f(0.6F, 0.6F, 0.6F); glVertex2i(x1, y1);
	glColor3f(0.45F, 0.45F, 0.45F); glVertex2i(x2, y2);
	glEnd();
	glLineWidth(1.F);
}

/*
	types:
		0 - not
		1 - or
		2 - nor
		3 - and
		4 - nand
		5 - xor
		6 - xnor
*/

void r_drawGate(gate_t gate) {
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(gate.r_x - 50, gate.r_y - 50);
	glVertex2f(gate.r_x - 50, gate.r_y + 50);
	glVertex2f(gate.r_x + 50, gate.r_y + 50);
	glVertex2f(gate.r_x + 50, gate.r_y - 50);
	//
	glColor3f(0.25F, 0.25F, 0.25F);
	glVertex2f(gate.r_x - 50 + 1, gate.r_y - 50 + 1);
	glVertex2f(gate.r_x - 50 + 1, gate.r_y + 50 - 1);
	glVertex2f(gate.r_x + 50 - 1, gate.r_y + 50 - 1);
	glVertex2f(gate.r_x + 50 - 1, gate.r_y - 50 + 1);
	glEnd();
	//
	if (gate.type == 0) {
		cpl_drawConStringN("NOT", gate.r_x - 24, gate.r_y - 32, 3);
	}
	if (gate.type == 1) {
		cpl_drawConStringN("OR", gate.r_x - 16, gate.r_y - 32, 2);
	}
	if (gate.type == 2) {
		cpl_drawConStringN("NOR", gate.r_x - 24, gate.r_y - 32, 3);
	}
	if (gate.type == 3) {
		cpl_drawConStringN("AND", gate.r_x - 24, gate.r_y - 32, 3);
	}
	if (gate.type == 4) {
		cpl_drawConStringN("NAND", gate.r_x - 32, gate.r_y - 32, 4);
	}
	if (gate.type == 5) {
		cpl_drawConStringN("XOR", gate.r_x - 24, gate.r_y - 32, 3);
	}
	if (gate.type == 6) {
		cpl_drawConStringN("XNOR", gate.r_x - 32, gate.r_y - 32, 4);
	}
	if (gate.type == 7) {
		cpl_drawConStringN("1->2", gate.r_x - 32, gate.r_y - 32, 4);
	}
	if (gate.type == 11) {
		cpl_drawConStringN("IN", gate.r_x - 16, gate.r_y - 32, 2);
	}
	if (gate.type == 12) {
		cpl_drawConStringN("OUT", gate.r_x - 24, gate.r_y - 32, 3);
	}
	//
	/*
	if (gate.type == 0) {
		glBegin(GL_QUADS);
		//
		if (gate.inA == 0) {
			glColor3f(0, 0, 0);
		}
		if (gate.inA == 1) {
			glColor3f(0, 1, 0);
		}
		if (gate.inA == -1) {
			glColor3f(1, 0, 0);
		}
		glVertex2f(gate.r_x - 20 + 10, gate.r_y + 20 + 10);
		glVertex2f(gate.r_x - 20 + 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x - 20 - 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x - 20 - 10, gate.r_y + 20 + 10);
		//
		if (gate.inC == 0) {
			glColor3f(0, 0, 0);
		}
		if (gate.inC == 1) {
			glColor3f(0, 1, 0);
		}
		if (gate.inC == -1) {
			glColor3f(1, 0, 0);
		}
		glVertex2f(gate.r_x + 20 + 10, gate.r_y + 20 + 10);
		glVertex2f(gate.r_x + 20 + 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x + 20 - 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x + 20 - 10, gate.r_y + 20 + 10);
		glEnd();
		//
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0, 0, 0);
		glBegin(GL_QUADS);
		glVertex2f(gate.r_x - 20 + 10, gate.r_y + 20 + 10);
		glVertex2f(gate.r_x - 20 + 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x - 20 - 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x - 20 - 10, gate.r_y + 20 + 10);
		//
		glVertex2f(gate.r_x + 20 + 10, gate.r_y + 20 + 10);
		glVertex2f(gate.r_x + 20 + 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x + 20 - 10, gate.r_y + 20 - 10);
		glVertex2f(gate.r_x + 20 - 10, gate.r_y + 20 + 10);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//
		cpl_drawConChar('I', gate.r_x - 20 - 9, gate.r_y + 20 - 8);
		cpl_drawConChar('O', gate.r_x + 20 - 8, gate.r_y + 20 - 8);
		return;
	}
	*/
	if (gate.type < 10) {
		if (gate.outInId == 1) {
			glPushMatrix();
			glTranslatef(gate.r_x - 39, gate.r_y + 10 , 0);
			glScalef(1, -1, 1);
			glTranslatef(-(gate.r_x - 39), -(gate.r_y + 10), 0);
			cpl_drawConChar('^', gate.r_x - 39, gate.r_y + 10);
			glPopMatrix();
		}

		if (gate.outInId == 2) {
			glPushMatrix();
			glTranslatef(gate.r_x - 9, gate.r_y + 10, 0);
			glScalef(1, -1, 1);
			glTranslatef(-(gate.r_x - 9), -(gate.r_y + 10), 0);
			cpl_drawConChar('^', gate.r_x - 9, gate.r_y + 10);
			glPopMatrix();
		}

		if (gate.outInId == 3) {
			glPushMatrix();
			glTranslatef(gate.r_x + 21, gate.r_y + 10, 0);
			glScalef(1, -1, 1);
			glTranslatef(-(gate.r_x + 21), -(gate.r_y + 10), 0);
			cpl_drawConChar('^', gate.r_x + 21, gate.r_y + 10);
			glPopMatrix();
		}	
	}

	glBegin(GL_QUADS);
	//
	if (gate.inA == 0) {
		glColor3f(1, 0, 0);
	}
	if (gate.inA == 1) {
		glColor3f(0, 1, 0);
	}
	if (gate.inA == -1) {
		glColor3f(0, 0, 0);
	}
	
	glVertex2f(gate.r_x - 30 + 10, gate.r_y + 20 + 10);
	glVertex2f(gate.r_x - 30 + 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 30 - 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 30 - 10, gate.r_y + 20 + 10);
	//
	if (gate.inB == 0) {
		glColor3f(1, 0, 0);
	}
	if (gate.inB == 1) {
		glColor3f(0, 1, 0);
	}
	if (gate.inB == -1) {
		glColor3f(0, 0, 0);
	}
	glVertex2f(gate.r_x + 10, gate.r_y + 20 + 10);
	glVertex2f(gate.r_x + 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 10, gate.r_y + 20 + 10);
	//
	if (gate.inC == 0) {
		glColor3f(1, 0, 0);
	}
	if (gate.inC == 1) {
		glColor3f(0, 1, 0);
	}
	if (gate.inC == -1) {
		glColor3f(0, 0, 0);
	}
	glVertex2f(gate.r_x + 30 + 10, gate.r_y + 20 + 10);
	glVertex2f(gate.r_x + 30 + 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x + 30 - 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x + 30 - 10, gate.r_y + 20 + 10);
	glEnd();
	//
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 0, 0); 
	glBegin(GL_QUADS);
	glVertex2f(gate.r_x + 10, gate.r_y + 20 + 10);
	glVertex2f(gate.r_x + 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 10, gate.r_y + 20 + 10);
	//
	glVertex2f(gate.r_x + 30 + 10, gate.r_y + 20 + 10);
	glVertex2f(gate.r_x + 30 + 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x + 30 - 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x + 30 - 10, gate.r_y + 20 + 10);
	//
	glVertex2f(gate.r_x - 30 + 10, gate.r_y + 20 + 10);
	glVertex2f(gate.r_x - 30 + 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 30 - 10, gate.r_y + 20 - 10);
	glVertex2f(gate.r_x - 30 - 10, gate.r_y + 20 + 10); 
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//
	cpl_drawConChar('A', gate.r_x - 30	- 9, gate.r_y + 20 - 8);
	cpl_drawConChar('B', gate.r_x		- 8, gate.r_y + 20 - 8);
	cpl_drawConChar('C', gate.r_x + 30	- 8, gate.r_y + 20 - 8);
}

void r_drawGateToolbox(int rx, int ry, int type) {
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(rx - 50, ry - 50);
	glVertex2f(rx - 50, ry + 50);
	glVertex2f(rx + 50, ry + 50);
	glVertex2f(rx + 50, ry - 50);
	//
	glColor3f(0.25F, 0.25F, 0.25F);
	glVertex2f(rx - 50 + 1, ry - 50 + 1);
	glVertex2f(rx - 50 + 1, ry + 50 - 1);
	glVertex2f(rx + 50 - 1, ry + 50 - 1);
	glVertex2f(rx + 50 - 1, ry - 50 + 1);
	glEnd();
	//
	if (type == 0) {
		cpl_drawConStringN("NOT", rx - 24, ry - 32, 3);
	}
	if (type == 1) {
		cpl_drawConStringN("OR", rx - 16, ry - 32, 2);
	}
	if (type == 2) {
		cpl_drawConStringN("NOR", rx - 24, ry - 32, 3);
	}
	if (type == 3) {
		cpl_drawConStringN("AND", rx - 24, ry - 32, 3);
	}
	if (type == 4) {
		cpl_drawConStringN("NAND", rx - 32, ry - 32, 4);
	}
	if (type == 5) {
		cpl_drawConStringN("XOR", rx - 24, ry - 32, 3);
	}
	if (type == 6) {
		cpl_drawConStringN("XNOR", rx - 32, ry - 32, 4);
	}
	if (type == 7) {
		cpl_drawConStringN("1->2", rx - 32, ry - 32, 4);
	}
	if (type == 11) {
		cpl_drawConStringN("IN", rx - 16, ry - 32, 2);
	}
	if (type == 12) {
		cpl_drawConStringN("OUT", rx - 24, ry - 32, 3);
	}
}

void r_drawToolbox() {
	glBegin(GL_QUADS);
	glColor3f(0.65F, 0.65F, 0.65F);
	glVertex2f(0		, 0);
	glVertex2f(Wwidth	, 0);
	glVertex2f(Wwidth	, 100);
	glVertex2f(0		, 100);
	glEnd();
	//
	for (int i = 0; i < 8; i++) {
		r_drawGateToolbox(50 + (i * 100), 50, i);
	}
	r_drawGateToolbox(850, 50, 11);
	r_drawGateToolbox(950, 50, 12);

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(Wwidth - 100, 0);
	glVertex2f(Wwidth - 100, 100);
	glVertex2f(Wwidth, 100);
	glVertex2f(Wwidth, 0);
	//
	glColor3f(0.25F, 0.25F, 0.25F);
	glVertex2f(Wwidth - 96, 4);
	glVertex2f(Wwidth - 96, 96);
	glVertex2f(Wwidth - 4, 96);
	glVertex2f(Wwidth - 4, 4);
	glEnd();
	//
	glPushMatrix();
	glTranslatef(Wwidth - 50, 50, 0);
	glScalef(1.5F, 1.5F, 1);
	glTranslatef(-(Wwidth - 50), -50, 0);

	char flag = r_showAllRays;

	if (hover1Left) flag = !flag;

	if (flag) {
 		cpl_drawConStringN("<0>", Wwidth - 74, 42, 3);
	}
	else {
		cpl_drawConStringN("<->", Wwidth - 74, 42, 3);
	}

	glPopMatrix();
}

extern tex_t tex_conChars;

void cpl_display() {
	for (int i = 0; i < g_getGatesSize(); i++) {
		r_drawGate(g_getGates()[i]);
	}
	//
	char flag = r_showAllRays;

	if (hover1Left) flag = !flag;

	if (flag)
	for (int i = 0; i < g_getGateraysSize(); i++) {
		r_drawRay(g_getGaterays()[i]);
	}
	//
	r_drawToolbox();
	//
	if (selectedId > -1 && selectedType > -1) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glColor4f(1, 1, 1, 0.1F);
		glVertex2f(selectedX - 50, selectedY - 50);
		glVertex2f(selectedX - 50, selectedY + 50);
		glVertex2f(selectedX + 50, selectedY + 50);
		glVertex2f(selectedX + 50, selectedY - 50);
		glEnd();
		glDisable(GL_BLEND);
	}
	/*
	if (selectedId > -1 && targetId > -1 && selectedType > -1) {
		glBegin(GL_QUADS);
		glColor3f(0.65F, 0.65F, 0.65F);
		glVertex2f(hookedX, hookedY);
		glVertex2f(hookedX, hookedY + 40);
		glVertex2f(hookedX + 120, hookedY + 40);
		glVertex2f(hookedX + 120, hookedY);
		glEnd();
		//
		if (hoverID) {
			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(hookedX, hookedY + (hoverID * 20) - 20);
			glVertex2f(hookedX, hookedY + (hoverID * 20));
			glVertex2f(hookedX + 120, hookedY + (hoverID * 20));
			glVertex2f(hookedX + 120, hookedY + (hoverID * 20) - 20);
			glEnd();
		}
		//
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		glVertex2f(hookedX, hookedY);
		glVertex2f(hookedX, hookedY + 20);
		glVertex2f(hookedX + 120, hookedY + 20);
		glVertex2f(hookedX + 120, hookedY);
		glVertex2f(hookedX, hookedY);
		glVertex2f(hookedX, hookedY + 40);
		glVertex2f(hookedX + 120, hookedY + 40);
		glVertex2f(hookedX + 120, hookedY);
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (!selectedType) {
			cpl_drawConStringN("Input A", hookedX, hookedY, 7);
			cpl_drawConStringN("Input B", hookedX, hookedY + 20, 7);
		}
		
	}
	*/

	//
	if (selectedId > -1 && selectedType == -1 && targetId == -1) {
		glBegin(GL_QUADS);
		glColor3f(0.65F, 0.65F, 0.65F);
		glVertex2f(selectedX, selectedY);
		glVertex2f(selectedX, selectedY + 80);
		glVertex2f(selectedX + 120, selectedY + 80);
		glVertex2f(selectedX + 120, selectedY);
		glEnd();
		//
		/*
		if (hoverID) {
			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(selectedX, selectedY + (hoverID * 20) - 20);
			glVertex2f(selectedX, selectedY + (hoverID * 20));
			glVertex2f(selectedX + 120, selectedY + (hoverID * 20));
			glVertex2f(selectedX + 120, selectedY + (hoverID * 20) - 20);
			glEnd();
		}
		*/
		//
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		glVertex2f(selectedX, selectedY);
		glVertex2f(selectedX, selectedY + 20);
		glVertex2f(selectedX + 120, selectedY + 20);
		glVertex2f(selectedX + 120, selectedY);
		glVertex2f(selectedX, selectedY);
		glVertex2f(selectedX, selectedY + 40);
		glVertex2f(selectedX + 120, selectedY + 40);
		glVertex2f(selectedX + 120, selectedY);
		glVertex2f(selectedX, selectedY);
		glVertex2f(selectedX, selectedY + 60);
		glVertex2f(selectedX + 120, selectedY + 60);
		glVertex2f(selectedX + 120, selectedY);
		glVertex2f(selectedX, selectedY);
		glVertex2f(selectedX, selectedY + 80);
		glVertex2f(selectedX + 120, selectedY + 80);
		glVertex2f(selectedX + 120, selectedY);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//

		if (hoverMenuID || (hoverID > 0 && hoverID < 3)) {
			int hov = hoverMenuID > 0 ? hoverMenuID : hoverID;
			glBegin(GL_QUADS);
			glColor3f(0.65F, 0.65F, 0.65F);
			glVertex2f(selectedX + 120, selectedY + (20 * (hov - 1))			);
			glVertex2f(selectedX + 120, selectedY + (20 * (hov - 1)) + 60		);
			glVertex2f(selectedX + 220, selectedY + (20 * (hov - 1)) + 60);
			glVertex2f(selectedX + 220, selectedY + (20 * (hov - 1))	);
			glEnd();
			/*
			if (hoverSubID > 0) {
				int sb = hoverSubID + hoverMenuID;
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex2f(selectedX + 120, selectedY + (sb * 20) + 20);
				glVertex2f(selectedX + 120, selectedY + (sb * 20));
				glVertex2f(selectedX + 220, selectedY + (sb * 20));
				glVertex2f(selectedX + 220, selectedY + (sb * 20) + 20);
				glEnd();
			}
			*/
			glColor3f(0, 0, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_QUADS);
			for (int i = 0; i < 3; i++) {
				int sb = i + hov;
				glVertex2f(selectedX + 120, selectedY + (sb * 20) - 20);
				glVertex2f(selectedX + 120, selectedY + (sb * 20));
				glVertex2f(selectedX + 220, selectedY + (sb * 20)); 
				glVertex2f(selectedX + 220, selectedY + (sb * 20) - 20);
			}
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
			cpl_drawConStringN("PIN A", selectedX + 128, selectedY + (20 * (hov - 1)), 5);
			cpl_drawConStringN("PIN B", selectedX + 128, selectedY + (20 * (hov - 1)) + 20, 5);
			cpl_drawConStringN("PIN C", selectedX + 128, selectedY + (20 * (hov - 1)) + 40, 5);

			//
		}

		
		//
		cpl_drawConStringN("Attach", selectedX + 5, selectedY, 7);
		cpl_drawConStringN("Output", selectedX + 5, selectedY + 20, 6);
		cpl_drawConStringN("Option", selectedX + 4, selectedY + 40, 6);
		cpl_drawConStringN("Delete", selectedX + 4, selectedY + 60, 6);
		cpl_drawConChar('>', selectedX + 104, selectedY);
		cpl_drawConChar('>', selectedX + 104, selectedY + 20);

	}
	//
	if (targetId > -1 && hookedX > 0 && hookedY > 0) {
		glBegin(GL_QUADS);
		glColor3f(0.65F, 0.65F, 0.65F);
		glVertex2f(hookedX, hookedY);
		glVertex2f(hookedX, hookedY + 60);
		glVertex2f(hookedX + 100, hookedY + 60);
		glVertex2f(hookedX + 100, hookedY);
		glEnd();

		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		for (int i = 0; i < 3; i++) {
			glVertex2f(hookedX, hookedY + (i * 20) + 20);
			glVertex2f(hookedX, hookedY + (i * 20));
			glVertex2f(hookedX + 100, hookedY + (i * 20));
			glVertex2f(hookedX + 100, hookedY + (i * 20) + 20);
		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		cpl_drawConStringN("PIN A", hookedX + 8, hookedY, 5);
		cpl_drawConStringN("PIN B", hookedX + 8, hookedY + 20, 5);
		cpl_drawConStringN("PIN C", hookedX + 8, hookedY + 40, 5);

		//
	}
	//
}