#pragma once
#include "SDL.h"

enum GameEvents {
	KP0_PRESSED,
	KP1_PRESSED,
	KP2_PRESSED,
	KP3_PRESSED,
	KP4_PRESSED,
	KP5_PRESSED,
	KP6_PRESSED,
	KP7_PRESSED,
	KP8_PRESSED,
	KP9_PRESSED,

	K0_PRESSED,
	K1_PRESSED,
	K2_PRESSED,
	K3_PRESSED,
	K4_PRESSED,
	K5_PRESSED,
	K6_PRESSED,
	K7_PRESSED,
	K8_PRESSED,
	K9_PRESSED,
	A_PRESSED, //I needed more
	B_PRESSED, //buttons james
	C_PRESSED, //welcome to yanderedev code
	D_PRESSED, //if it works it works! -Kieran
	E_PRESSED,
	F_PRESSED,
	G_PRESSED,
	H_PRESSED,
	I_PRESSED,
	J_PRESSED,
	K_PRESSED,
	L_PRESSED,
	M_PRESSED,
	N_PRESSED,
	O_PRESSED,
	P_PRESSED,
	Q_PRESSED,
	R_PRESSED,
	S_PRESSED,
	T_PRESSED,
	U_PRESSED,
	V_PRESSED,
	W_PRESSED,
	X_PRESSED,
	Y_PRESSED,
	Z_PRESSED,

	UP_PRESSED,
	DOWN_PRESSED,
	LEFT_PRESSED,
	RIGHT_PRESSED,
	SPACE_PRESSED,
	BACKSPACE_PRESSED,
	ENTER_PRESSED,
	ESCAPE_PRESSED,

	NUM_GAME_EVENTS
};

class EventHandler {
public:
	EventHandler();
	~EventHandler();

	static bool Update();
	static bool MDown(short btn);
	static bool MUp(short btn);
	static bool MClicked(short btn);
	static bool MReleased(short btn);
	static bool KeyHit(short btn);

	static int MouseX;
	static int MouseY;

	static const int NUM_EVENTS = 54;

	static bool events[GameEvents::NUM_GAME_EVENTS];
	static bool prev_events[GameEvents::NUM_GAME_EVENTS];
private:

	static void SetButton(GameEvents eventNum, bool pressed);
	static bool MouseState[5];
	static bool PrevMouseState[5];
};


