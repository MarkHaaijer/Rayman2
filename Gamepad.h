#ifndef GAMEPAD_H
#define GAMEPAD_H
 
#include <Xinput.h>
 
class Gamepad
{
public:
    // Function prototypes
    //---------------------//
 
    // Constructors
    Gamepad(const Gamepad& other) = delete;
	Gamepad(Gamepad&& other) noexcept = delete;
	Gamepad& operator=(const Gamepad& other) = delete;
	Gamepad& operator=(Gamepad&& other) noexcept = delete;

    Gamepad(int a_iIndex);

	// Destructor
	virtual ~Gamepad() = default;

    void Update();
 
    // Thumbstick functions
    bool LStick_InDeadzone();
 
    float LeftStick_X();  
    float LeftStick_Y();  

    // Trigger functions
    float LeftTrigger();  
	
    // Utility functions
    XINPUT_STATE GetState(); 
    int GetIndex();          
    bool Connected();        
 
private:
    // Private variables
    //---------------------//
 
    XINPUT_STATE m_State;
    int m_iGamepadIndex; 
};
 
#endif