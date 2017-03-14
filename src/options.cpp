/*                                                     ''~``
 * @(#)options.cpp 1.0  2008/07/29                    ( o o )
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
 * | Module....: dialog options                                       |
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


void CopyGameInfo(GAMEINFO *dest, const GAMEINFO *src);


// Global Variables
extern GAMEINFO gameInfo;
GAMEINFO tempInfo;
int selectedIndexComboPlayer1, selectedIndexComboPlayer2;


BOOL CALLBACK OptionsDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hComboPlayer1 = GetDlgItem(hWnd, IDC_COMBO_PLAYER_1);
	HWND hComboPlayer2 = GetDlgItem(hWnd, IDC_COMBO_PLAYER_2);
	HWND hEditPlayer1 = GetDlgItem(hWnd, IDC_EDIT_PLAYER_1);
	HWND hEditPlayer2 = GetDlgItem(hWnd, IDC_EDIT_PLAYER_2);

	int ii;

    UNREFERENCED_PARAMETER(lParam);

    switch ( msg ) {

		case WM_INITDIALOG:

		    CopyGameInfo(&tempInfo, &gameInfo);

			for ( ii=PLAYER1; ii<=PLAYER2; ii++ ) {
		        SendMessage(hComboPlayer1, CB_ADDSTRING, (WPARAM)NULL, (LPARAM)gameInfo.playerTypeName[ii]);
				SendMessage(hComboPlayer2, CB_ADDSTRING, (WPARAM)NULL, (LPARAM)gameInfo.playerTypeName[ii]);
			}
			SendMessage(hComboPlayer1, CB_SETCURSEL, gameInfo.player[PLAYER1].type, (LPARAM)NULL);
			SendMessage(hComboPlayer2, CB_SETCURSEL, gameInfo.player[PLAYER2].type, (LPARAM)NULL);
            selectedIndexComboPlayer1 = gameInfo.player[PLAYER1].type;
            selectedIndexComboPlayer2 = gameInfo.player[PLAYER2].type;

			SetDlgItemText(hWnd, IDC_EDIT_PLAYER_1, gameInfo.player[PLAYER1].name);
			SetDlgItemText(hWnd, IDC_EDIT_PLAYER_2, gameInfo.player[PLAYER2].name);
			if ( gameInfo.player[PLAYER1].type == COMPUTER ) EnableWindow(hEditPlayer1, FALSE);
			if ( gameInfo.player[PLAYER2].type == COMPUTER ) EnableWindow(hEditPlayer2, FALSE);

			CheckRadioButton(hWnd, IDC_RADIO_LEVEL_1, IDC_RADIO_LEVEL_3, 
				            (tempInfo.level==EASY? IDC_RADIO_LEVEL_1 : (tempInfo.level==MEDIUM? IDC_RADIO_LEVEL_2 : IDC_RADIO_LEVEL_3)));
		return TRUE;

		case WM_COMMAND:

			switch ( LOWORD(wParam) ) {
                case IDC_COMBO_PLAYER_1:
                    if ( HIWORD(wParam) == CBN_SELCHANGE ) {

						int newIndex = SendMessage(hComboPlayer1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

                        if ( newIndex != selectedIndexComboPlayer1 ) {
                            selectedIndexComboPlayer1 = newIndex;
							if ( tempInfo.player[PLAYER1].type == HUMAN ) {
								tempInfo.player[PLAYER1].type = COMPUTER;
								if ( tempInfo.player[PLAYER2].type == HUMAN ) tempInfo.playmode = MIXED_MODE;
								else tempInfo.playmode = COMPUTER_MODE;
								GetDlgItemText(hWnd, IDC_EDIT_PLAYER_1, gameInfo.playerTempName[PLAYER1], MAX_NAMELEN);
								SetDlgItemText(hWnd, IDC_EDIT_PLAYER_1, gameInfo.playerTypeName[COMPUTER]);
								EnableWindow(hEditPlayer1, FALSE);
							} else {
								tempInfo.player[PLAYER1].type = HUMAN;
								if ( tempInfo.player[PLAYER2].type == HUMAN ) tempInfo.playmode = HUMAN_MODE;
								else tempInfo.playmode = MIXED_MODE;
								SetDlgItemText(hWnd, IDC_EDIT_PLAYER_1, gameInfo.playerTempName[PLAYER1]);
								EnableWindow(hEditPlayer1, TRUE);
							}
						}
                    }
                break;

                case IDC_COMBO_PLAYER_2:
                    if ( HIWORD(wParam) == CBN_SELCHANGE ) {

						int newIndex = SendMessage(hComboPlayer2, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

                        if ( newIndex != selectedIndexComboPlayer2 ) {
                            selectedIndexComboPlayer2 = newIndex;
							if ( newIndex == COMPUTER ) {
								tempInfo.player[PLAYER2].type = COMPUTER;
								if ( tempInfo.player[PLAYER1].type == HUMAN ) tempInfo.playmode = MIXED_MODE;
								else tempInfo.playmode = COMPUTER_MODE;
								GetDlgItemText(hWnd, IDC_EDIT_PLAYER_2, gameInfo.playerTempName[PLAYER2], MAX_NAMELEN);
								SetDlgItemText(hWnd, IDC_EDIT_PLAYER_2, gameInfo.playerTypeName[COMPUTER]);
							    EnableWindow(hEditPlayer2, FALSE);
							} else {
								tempInfo.player[PLAYER2].type = HUMAN;
								if ( tempInfo.player[PLAYER1].type == HUMAN ) tempInfo.playmode = HUMAN_MODE;
								else tempInfo.playmode = MIXED_MODE;
								SetDlgItemText(hWnd, IDC_EDIT_PLAYER_2, gameInfo.playerTempName[PLAYER2]);
								EnableWindow(hEditPlayer2, TRUE);
							}
						}
                    }
                break;

                case IDC_RADIO_LEVEL_1:
					tempInfo.level = EASY;
				    CheckRadioButton(hWnd, IDC_RADIO_LEVEL_1, IDC_RADIO_LEVEL_3, IDC_RADIO_LEVEL_1);
				break;

				case IDC_RADIO_LEVEL_2:
					tempInfo.level = MEDIUM;
				    CheckRadioButton(hWnd, IDC_RADIO_LEVEL_1, IDC_RADIO_LEVEL_3, IDC_RADIO_LEVEL_2);
				break;

				case IDC_RADIO_LEVEL_3:
					tempInfo.level = HARD;
				    CheckRadioButton(hWnd, IDC_RADIO_LEVEL_1, IDC_RADIO_LEVEL_3, IDC_RADIO_LEVEL_3);
				break;

				case IDOK:
				{
					TCHAR tmp1[MAX_NAMELEN], tmp2[MAX_NAMELEN];
					GetDlgItemText(hWnd, IDC_EDIT_PLAYER_1, tmp1, MAX_NAMELEN);
					GetDlgItemText(hWnd, IDC_EDIT_PLAYER_2, tmp2, MAX_NAMELEN);

					if ( !strlen(tmp1) ) {
						MessageBox(NULL, "Insert Player 1 name", "Warning!", MB_ICONEXCLAMATION | MB_OK);
					} else if ( !strlen(tmp2) ) {
						MessageBox(NULL, "Insert Player 2 name", "Warning!", MB_ICONEXCLAMATION | MB_OK);
					} else {
						GetDlgItemText(hWnd, IDC_EDIT_PLAYER_1, tempInfo.player[PLAYER1].name, MAX_NAMELEN);
						GetDlgItemText(hWnd, IDC_EDIT_PLAYER_2, tempInfo.player[PLAYER2].name, MAX_NAMELEN);
						tempInfo.initialized = TRUE;
						CopyGameInfo(&gameInfo, &tempInfo);
						EndDialog(hWnd, IDOK);
					}
				}
                break;

				case IDCANCEL:
                    EndDialog(hWnd, IDCANCEL);
                break;
			}
        break;

        case WM_CLOSE:
			EndDialog(hWnd, IDCANCEL);
		break;

		default:
            return FALSE;
    }

	return TRUE;
}
