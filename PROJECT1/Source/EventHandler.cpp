#include "EventHandler.h"
#include <stdio.h>
#include <string.h>

bool EventHandler::events[];
bool EventHandler::prev_events[];
bool EventHandler::MouseState[5] = { false, false, false, false, false };
bool EventHandler::PrevMouseState[5] = { false, false, false, false, false };
int EventHandler::MouseX = 0;
int EventHandler::MouseY = 0;

EventHandler::EventHandler() {
}

EventHandler::~EventHandler() {
}

bool EventHandler::Update() {
	bool success = true;

	SDL_Event currEvents;

	SDL_GetMouseState(&MouseX, &MouseY);

	for (int i = 0; i < 5; ++i) PrevMouseState[i] = MouseState[i];
	for (int i = 0; i < GameEvents::NUM_GAME_EVENTS; ++i) prev_events[i] = events[i];

	while (SDL_PollEvent(&currEvents)) {
		switch (currEvents.key.keysym.sym) {
		case SDLK_0: {
			SetButton(GameEvents::K0_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_1: {
			SetButton(GameEvents::K1_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_2: {
			SetButton(GameEvents::K2_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_3: {
			SetButton(GameEvents::K3_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_4: {
			SetButton(GameEvents::K4_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_5: {
			SetButton(GameEvents::K5_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_6: {
			SetButton(GameEvents::K6_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_7: {
			SetButton(GameEvents::K7_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_8: {
			SetButton(GameEvents::K8_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_9: {
			SetButton(GameEvents::K9_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_a: {
			SetButton(GameEvents::A_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_b: {
			SetButton(GameEvents::B_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_c: {
			SetButton(GameEvents::C_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_d: {
			SetButton(GameEvents::D_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_e: {
			SetButton(GameEvents::E_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_f: {
			SetButton(GameEvents::F_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_g: {
			SetButton(GameEvents::G_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_h: {
			SetButton(GameEvents::H_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_i: {
			SetButton(GameEvents::I_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_j: {
			SetButton(GameEvents::J_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_k: {
			SetButton(GameEvents::K_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_l: {
			SetButton(GameEvents::L_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_m: {
			SetButton(GameEvents::M_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_n: {
			SetButton(GameEvents::N_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_o: {
			SetButton(GameEvents::O_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_p: {
			SetButton(GameEvents::P_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_q: {
			SetButton(GameEvents::Q_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_r: {
			SetButton(GameEvents::R_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_s: {
			SetButton(GameEvents::S_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_t: {
			SetButton(GameEvents::T_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_u: {
			SetButton(GameEvents::U_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_v: {
			SetButton(GameEvents::V_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_w: {
			SetButton(GameEvents::W_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_x: {
			SetButton(GameEvents::X_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_y: {
			SetButton(GameEvents::Y_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_z: {
			SetButton(GameEvents::Z_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_UP:
		{
			SetButton(GameEvents::UP_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_DOWN:
		{
			SetButton(GameEvents::DOWN_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_LEFT:
		{
			SetButton(GameEvents::LEFT_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_RIGHT:
		{
			SetButton(GameEvents::RIGHT_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_0:
		{
			SetButton(GameEvents::KP0_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_1:
		{
			SetButton(GameEvents::KP1_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_2:
		{
			SetButton(GameEvents::KP2_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_3:
		{
			SetButton(GameEvents::KP3_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_4:
		{
			SetButton(GameEvents::KP4_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_5:
		{
			SetButton(GameEvents::KP5_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_6:
		{
			SetButton(GameEvents::KP6_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_7:
		{
			SetButton(GameEvents::KP7_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_8:
		{
			SetButton(GameEvents::KP8_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_KP_9:
		{
			SetButton(GameEvents::KP9_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_SPACE:
		{
			SetButton(GameEvents::SPACE_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_BACKSPACE:
		{
			SetButton(GameEvents::BACKSPACE_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		case SDLK_RETURN:
		{
			SetButton(GameEvents::ENTER_PRESSED, currEvents.key.type == SDL_KEYDOWN);
			break;
		}
		}
		//
		switch (currEvents.type) {
			case SDL_MOUSEBUTTONDOWN:
				MouseState[currEvents.button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				MouseState[currEvents.button.button] = false;
				break;
		}
	}
	return success;
}

bool EventHandler::KeyHit(short btn){
	return events[btn] && !prev_events[btn];
}

bool EventHandler::MDown(short btn) {
	return MouseState[btn];
}

bool EventHandler::MUp(short btn) {
	return !MouseState[btn];
}

bool EventHandler::MClicked(short btn) {
	return MouseState[btn] && !PrevMouseState[btn];
}

bool EventHandler::MReleased(short btn) {
	return !MouseState[btn] && PrevMouseState[btn];
}

void EventHandler::SetButton(GameEvents eventNum, bool pressed) {
	events[eventNum] = pressed;
}
