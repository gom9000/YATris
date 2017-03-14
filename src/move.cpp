/*                                                     ''~``
 * @(#)move.cpp 1.0  2008/07/29                       ( o o )
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
 * | Module....: evaluation and move routines                         |
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


void  UpdateStats(POINT cell);
void  UpdateOpponentWeight(POINT cell);
POINT Stats2Cell(int idx);
void  Debug(POINT cell);


// Global Variables
extern GAMEINFO gameInfo;
extern char cellsWeight[3][3][3];



int GetNextTurn(void) {
    return (gameInfo.turn == PLAYER1)? PLAYER2 : PLAYER1;
}


BOOL Move(POINT cell) {
    BOOL res = FALSE;

    if ( gameInfo.cells[cell.x][cell.y] == NOPIECE ) {
	    gameInfo.cells[cell.x][cell.y] = gameInfo.player[gameInfo.turn].piece;
        UpdateStats(cell);
		UpdateOpponentWeight(cell);
		gameInfo.moved++;
		res = TRUE;
//        Debug(cell);
	}

    return res;
}


BOOL WinnerMove(void) {

	for( int ii=0; ii < STATS_SIZE; ii++ )
        if ( gameInfo.player[gameInfo.turn].stats[ii] == 3 )
		    return TRUE;

	return FALSE;
}


void UpdateStats(POINT cell)
{
   gameInfo.player[gameInfo.turn].stats[STATS_ROW_IDX + cell.x] += 1;
   gameInfo.player[gameInfo.turn].stats[STATS_COL_IDX + cell.y] += 1;

   if ( cell.x == cell.y )
	   gameInfo.player[gameInfo.turn].stats[STATS_DIAG1_IDX] += 1;

   if ( cell.x + cell.y == 2 )
	   gameInfo.player[gameInfo.turn].stats[STATS_DIAG2_IDX] += 1;
}


void UpdateOpponentWeight(POINT cell)
{
    int row, col, kk;
    int opponent = GetNextTurn();

    gameInfo.player[gameInfo.turn].weight[cell.x][cell.y] = TAG;
    gameInfo.player[opponent].weight[cell.x][cell.y] = TAG;

    // update rows
    for ( row=0, kk=0; row<3; row++, kk++ )
        for ( col=0; col<3; col++)
	        if ( gameInfo.player[opponent].weight[row][col] > 0  &&  gameInfo.player[gameInfo.turn].stats[STATS_ROW_IDX + kk] > 0 )
	            gameInfo.player[opponent].weight[row][col] = cellsWeight[gameInfo.level][row][col] - 1;

    // update cols
    for ( col=0, kk=0; col<3; col++, kk++ )
        for ( row=0; row<3; row++)
	        if ( gameInfo.player[opponent].weight[row][col] > 0  &&  gameInfo.player[gameInfo.turn].stats[STATS_COL_IDX + kk] > 0 )
	            gameInfo.player[opponent].weight[row][col] -= 1;

    // update diag 1
	for ( row=0; row<3; row++ )
	    if ( gameInfo.player[opponent].weight[row][row] > 0  &&  gameInfo.player[gameInfo.turn].stats[STATS_DIAG1_IDX] > 0 )
	        gameInfo.player[opponent].weight[row][row] -= 1;

    // update diag 2
	for ( row=0; row<3; row++)
	    if ( gameInfo.player[opponent].weight[row][2-row] > 0  &&  gameInfo.player[gameInfo.turn].stats[STATS_DIAG2_IDX] > 0 )
	        gameInfo.player[opponent].weight[row][2-row] -= 1;
}


POINT ComputerMove(void)
{
	POINT cell;
    int ii, jj, val = TAG;
    int opponent = GetNextTurn();


	// do winner move
    for ( ii=0; ii < STATS_SIZE; ii++ )
        if( (gameInfo.player[gameInfo.turn].stats[ii] == 2) && (gameInfo.player[opponent].stats[ii] == 0) )
	        return Stats2Cell(ii);


	// do defence move
	for ( ii=0; ii < STATS_SIZE; ii++ )
        if ( (gameInfo.player[opponent].stats[ii] == 2) && (gameInfo.player[gameInfo.turn].stats[ii] == 0) )
		    return Stats2Cell(ii);


	// do level move
    switch (gameInfo.level) {
		case EASY:
        case MEDIUM:
		{
	        // do first best-weight move.
	        for ( ii=0; ii < 3; ii++ )
                for ( jj=0; jj < 3; jj++ )
	                if( gameInfo.player[gameInfo.turn].weight[ii][jj] > val ) {
	                    val = gameInfo.player[gameInfo.turn].weight[ii][jj];
                        cell.x = ii;
                        cell.y = jj;
                    }
		}
        break;

        case HARD:
		{
	        // do first best-stats move.
		    // la mossa che fa ottenere il massimo numero di 2 nel vettore stats del giocatore.
		    // (ed il minor punteggio nello stesso vettore dell'opponente dopo che esso muove)
			int kk, initCount, tempCount, tempDelta = TAG;
			int tempStats[8];
			POINT tempCell;

			for ( kk=0, initCount=0; kk < STATS_SIZE; kk++)
				if ( gameInfo.player[gameInfo.turn].stats[kk] == 2 && gameInfo.player[opponent].stats[kk] == 0 )
				    initCount++;

			for ( ii=0; ii < 3; ii++ )
                for ( jj=0; jj < 3; jj++ ) {
				    if( gameInfo.player[gameInfo.turn].weight[ii][jj] != TAG ) {
						tempCell.x = ii;
						tempCell.y = jj;
					
				        // simulate update stats
						for ( kk=0; kk < STATS_SIZE; kk++) tempStats[kk] = gameInfo.player[gameInfo.turn].stats[kk];
                        tempStats[STATS_ROW_IDX + tempCell.x] += 1;
                        tempStats[STATS_COL_IDX + tempCell.y] += 1;
                        if ( tempCell.x == tempCell.y ) tempStats[STATS_DIAG1_IDX] += 1;
                        if ( tempCell.x + tempCell.y == 2 ) tempStats[STATS_DIAG2_IDX] += 1;

						for ( kk=0, tempCount=0; kk < STATS_SIZE; kk++)
							if ( tempStats[kk] == 2  && gameInfo.player[opponent].stats[kk] == 0 ) 
							    tempCount++;

						if ( (tempCount + gameInfo.player[gameInfo.turn].weight[ii][jj] - initCount) > tempDelta ) {
							tempDelta = tempCount - initCount;
							cell.x = tempCell.x;
							cell.y = tempCell.y;
					    }
//				TCHAR aaa[MAX_NAMELEN];
//				wsprintf(aaa, "(%d,%d), initCount=%d, tempCount=%d,%d", tempCell.x, tempCell.y, initCount, tempCount, gameInfo.player[gameInfo.turn].weight[ii][jj]);
//                MessageBox(NULL, aaa, "Attenzione!", MB_ICONEXCLAMATION | MB_OK);

					}
		        }
		}
	    break;
	}

	return cell;
}


POINT Stats2Cell(int idx)
{
	POINT cell;
    int ii, jj;

    if ( idx < STATS_COL_IDX )
        for ( jj=0; jj < 3; jj++ )
	        if ( gameInfo.cells[idx][jj] == NOPIECE ) {
	            cell.x = idx;
	            cell.y = jj;
	            return cell;
	        }

	if ( idx < STATS_DIAG1_IDX )
        for ( ii=0; ii<3; ii++ )
	        if ( gameInfo.cells[ii][idx - STATS_COL_IDX] == NOPIECE ) {
	        cell.x = ii;
	        cell.y = idx - STATS_COL_IDX;
	        return cell;
	    }

    if ( idx == STATS_DIAG1_IDX )
        for ( ii=0; ii<3; ii++ )
	        if ( gameInfo.cells[ii][ii] == NOPIECE ) {
	            cell.x = cell.y = ii;
	            return cell;
	        }

    if ( idx == STATS_DIAG2_IDX )
        for ( ii=0; ii<3; ii++ )
	        if ( gameInfo.cells[ii][2-ii] == NOPIECE ) {
	            cell.x = ii;
	            cell.y = 2 - ii;
	            return cell;
	        }

    cell.x=cell.y=-1;
	return cell;
}


void Debug(POINT cell)
{
	TCHAR c[200];

	wsprintf(c, "cell = %d,%d\nplayer1 stats:\n[%d,%d,%d,%d,%d,%d,%d,%d]\n\nplayer1 weight:\n|%d %d %d|\n|%d %d %d|\n|%d %d %d|\n\n\nplayer2 stats:\n[%d,%d,%d,%d,%d,%d,%d,%d]\n\nplayer2 weight:\n|%d %d %d|\n|%d %d %d|\n|%d %d %d|\n",
		        cell.x, cell.y,
                gameInfo.player[PLAYER1].stats[0],
                gameInfo.player[PLAYER1].stats[1],
                gameInfo.player[PLAYER1].stats[2],
                gameInfo.player[PLAYER1].stats[3],
                gameInfo.player[PLAYER1].stats[4],
                gameInfo.player[PLAYER1].stats[5],
                gameInfo.player[PLAYER1].stats[6],
                gameInfo.player[PLAYER1].stats[7],

	            gameInfo.player[PLAYER1].weight[0][0],
                gameInfo.player[PLAYER1].weight[0][1],
                gameInfo.player[PLAYER1].weight[0][2],
                gameInfo.player[PLAYER1].weight[1][0],
		        gameInfo.player[PLAYER1].weight[1][1],
		        gameInfo.player[PLAYER1].weight[1][2],
		        gameInfo.player[PLAYER1].weight[2][0],
		        gameInfo.player[PLAYER1].weight[2][1],
		        gameInfo.player[PLAYER1].weight[2][2],


                gameInfo.player[PLAYER2].stats[0],
                gameInfo.player[PLAYER2].stats[1],
                gameInfo.player[PLAYER2].stats[2],
                gameInfo.player[PLAYER2].stats[3],
                gameInfo.player[PLAYER2].stats[4],
                gameInfo.player[PLAYER2].stats[5],
                gameInfo.player[PLAYER2].stats[6],
                gameInfo.player[PLAYER2].stats[7],

                gameInfo.player[PLAYER2].weight[0][0],
                gameInfo.player[PLAYER2].weight[0][1],
                gameInfo.player[PLAYER2].weight[0][2],
                gameInfo.player[PLAYER2].weight[1][0],
		        gameInfo.player[PLAYER2].weight[1][1],
		        gameInfo.player[PLAYER2].weight[1][2],
		        gameInfo.player[PLAYER2].weight[2][0],
		        gameInfo.player[PLAYER2].weight[2][1],
		        gameInfo.player[PLAYER2].weight[2][2]);
    MessageBox(NULL, c, "Debug", MB_ICONEXCLAMATION | MB_OK);
}
