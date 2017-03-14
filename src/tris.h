/*                                                     ''~``
 * @(#)tris.h 1.0  2008/07/29                         ( o o )
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
 * | Module....: main header file                                     |
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


#include "resource.h"


#define TAG      -1                                /* la casella è occupata */



/* constants */

#define MAX_NAMELEN 12

// player stats indexes
#define STATS_SIZE      8
#define STATS_ROW_IDX   0
#define STATS_COL_IDX   3
#define STATS_DIAG1_IDX 6
#define STATS_DIAG2_IDX 7

// pieces
#define NOPIECE -1
#define CIRCLE   0
#define CROSS    1

// levels
#define EASY     0
#define MEDIUM   1
#define HARD     2

// players
#define PLAYER1  0
#define PLAYER2  1

// players type
#define HUMAN    0
#define COMPUTER 1

// opponents types
#define HUMAN_MODE    0
#define MIXED_MODE    1
#define COMPUTER_MODE 2


// Blackboard bitmap struct
struct BOARDBITMAP {
	HBITMAP back;
	HBITMAP grid;
    HBITMAP mask;
};

// Pieces bitmap struct
struct PIECEBITMAP {
	HBITMAP piece;
    HBITMAP mask;
};


/* struttura contenente i dati del giocatore */
typedef struct pinfo_struct {  
    TCHAR name[MAX_NAMELEN];                          /* nome del giocatore */
    int   type;                               /* FALSE=umano, TRUE=computer */
    int   piece;            /* pedina associata al giocatore: CIRCLE, CROSS */
    int   weight[3][3];                                 /* potenziali campo */
    int   stats[8];                                     /* vettore di stato */
} PLAYERINFO;

/* struttura contenente i dati di gioco */
typedef struct ginfo_struct {      
    int        level;                       /* livello di gioco selezionato */
    int        playmode;      /* modalità di gioco: umano, misto o computer */
	PLAYERINFO player[2];                     /* 0=giocatore1, 1=giocatore2 */
	BOOL       playing;               /* vale TRUE se la partita è in corso */
	BOOL       initialized;    /* vale TRUE se la struttura è inizializzata */
	int        turn;                            /* indica il turno di gioco */
	int        moved;                  /* numero di mosse valide effettuate */
    TCHAR      playerTypeName[2][MAX_NAMELEN];    /* tipologia di giocatore */
    TCHAR      playerTempName[2][MAX_NAMELEN]; /* nome iniziale dei giocat. */
	int        cells[3][3];                     /* celle del campo di gioco */
} GAMEINFO;
