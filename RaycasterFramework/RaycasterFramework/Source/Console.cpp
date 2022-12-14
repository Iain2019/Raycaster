#include <stdafx.h>
#include "Console.h"

#define CONSOLE_X_POS 20
#define CONSOLE_Y_POS 180
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40

Console::Console() : m_xPos(0), m_yPos(0), m_consoleWidth(0), m_consoleHeight(0),
m_consoleHandle(nullptr), m_consoleWindow(nullptr)
{

}

Console::~Console()
{

}

bool Console::Initialise(const char* a_windowTitle, const unsigned short a_xPos, const unsigned short a_yPos,
	const unsigned short a_width, const unsigned short a_height)
{
	//Initialise window dimentions, x, y, width and height can be passed through ao to use defualts
	//using a ternary operator to determine if default size needs to be used
	m_xPos = a_xPos ? a_xPos : CONSOLE_X_POS;
	m_yPos = a_xPos ? a_yPos : CONSOLE_Y_POS;
	m_consoleWidth = a_width ? a_width : CONSOLE_WIDTH;
	m_consoleHeight = a_height ? a_height : CONSOLE_HEIGHT;

	//Windows code to create console screen buffer
	m_consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(m_consoleHandle);
	COORD consoleSize = { (short)m_consoleWidth, (short)m_consoleHeight };
	SetConsoleScreenBufferSize(m_consoleHandle, consoleSize);
	//Set console window title
	SetConsoleTitle(a_windowTitle);

	m_consoleWindow = GetConsoleWindow();
	SetWindowPos(m_consoleWindow, 0, m_xPos, m_yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	return true;
}

unsigned int  Console::WriteToLocation(wchar_t* a_data, short a_numCharacters, short a_xPos, short a_yPos)
{
	DWORD dwBytesWritten = 0;
	//Multiply number of characters by size of wchar_t (2) to get number of bytes to write
	WriteConsoleOutputCharacter(m_consoleHandle, (LPCSTR)a_data, a_numCharacters * sizeof(wchar_t), { a_xPos, a_yPos }, &dwBytesWritten);
	//return byteds written to indicate success
	return dwBytesWritten;
}