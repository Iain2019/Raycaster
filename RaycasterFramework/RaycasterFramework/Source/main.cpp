#include <stdafx.h>
#include "rc_InputManager.h"
#include "rc_GameStateManager.h"
#include "Renderer.h"
#include "Console.h"
#include "rc_pcxLoader.h"
#include "rc_ImageLoader.h"

#include "rc_SplashState.h"

int main(int argc, char* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	
	//Set up the console for the application
	Console console;
	console.Initialise("Raycaster Console");

	GameStateManager* gsManager = new GameStateManager();

	//set up the splash state (first state that loads)
	SplashState* splashState = new SplashState();
	splashState->SetStateName("SplashState");
	splashState->SetManager(gsManager);

	gsManager->PushState(splashState);

	//Create a render context window - or bitmap rendeer window
	u32 windowWidth = 640;
	u32 WindowHeight = 480;

	//creates instance of the input manager to allow for movement
	InputManager::CreateInstance();

	//Start up main window
	Renderer mainWindow;
	if (!mainWindow.Initialise(console.GetConsoleWindowHandle(), windowWidth, WindowHeight))
	{
		srand((u32)time(nullptr));
		MSG msg = { 0 };

		//Create a timer, with current time and previous time to get delta time between frames
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto previousTime = currentTime;

		//delta time variable
		std::chrono::duration<double> elapsedTime;

		u32 frame = 0;

		//Main game loop
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//calculate time between frames
				previousTime = currentTime;
				currentTime = std::chrono::high_resolution_clock::now();
				elapsedTime = currentTime - previousTime;
				double fElapsedTime = elapsedTime.count();

				//Update the gamestate manager, updates whichever state is active
				gsManager->Update((float)(fElapsedTime));
				
				//clears items drawn in previous frame
				mainWindow.ClearRenderBuffer();

				//draws frame
				gsManager->Draw();
				mainWindow.Draw();

				if (frame % 30 == 0)
				{
					//buffer to hold FPS value
					wchar_t fpsBuffer[8];
					unsigned int fps = (unsigned int)(1.0f / fElapsedTime);
					//swprintf_s -- adds a null terminator at n-1, saves us doing it manually
					//Due to this, buffer to be an additional wchar_t longer
					//Using a ternary to limit fps display to 999 if over 1000 as not enough characters in buffer
					//to display more than this, and 999 is plenty fast
					swprintf_s(fpsBuffer, 8, L"FPS=%3u", fps > 999 ? 999 : fps);
					console.WriteToLocation(fpsBuffer, 8, 0, 0);
				}
				++frame;
			}
		}
	}
	//delte gamestate and input managers
	delete gsManager;
	InputManager::DestroyInstance();
	return 0;
}