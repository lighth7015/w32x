/*
 * Copyright (c) 2000 Masaru OKI
 * Copyright (c) 2001 TAMURA Kent
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
#include "w32x_priv.h"


BOOL DrawMenuBar(HWND hwnd)
{
	if (!IsWindow(hwnd))
		return FALSE;
	
	RECT rect;
	HDC hdc = GetDC(hwnd);
	
	HBRUSH MenuBrush = CreateSolidBrush(RGB(0xff, 0xff, 0xff));
	
	if (GetMenu(hwnd))
	{
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		GetClientRect(hwnd, &rect);
		
		rect.bottom = GetSystemMetrics(SM_CYMENU);
		SetDCBrushColor(hdc, RGB(0xaa, 0xbb, 0xcc));
		
		FillRect(hdc, &rect, MenuBrush);
	}
	
	
finished:
	return TRUE;
}

static int MenuWindowProc(HWND wnd, unsigned int msg, WPARAM wParam,
		LPARAM lParam);

WNDCLASS MenuClass = {
	"#32768", C_GRAY5, MenuWindowProc, 0
};

static int
MenuWindowProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{	
	PAINTSTRUCT ps;
	int result = 0;
	
	switch (msg) {
	case WM_PAINT:
		DrawMenuBar(hWnd);
		break;
	
	default:
		result = DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	
	return result;
}

BOOL
IsMenu(HMENU menu)
{
	BOOL result = FALSE;
	
	if (menu != NULL)
	{
		result = (menu->magic == MENU_MAGIC);
	}
	
	return result;
}

HMENU
CreateMenu(void)
{
	struct WndMenu *menu = calloc(1, sizeof(struct WndMenu));
	menu->magic = MENU_MAGIC;
	
	return menu;
}
