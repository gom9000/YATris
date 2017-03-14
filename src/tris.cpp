/*                                                     ''~``
 * @(#)tris.cpp 1.0  2008/07/29                       ( o o )
 * +---------------------------------------------.oooO--(_)--Oooo.----+
 * |   ________        _________________.________                     |
 * |  /  _____/  ____ /   _____/   __   \   ____/                     |
 * | /   \  ___ /  _ \\_____  \\____    /____  \                      |
 * | \    \_\  (  <_> )        \  /    //       \                     |
 * |  \______  /\____/_______  / /____//______  /                     |
 * |         \/              \/               \/                      |
 * | Copyright (c) 2008 by                                            |
 * | Alessandro Fraschetti (gos95@gommagomma.net)                     |
 * |                                                                  |
 * | Project...: YATris - Yet Another Tris Implementation             |
 * | Module....: WinMain                                              |
 * | Author....: Alessandro Fraschetti                                |
 * | Notes.....:                                                      |
 * |                                              .oooO               |
 * |                                             (     )    Oooo.     |
 * +-----------------------------------------------\  (----(    )-----+
 *                                                  \_ )    )  /
 * For more information about YATris visit:                ( _/
 *     http://gommagomma.net/gos95/YATris
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version. 
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#define STRICT
#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <commctrl.h>
#include "tris.h"


#define MAX_LOADSTRING 100


// Function Prototypes
ATOM             RegisterTheClass(HINSTANCE hInstance);
BOOL             CreateTheWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK TheWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HBITMAP          CreateBitmapMask(HBITMAP hBmp, COLORREF transparentColor);
BOOL CALLBACK    AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    OptionsDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void             InitGameInfo(HINSTANCE hInstance);
BOOL             NewGame(void);
void             DrawBoard(HDC hDC, struct BOARDBITMAP *board);
void             DrawPieces(HDC hDC, struct PIECEBITMAP *pieces);
void             DrawPiece(HDC hDC, struct PIECEBITMAP *pieces, char pieceType, int x, int y);
BOOL             Coord2Cell(POINT coord, POINT *cell);
BOOL             Move(POINT cell);
POINT            ComputerMove(void);
int              GetNextTurn(void);
BOOL             WinnerMove(void);


// Global Variables
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
struct BOARDBITMAP board;
struct PIECEBITMAP pieces[2];
HCURSOR cursors[3];
HFONT hFontChalk;

GAMEINFO gameInfo;



/*
 * WinMain
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;

    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(hPrevInst);

	InitCommonControls();

    // Initialize Global Strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_APP_WCLASSNAME, szWindowClass, MAX_LOADSTRING);

	//Registering the Window Class
    if ( !RegisterTheClass(hInstance) ) {
        MessageBox(NULL, "Window Registration Failed!", "Errore!", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // Creating the Window
	if ( !CreateTheWindow(hInstance, nCmdShow) ) {
		MessageBox(NULL, "Window Creation Failed!", "Errore!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

    // The Message Loop
    while ( GetMessage(&msg, NULL, 0, 0) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
    }

	return msg.wParam;
}


ATOM RegisterTheClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize         = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)TheWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor		= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CHALK_CURSOR));
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_APP_MENU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm        = (HICON)LoadImage(wcex.hInstance, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 16, 16, LR_CREATEDIBSECTION);

	return RegisterClassEx(&wcex);
}


BOOL CreateTheWindow(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

    hWnd = CreateWindowEx(
		WS_EX_WINDOWEDGE,
        szWindowClass,
        szTitle,
		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 592+6, 443+50,
        NULL, NULL, hInstance, NULL
	);

   if (!hWnd) return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// The Window Procedure
LRESULT CALLBACK TheWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
    POINT coord, cell;
	int nResult = IDCANCEL;

	switch(msg)
    {

        case WM_CREATE:
		{
			LOGFONT logFont;
			int nFontLoaded = 0;

            // initialize game objects
			InitGameInfo(hInstance);

            // initialize graphic objects
			board.back = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BLACKBOARD));
			board.grid = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GRID));
			board.mask = CreateBitmapMask(board.grid, RGB(0, 0, 0));

			pieces[PLAYER1].piece = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CIRCLE));
			pieces[PLAYER1].mask = CreateBitmapMask(pieces[PLAYER1].piece, RGB(0, 0, 0));
			pieces[PLAYER2].piece = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CROSS));
			pieces[PLAYER2].mask = CreateBitmapMask(pieces[PLAYER2].piece, RGB(0, 0, 0));

            cursors[PLAYER1] = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CIRCLE_CURSOR));
			cursors[PLAYER2] = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CROSS_CURSOR));
			cursors[2] = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CHALK_CURSOR));


			nFontLoaded = AddFontResource("erasier.ttf");
//			if ( nFontLoaded==0 ) MessageBox(NULL, "Font Load Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

			logFont.lfHeight = 26;
			logFont.lfWidth = 12;
			logFont.lfEscapement = 4; 
			logFont.lfOrientation = 0; 
  			logFont.lfWeight = FW_BOLD; 
            logFont.lfItalic = TRUE; 
            logFont.lfUnderline = TRUE; 
            logFont.lfStrikeOut = FALSE; 
            logFont.lfCharSet = ANSI_CHARSET; 
            logFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
            logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
            logFont.lfQuality = DEFAULT_QUALITY; 
            logFont.lfPitchAndFamily = DEFAULT_PITCH; 
            strcpy(logFont.lfFaceName, "courier new");

			hFontChalk = CreateFontIndirect(&logFont);
		}
		break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
        break;

		case WM_DESTROY:
			DeleteObject(board.back);
		    DeleteObject(board.grid);
			DeleteObject(board.mask);
			DeleteObject(pieces[PLAYER1].piece);
			DeleteObject(pieces[PLAYER1].mask);
			DeleteObject(pieces[PLAYER2].piece);
			DeleteObject(pieces[PLAYER2].mask);
			DeleteObject(cursors[PLAYER1]);
			DeleteObject(cursors[PLAYER2]);
			DeleteObject(cursors[2]);
			DeleteObject(hFontChalk);
			RemoveFontResource("eraser.ttf");
            PostQuitMessage(0);
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);

            DrawBoard(hDC, &board);
            if ( gameInfo.initialized ) DrawPieces(hDC, pieces);

            EndPaint(hWnd, &ps);
        }
        break;

        case WM_LBUTTONUP:
			if ( gameInfo.playing ) {
                coord.x = LOWORD(lParam);
				coord.y = HIWORD(lParam);
			    if ( Coord2Cell(coord, &cell) )
					PostMessage(hWnd, MSG_MOVE, (WPARAM)cell.x, (LPARAM)cell.y);
		    } else
                DefWindowProc(hWnd, msg, wParam, lParam);
		break;

        case WM_COMMAND:
		    switch ( LOWORD(wParam) ) {

				case ID_CMD_NEW:
				    if ( gameInfo.playing ) {
						nResult = MessageBox(NULL, "Stop the running game to start another?", "Warning!", MB_ICONEXCLAMATION | MB_YESNO);
						if ( nResult == IDYES )	{
                            PostMessage(hWnd, MSG_END_GAME, (WPARAM)ID_CMD_NEW, (LPARAM)NULL);
							break;
						}
						break;
					}
					if ( !gameInfo.initialized ) {
						SendMessage(hWnd, WM_COMMAND, ID_CMD_OPTIONS, (LPARAM)ID_CMD_NEW);
						break;
					}
					gameInfo.playing = NewGame();
					SetClassLong(hWnd, GCL_HCURSOR, (LONG)cursors[gameInfo.turn]);
					if ( gameInfo.player[gameInfo.turn].type == COMPUTER ) {
						cell = ComputerMove();
						PostMessage(hWnd, MSG_MOVE, (WPARAM)cell.x, (LPARAM)cell.y);
					}
					InvalidateRect(hWnd, NULL, TRUE);

                break;

                case ID_CMD_OPTIONS:
					if ( gameInfo.playing ) {
					    nResult = MessageBox(NULL, "Stop the running game?", "Warning!", MB_ICONEXCLAMATION | MB_YESNO);
						if ( nResult == IDYES )	PostMessage(hWnd, MSG_END_GAME, (WPARAM)ID_CMD_OPTIONS, (LPARAM)NULL);
						break;
				    }
					nResult = DialogBox(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), hWnd, OptionsDlgProc);
				    if ( nResult == IDOK ) {
						InvalidateRect(hWnd, NULL, TRUE);
  				        if ( LOWORD(lParam) ) SendMessage(hWnd, WM_COMMAND, ID_CMD_NEW, (LPARAM)NULL);
					}
                break;

                case ID_CMD_EXIT:
					PostMessage(hWnd, WM_CLOSE, (WPARAM)NULL, (LPARAM)NULL);
                break;

				case ID_CMD_ABOUT:
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
                break;
            }
        break;

		case MSG_MOVE:
            cell.x = wParam;
            cell.y = lParam;
			if ( Move(cell) ) {
				if ( WinnerMove() ) {
					// winner!
					PostMessage(hWnd, MSG_END_GAME, (WPARAM)MSG_WINNER, (LPARAM)NULL);
				} else {
					if ( gameInfo.moved < 9 ) {
					    gameInfo.turn = GetNextTurn();
						SetClassLong(hWnd, GCL_HCURSOR, (LONG)cursors[gameInfo.turn]);
						InvalidateRect(hWnd, NULL, TRUE);
						// ... timer...???
						if ( gameInfo.player[gameInfo.turn].type == COMPUTER ) {
							cell = ComputerMove();
							PostMessage(hWnd, MSG_MOVE, (WPARAM)cell.x, (LPARAM)cell.y);
					    }
					} else {
						// equal!
						PostMessage(hWnd, MSG_END_GAME, (WPARAM)MSG_EQUAL, (LPARAM)NULL);
					}
				}
			} else {
				TCHAR playerName[MAX_NAMELEN];
				wsprintf(playerName, "This is an invalid move, try again %s!", gameInfo.player[gameInfo.turn].name);
                MessageBox(NULL, playerName, "Attenzione!", MB_ICONEXCLAMATION | MB_OK);
			}
		break;

        case MSG_END_GAME:
			gameInfo.playing = FALSE;
		    SetClassLong(hWnd, GCL_HCURSOR, (LONG)cursors[2]);
			InvalidateRect(hWnd, NULL, TRUE);

			switch ( LOWORD(wParam) ) {
				case ID_CMD_NEW:
					SendMessage(hWnd, WM_COMMAND, ID_CMD_NEW, (LPARAM)NULL);
				break;

				case ID_CMD_OPTIONS:
					SendMessage(hWnd, WM_COMMAND, ID_CMD_OPTIONS, (LPARAM)NULL);
				break;

				case MSG_WINNER:
				{
					TCHAR theWinner[MAX_NAMELEN];
					wsprintf(theWinner, "The winner is %s!", gameInfo.player[gameInfo.turn].name);
					MessageBox(NULL, theWinner, "Win!", MB_ICONEXCLAMATION | MB_OK);
				}
				break;

				case MSG_EQUAL:
					MessageBox(NULL, "The match ended in a draw.", "Draw!", MB_ICONEXCLAMATION | MB_OK);
				break;
			}
		break;

		default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

	return 0;
}


BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);

    switch ( msg ) {

		case WM_INITDIALOG:
        return TRUE;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
            DrawPiece(hDC, pieces, CROSS, 230, 6);
			DrawPiece(hDC, pieces, CIRCLE, 230+41, 26);
            EndPaint(hWnd, &ps);
        }
        break;

		case WM_COMMAND:
            switch ( LOWORD(wParam) ) {
				case IDOK:
                    EndDialog(hWnd, IDOK);
                break;
			}
        break;

        case WM_CLOSE:
			EndDialog(hWnd, 0);
		break;

		default:
            return FALSE;
    }

	return TRUE;
}


HBITMAP CreateBitmapMask(HBITMAP hBmp, COLORREF transparentColor)
{
	HDC hMemDC, hMemDC2;
	HBITMAP hBmpMask;
	BITMAP bmp;

	GetObject(hBmp, sizeof(BITMAP), &bmp);
	hBmpMask = CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);

	hMemDC = CreateCompatibleDC(0);
	hMemDC2 = CreateCompatibleDC(0);

	SelectObject(hMemDC, hBmp);
	SelectObject(hMemDC2, hBmpMask);

	SetBkColor(hMemDC, transparentColor);

	BitBlt(hMemDC2, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
	BitBlt(hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC2, 0, 0, SRCINVERT);

	DeleteDC(hMemDC);
	DeleteDC(hMemDC2);

	return hBmpMask;
}
