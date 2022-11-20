#include <stdafx.h>
#include "rc_InputManager.h"
#include "Renderer.h"

InputManager* InputManager::m_instance = nullptr;

//Allow for instance to be created
InputManager* InputManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new InputManager();
	}
	return m_instance;
}

//Destroy the created instance
void InputManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}



InputManager::InputManager()
{
	memset(m_keyCurrentState, 0, 256);
}

InputManager::~InputManager()
{

}

void InputManager::SetKey(u8 a_key, bool a_val)
{
	m_keyCurrentState[a_key] = a_val;
}

bool InputManager::GetKeyDown(u8 a_key)
{
	return m_keyCurrentState[a_key];
}

LRESULT CALLBACK InputManager::HandleWindowCallbacks(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);
	PAINTSTRUCT paintStruct;
	HDC hDC;
	Renderer* renderer = Renderer::GetInstance();
	if (hwnd == renderer->GetWindowHandle())
	{
		switch (message)
		{
		case WM_PAINT:
		{
			hDC = BeginPaint(hwnd, &paintStruct);
			RECT clRect;
			GetClientRect(hwnd, &clRect);
			BitBlt(hDC, clRect.left, clRect.top, (clRect.right - clRect.left) + 1, (clRect.bottom - clRect.top) + 1, renderer->GetBufferContext(), 0, 0, SRCCOPY);
			EndPaint(hwnd, &paintStruct);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			InputManager* im = InputManager::GetInstance();
			im->SetKey((u8)wparam, true);
			break;
		}
		case WM_KEYUP:
		{
			InputManager* im = InputManager::GetInstance();
			im->SetKey((u8)wparam, false);
			break;
		}
		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
		};
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}
