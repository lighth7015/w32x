/*
 * Copyright (c) 2017 Devin Smith <devin@devinsmith.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *		notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *		notice, this list of conditions and the following disclaimer in the
 *		documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

static int
MainWindowProc(HWND wnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(wnd, msg, wParam, lParam);
}

int 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	WNDCLASS myClass;
	BOOL bRet; MSG msg;
	
	myClass.Name = "TopWindow";
	myClass.BackgroundColor = C_GRAY2;
	myClass.wndExtra = 0;
	myClass.EventProc = MainWindowProc;
	RegisterClass(&myClass);

	HMENU menu = CreateMenu();

	/* parent window */
	HWND top = CreateWindowEx(WS_EX_CLIENTEDGE, "TopWindow", "Test1",
							  WS_OVERLAPPEDWINDOW, 200, 200, 500, 300, 
							  NULL, menu);
	
	CreateWindow("RadioButton", "Radio 1", WS_CHILD | WS_BORDER, 5, 30,
			100, 25, top, NULL);
	
	CreateWindow("RadioButton", "Radio 2", WS_CHILD | WS_BORDER, 5, 60,
			100, 25, top, NULL);
	
	ShowWindow(top);
	UpdateWindow(top);

	for (bRet = GetMessage(&msg, NULL) 
		;bRet != 0
		;bRet = GetMessage(&msg, NULL)) 
	{
		DispatchMessage(&msg);
	}

	return 0;
}
