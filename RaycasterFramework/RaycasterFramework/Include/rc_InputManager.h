#include <stdafx.h>
#pragma once

#ifndef __RC_INPUT_MANAGER_H__
#define  __RC_INPUT_MANAGER_H__

//This is the main input handler class, as such it will be a static instance that is accessible from anywhere in the application
class InputManager
{
public:
	//Our singleton accessor function call this function to retrieve an instance of the InputManager
	static InputManager* GetInstance() { return m_instance; }
	//Our singleton creation function, call this to create an instance of our input manager, and return it
	static InputManager* CreateInstance();
	//Destroy Instance this is what we call to delete the instance of our input manager
	static void DestroyInstance();
	//Lets have that function to handle window callback stuff in out input manager, its a static funtion
	static LRESULT CALLBACK HandleWindowCallbacks(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


	//set the value of a key in our key state array
	void SetKey(u8 a_key, bool a_val);
	//get the current state of a key in our key state array
	bool GetKeyDown(u8 a_key);

private:
	//this is the static instance of our input manager class
	static  InputManager* m_instance;
	//Constructor and destructor are private, this means that they can only be accessed from inside an instance of the class
	//so we cannot call InputManager* im = new InputManager() unless we are inside a function belonging to this class
	InputManager();
	~InputManager();

	//An array of 256 bools to keep track of keys that are currently pressed (or not)
	bool m_keyCurrentState[256];
};

#endif //! __RC_INPUT_MANAGER_H__