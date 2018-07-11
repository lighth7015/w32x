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

#include <string.h>
#include <windows.h>

#define RBDIAM 11
#define RB_X 8

struct RadioButtonInfo {
	int activated;
};

static int RadioButtonProc(HWND wnd, unsigned int msg, WPARAM wParam,
		LPARAM lParam);

WNDCLASS ButtonClass = {
	"RadioButton", C_GRAY5, RadioButtonProc, sizeof(struct RadioButtonInfo)
};

static void
drawRadioButton(HWND wnd, RECT r, int clicked)
{
	struct RadioButtonInfo *extra;
	char label[256];
	
	GetWindowText(wnd, label, sizeof(label));
	extra = GetWindowLongPtr(wnd, 0);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(wnd, &ps);

	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SelectObject(hdc, GetStockObject(BLACK_PEN));

	unsigned int color = (clicked) ? RGB(0x80, 0x80, 0x80)
								   : RGB(0xe0, 0xe0, 0xe0);
	
	SetDCBrushColor(hdc, color);
	
	if (clicked) {
		extra->activated = extra->activated ? 0 : 1;
	}
	
	int h = r.bottom,
		w = r.right;
	
	Rectangle(hdc, 0, 0, w - 1, h - 1);
	Rectangle(hdc, 1, 1, w - 2, h - 2);
	
	/*
	if (extra->activated) {
		SetDCBrushColor(hdc, RGB(0, 0xff, 0));
	} else {
		SetDCBrushColor(hdc, RGB(0xc0, 0xc0, 0xc0));
	}
	
	Ellipse(hdc, RB_X, (h + RBDIAM) / 2, RB_X + RBDIAM,
			((h + RBDIAM) / 2) + RBDIAM);
	
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	//TextOut(hdc, (RB_X * 2) + RBDIAM, (h * 3) + h, label, strlen(label));
	*/
}

static int
RadioButtonProc(HWND wnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	RECT winRect;
	GetClientRect(wnd, &winRect);
	
	int activated = 0;
	
	switch (msg) 
	{
	case WM_LBUTTONDOWN:
		activated = 1;
		break;
	
	case WM_LBUTTONUP:
		break;
	
	case WM_PAINT:
		break;
	
	default:
		return DefWindowProc(wnd, msg, wParam, lParam);
		break;
	}
	
	drawRadioButton(wnd, winRect, activated);
	return 1;
}
