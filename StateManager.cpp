#include "StateManager.h"
#include "GameState.h"

CStateManager::CStateManager() : m_pActiveState(NULL) {}

CStateManager::~CStateManager() {}

void CStateManager::keyboard_callback(unsigned char key, int x, int y) {
	if (m_pActiveState)
		m_pActiveState->keyboard_callback(key, x, y);
}

void CStateManager::keyboard_up_callback(unsigned char key, int x, int y) {
	if (m_pActiveState)
		m_pActiveState->keyboard_up_callback(key, x, y);
}

void CStateManager::mouse_callback(int button, int state, int x, int y) {
	if (m_pActiveState)
		m_pActiveState->mouse_callback(button, state, x, y);
}

void CStateManager::idle(int value) {
	if (m_pActiveState)
		m_pActiveState->idle(value);
}

void CStateManager::display_callback(void) {
	if (m_pActiveState)
		m_pActiveState->display_callback();
}

void CStateManager::mouse_motion_callback(int x, int y) {
	if (m_pActiveState)
		m_pActiveState->mouse_motion_callback(x, y);
}