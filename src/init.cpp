/*                                                     ''~``
 * @(#)init.cpp 1.0  2008/07/29                       ( o o )
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
 * | Module....: initialization routines                              |
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


void InitCells(void);
void InitStats(void);


// Global Variables
extern GAMEINFO gameInfo;


/* peso di ogni cella nei tre livelli di gioco */
char cellsWeight[3][3][3] = { {{1, 1, 1},{1, 1, 1},{1, 1, 1}},
	                          {{3, 2, 3},{2, 4, 2},{3, 2, 3}},
	                          {{3, 2, 3},{2, 4, 2},{3, 2, 3}} };

/* bonus aggiuntivo di ogni cella nei tre livelli di gioco */
char cellsBonus[3][3][3] = { {{0, 0, 0},{0, 0, 0},{0, 0, 0}},
	                         {{2, 0, 0},{0, 0, 0},{0, 0, 0}},
	                         {{2, 0, 0},{0, 0, 0},{0, 0, 0}} };



void InitGameInfo(HINSTANCE hInstance)
{
    // init array playerTypeName
	LoadString(hInstance, IDS_HUMAN_PLAYER, gameInfo.playerTypeName[PLAYER1], MAX_NAMELEN);
	LoadString(hInstance, IDS_COMPUTER_PLAYER, gameInfo.playerTypeName[PLAYER2], MAX_NAMELEN);

	// init array playerTempName
	LoadString(hInstance, IDS_PLAYER1_DEF_NAME, gameInfo.playerTempName[PLAYER1], MAX_NAMELEN);
	LoadString(hInstance, IDS_PLAYER2_DEF_NAME, gameInfo.playerTempName[PLAYER2], MAX_NAMELEN);

    // init struct GAMEINFO
	gameInfo.level       = MEDIUM;
	gameInfo.playmode    = MIXED_MODE;
	gameInfo.playing     = FALSE;
	gameInfo.initialized = FALSE;

	// init struct PLAYERINFO
	//   player 1 is human
	strcpy(gameInfo.player[PLAYER1].name, gameInfo.playerTempName[HUMAN]);
    gameInfo.player[PLAYER1].type = HUMAN;
	gameInfo.player[PLAYER1].piece = CIRCLE;
	//   player 2 is computer
	strcpy(gameInfo.player[PLAYER2].name, gameInfo.playerTypeName[COMPUTER]);
	gameInfo.player[PLAYER2].type = COMPUTER;
	gameInfo.player[PLAYER2].piece = CROSS;

    // init game board
    InitCells();
}


void CopyGameInfo(GAMEINFO *dest, const GAMEINFO *src)
{
	dest->level       = src->level;
    dest->playmode    = src->playmode;
	dest->playing     = src->playing;
	dest->initialized = src->initialized;

	for (int ii=PLAYER1; ii<=PLAYER2; ii++) {
		strcpy(dest->player[ii].name, src->player[ii].name);
	    dest->player[ii].type = src->player[ii].type;
        dest->player[ii].piece = src->player[ii].piece;
	}
}


void InitCells(void)
{
    for ( int ii=0; ii<3; ii++ )
        for ( int jj=0; jj<3; jj++ ) {
		    gameInfo.cells[ii][jj] = NOPIECE;
	        gameInfo.player[PLAYER1].weight[ii][jj] = cellsWeight[gameInfo.level][ii][jj] + cellsBonus[gameInfo.level][ii][jj];
			gameInfo.player[PLAYER2].weight[ii][jj] = cellsWeight[gameInfo.level][ii][jj];
        }
}


void InitStats(void) {
    for ( int ii=0; ii<STATS_SIZE; ii++) {
	    gameInfo.player[PLAYER1].stats[ii] = 0;
		gameInfo.player[PLAYER2].stats[ii] = 0;
	}
}


BOOL NewGame(void) {

    InitCells();
	InitStats();
	gameInfo.turn = PLAYER1;
	gameInfo.moved = 0;

    return TRUE;
}
