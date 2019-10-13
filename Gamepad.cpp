#include "stdafx.h"
#include "Gamepad.h"
 
// Link the 'Xinput' library - Important!
#pragma comment(lib, "Xinput.lib")
 
Gamepad::Gamepad(int a_iIndex)
{
	m_State = GetState();
    m_iGamepadIndex = a_iIndex - 1;
}

void Gamepad::Update()
{
	m_State = GetState();
}

bool Gamepad::LStick_InDeadzone()
{
	short sX = m_State.Gamepad.sThumbLX;
	short sY = m_State.Gamepad.sThumbLY;
	
	// X axis is outside of deadzone
	if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
	    sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	    return false;
	
	// Y axis is outside of deadzone
	if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
	    sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	    return false;
	
	// One (or both axes) axis is inside of deadzone
	return true;
}

float Gamepad::LeftStick_X()
{
        // Obtain X axis of left stick
        short sX = m_State.Gamepad.sThumbLX;
 
        // Return axis value, converted to a float
        return (static_cast<float>(sX) / 32768.0f);
}
 
float Gamepad::LeftStick_Y()
{
	short sY = m_State.Gamepad.sThumbLY;
	
	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

float Gamepad::LeftTrigger()
{
	// Obtain value of left trigger
	BYTE Trigger = m_State.Gamepad.bLeftTrigger;
	
	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	      return Trigger / 255.0f;
	
	return 0.0f; // Trigger was not pressed
}

XINPUT_STATE Gamepad::GetState()
{
	// Temporary XINPUT_STATE to return
	XINPUT_STATE GamepadState;
	
	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));
	
	XInputGetState(m_iGamepadIndex, &GamepadState);
	
	return GamepadState;
}

int Gamepad::GetIndex()
{
	return m_iGamepadIndex;
}

bool Gamepad::Connected()
{
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));
	
	DWORD Result = XInputGetState(m_iGamepadIndex, &m_State);
	
	if (Result == ERROR_SUCCESS)
	      return true;
	else
	      return false;
}