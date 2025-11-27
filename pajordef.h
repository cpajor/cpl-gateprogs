#ifndef PAJOR_H
#define PAJOR_H
#ifndef tex_t
#define tex_t unsigned int
#endif // !tex_t

void txBegin(tex_t tex);

void txEnd();

void cpl_drawConChar(char c, int x, int y);

void cpl_drawConStringN(char* c, int x, int y, int N);

void cpl_drawConNoColorChar(char c, int x, int y);

void cpl_drawConNoColorStringN(char* c, int x, int y, int N);

char* cpl_getDir(char* file);

#endif