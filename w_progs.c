#include "pajordef.h"
#include "gateprogs.h"
#include <windows.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#include <stdio.h>
#include <direct.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include "resource.h"

GLFWwindow* Wwindow;

int Wwidth;
int Wheight;

int Mposx;
int Mposy;

char* _dlg_dir;

// l_gates.c
extern void g_init0();
extern void g_updateEtc();

char* cpl_getDir(char* file) {
	if (!file) {
		return _dlg_dir;
	}
	char* _tmp = malloc(sizeof(char) * (strlen(file) + strlen(_dlg_dir) + 2));
	int j = 0;
	for (int i = 0; i < strlen(_dlg_dir); i++) {
		_tmp[j] = _dlg_dir[i];
		j++; 
	}
	_tmp[j] = '\\';
	j++;
	for (int i = 0; i < strlen(file); i++) {
		_tmp[j] = file[i];
		j++;
	}
	_tmp[j] = '\0';
	return _tmp;
}

// l_user.c
extern void cpl_mouse_click(int key, int state, int cX, int cY);

void mouseCLB(GLFWwindow* window, int button, int action, int mods) {
	cpl_mouse_click(button, action, Mposx, Mposy);
}

// r_window.c
extern void r_updateMouse();

void mousePosCLB(GLFWwindow* window, double x, double y) {
	Mposx = x;
	Mposy = y;
	r_updateMouse();
}

void cpl_vid_create(int width, int height) {
	Wwidth = width;
	Wheight = height;
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	Wwindow = glfwCreateWindow(width, height, "cp-L GATEprogs", 0 /*SCREEN*/, NULL); //glfwGetPrimaryMonitor()
	glfwMakeContextCurrent(Wwindow);
	glfwSwapInterval(0);
	glfwSetMouseButtonCallback(Wwindow, mouseCLB);
	glfwSetCursorPosCallback(Wwindow, mousePosCLB);
	glViewport(0, 0, width, height);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}

int start(int w, int h) {
	g_init0();
	cpl_vid_create(w, h);
	glViewport(0, 0, Wwidth, Wheight);
	//
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, Wwidth, Wheight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glDisable(GL_DEPTH_TEST);
;	while (!glfwWindowShouldClose(Wwindow)) {
		g_updateEtc();
		//
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		cpl_display();
		glfwSwapBuffers(Wwindow);
		glfwPollEvents();
	}
	glfwDestroyWindow(Wwindow);
	glfwTerminate();
	return 0;
}

INT_PTR CALLBACK PopCallBProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	if (message == WM_INITDIALOG) {
		SendDlgItemMessageA(hDlg, IDC_EDITW, WM_SETTEXT, 0, (LPARAM)"1366");
		SendDlgItemMessageA(hDlg, IDC_EDITH, WM_SETTEXT, 0, (LPARAM)"768");
		SendDlgItemMessageA(hDlg, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)_dlg_dir);
		return 1;
	}

	if (message == WM_COMMAND) {
		//if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1) {
			if (wParam == IDOK) {
				wchar_t lpszW[6];
				wchar_t lpszH[6];
				SendDlgItemMessageW(hDlg, IDC_EDITW, WM_GETTEXT, 6, (LPARAM)&lpszW);
				SendDlgItemMessageW(hDlg, IDC_EDITH, WM_GETTEXT, 6, (LPARAM)&lpszH);
				int w = _wtoi(lpszW);
				int h = _wtoi(lpszH);
				if (w < 1111) w = 1111;
				if (h < 600) h = 600;
				EndDialog(hDlg, TRUE);
				start(w, h);
				return TRUE;
			}
			if (wParam == IDCANCEL) {
				EndDialog(hDlg, TRUE);
				return TRUE;
			}
			return 0;
		//}
	}
	return 0;
}

int init(char* dir) {
	char* cwd = _getcwd(0, 0);
	_dlg_dir = (dir > 0 ? dir : cwd);
	//
	DialogBox(0, MAKEINTRESOURCE(IDD_DIALOG1), 0, PopCallBProc);
	//start(1366, 768);
}

int main(int argc, char** argv) {
	init(argc <= 1 ? 0 : argv[1]);
	return 0;
}

int WINAPI WinMain() {
	int argc;
	wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	char* arg;
	wcstombs(&arg, argv[1], lstrlenW(argv[1])); // wchar_t -> char
	init(argc <= 1 ? 0 : arg);
	return 0;
}