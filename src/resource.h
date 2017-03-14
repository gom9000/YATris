/*                                                     ''~``
 * @(#)resource.h 1.0  2008/07/29                     ( o o )
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
 * | Module....: resource header file                                 |
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


// application constants
#define IDS_APP_TITLE			100
#define IDC_APP_WCLASSNAME   	101
#define IDI_APP_ICON			102
#define IDI_APP_ICON_SMALL   	103
#define IDF_CHALK_FONT    	    104

#define IDS_HUMAN_PLAYER        110
#define IDS_COMPUTER_PLAYER     111
#define IDS_PLAYER1_DEF_NAME    112
#define IDS_PLAYER2_DEF_NAME    113


// menu constants
#define IDR_APP_MENU			1000
#define ID_CMD_NEW 				1011
#define ID_CMD_OPTIONS			1012
#define ID_CMD_EXIT				1013
#define ID_CMD_ABOUT	    	1021

// menu option dialog constants
#define IDD_OPTIONS             1100
#define IDC_RADIO_LEVEL_1       1101
#define IDC_RADIO_LEVEL_2       1102
#define IDC_RADIO_LEVEL_3       1103
#define IDC_EDIT_PLAYER_1       1104
#define IDC_COMBO_PLAYER_1      1105
#define IDC_EDIT_PLAYER_2       1106
#define IDC_COMBO_PLAYER_2      1107
#define IDC_STATIC                -1

// menu about dialog constants
#define IDD_ABOUT				1221


// bitmap resources constants
#define IDB_BLACKBOARD           110
#define IDB_GRID                 111
#define IDB_CIRCLE               112
#define IDB_CROSS                113


// cursor resources constants
#define IDC_CHALK_CURSOR         120
#define IDC_CIRCLE_CURSOR        121
#define IDC_CROSS_CURSOR         122


// custom messages
#define MSG_MOVE				 WM_USER+1
#define MSG_END_GAME			 WM_USER+2
#define MSG_WINNER			     WM_USER+3
#define MSG_EQUAL			     WM_USER+4
