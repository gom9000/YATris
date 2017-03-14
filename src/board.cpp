/*                                                     ''~``
 * @(#)board.cpp 1.0  2008/07/29                      ( o o )
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
 * | Module....: game board                                           |
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
#include "tris.h"


// Global Variables
extern GAMEINFO gameInfo;
extern HFONT hFontChalk;


void DrawBoard(HDC hDC, struct BOARDBITMAP *board)
{
    BITMAP bmp;
    RECT rectOpponentsText;
    TCHAR opponentsText[MAX_NAMELEN*2+4];
    HDC hMemDC = CreateCompatibleDC(hDC);

	// display blackboard
	HBITMAP hBmpOld = (HBITMAP)SelectObject(hMemDC, board->back);
	GetObject(board->back, sizeof(bmp), &bmp);
	BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);

    // display grid
    SelectObject(hMemDC, board->mask);
    BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCAND);
    SelectObject(hMemDC, board->grid);
    BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCPAINT);

    // display opponents
    SelectObject(hDC, hFontChalk);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(240,240,240));
    SetRect(&rectOpponentsText,30,45,560,80);
	wsprintf(opponentsText, "%s vs %s", gameInfo.player[PLAYER1].name, gameInfo.player[PLAYER2].name);
	DrawText(hDC, opponentsText, -1, &rectOpponentsText, DT_CENTER|DT_VCENTER);

    SelectObject(hMemDC, hBmpOld);
    DeleteDC(hMemDC);
}


void DrawPieces(HDC hDC, struct PIECEBITMAP *pieces)
{
    BITMAP bmp;
	char pieceType;
    HDC hMemDC = CreateCompatibleDC(hDC);

	HBITMAP hBmpOld = (HBITMAP)SelectObject(hMemDC, pieces[PLAYER1].piece);
	GetObject(pieces[PLAYER1].piece, sizeof(bmp), &bmp);

    // display pieces loop
	for ( int ii=0; ii<3; ii++ )
        for ( int jj=0; jj<3; jj++ )
		    if ( (pieceType = gameInfo.cells[jj][ii]) != NOPIECE ) {
                SelectObject(hMemDC, pieces[pieceType].mask);
                BitBlt(hDC, 130 + ii*125, 100 + jj*105, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCAND);
                SelectObject(hMemDC, pieces[pieceType].piece);
                BitBlt(hDC, 130 + ii*125, 100 + jj*105, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCPAINT);
			}

	SelectObject(hMemDC, hBmpOld);
    DeleteDC(hMemDC);
}


void DrawPiece(HDC hDC, struct PIECEBITMAP *pieces, char pieceType, int x, int y)
{
    BITMAP bmp;
    HDC hMemDC = CreateCompatibleDC(hDC);

	HBITMAP hBmpOld = (HBITMAP)SelectObject(hMemDC, pieces[PLAYER1].piece);
	GetObject(pieces[PLAYER1].piece, sizeof(bmp), &bmp);

    // display pieces loop
    SelectObject(hMemDC, pieces[pieceType].mask);
    BitBlt(hDC, x, y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCAND);
    SelectObject(hMemDC, pieces[pieceType].piece);
    BitBlt(hDC, x+2, y+2, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCPAINT);

	SelectObject(hMemDC, hBmpOld);
    DeleteDC(hMemDC);
}


// transform screen coords (x, y) in game cells (x=row, y=col)
BOOL Coord2Cell(POINT coord, POINT *cell)
{
	BOOL valid = TRUE;
    cell->y = (int)( (coord.x - 110) / 120 );
    cell->x = (int)( (coord.y - 82) / 105 );
	if (cell->x < 0) { cell->x = 0; valid = FALSE; }
   	if (cell->y < 0) { cell->y = 0; valid = FALSE; }
	if (cell->x > 2) { cell->x = 2; valid = FALSE; }
   	if (cell->y > 2) { cell->y = 2; valid = FALSE; }

	return (valid);
}
