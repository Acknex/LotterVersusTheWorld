// made by Rackscha, adapted by Superku


#ifndef Console_h
	#define Console_h
	#include<windows.h>;

	long WINAPI WriteConsole(int Handle, char* Buffer, int CharsToWrite, int* CharsWritten, int reserved);
	long WINAPI CreateConsoleScreenBuffer(long dwDesiredAccess, long dwShareMode, long *lpSecurityAttributes, long dwFlags, long lpScreenBufferData);
	long WINAPI SetConsoleActiveScreenBuffer(long hConsoleOutput);
	long GConsoleBuffer;
	int consoleInitialized = 0;
	int consolePrintTrue = 1;
	int consolePrintTarget = 2; // 2

	void consoleInit()
	{
		if(consoleInitialized) return;
		consoleInitialized = 1;
		AllocConsole();
		GConsoleBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE, FILE_SHARE_READ, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		SetConsoleActiveScreenBuffer(GConsoleBuffer);	
		//SetWindowPos( GConsoleBuffer, 0, -800, -100, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
		//MoveWindow( GConsoleBuffer, -600, -100, 0, 0, TRUE);
	}

	void cdiag(char* AText)
	{
		if(!consolePrintTrue) return;
		if(consolePrintTarget) diag(AText);
		if(consolePrintTarget == 0 || consolePrintTarget == 2)
		{
			if(!consoleInitialized) consoleInit();
			WriteConsole(GConsoleBuffer, AText, str_len(AText), NULL, 0);
		}
	}
	
	#define cprintf0(str) cdiag(_chr(str))
	#define cprintf1(str,arg1) cdiag(_chr(str_printf(NULL,str,arg1)))
	#define cprintf2(str,arg1,arg2) cdiag(_chr(str_printf(NULL,str,arg1,arg2)))
	#define cprintf3(str,arg1,arg2,arg3) cdiag(_chr(str_printf(NULL,str,arg1,arg2,arg3)))
	#define cprintf4(str,arg1,arg2,arg3,arg4) cdiag(_chr(str_printf(NULL,str,arg1,arg2,arg3,arg4)))
	#define cprintf5(str,arg1,arg2,arg3,arg4,arg5) cdiag(_chr(str_printf(NULL,str,arg1,arg2,arg3,arg4,arg5)))
	#define cprintf6(str,arg1,arg2,arg3,arg4,arg5,arg6) cdiag(_chr(str_printf(NULL,str,arg1,arg2,arg3,arg4,arg5,arg6)))
	#define cprintf7(str,arg1,arg2,arg3,arg4,arg5,arg6,arg7) cdiag(_chr(str_printf(NULL,str,arg1,arg2,arg3,arg4,arg5,arg6,arg7)))
#endif
