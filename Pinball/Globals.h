#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum category_Bits
{
	GROUND = 0x0001,
	LVL1 = 0x0002,
	LVL2 = 0x0004,
	BALL = 0x0008,
	BOX = 0x0016,
	START = 0x0032
};

// Configuration -----------
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 432
#define SCREEN_SIZE 1.5
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "2D Physics Playground"