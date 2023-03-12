
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  
#define BOARD_SIZE 8        // board size
#define ROWS_WITH_PIECES 3  // number of initial rows with pieces
#define CELL_EMPTY '.'      // empty cell character
#define CELL_BPIECE 'b'     // black piece character
#define CELL_WPIECE 'w'     // white piece character
#define CELL_BTOWER 'B'     // black tower character
#define CELL_WTOWER 'W'     // white tower character
#define COST_PIECE 1        // one piece cost
#define COST_TOWER 3        // one tower cost
#define TREE_DEPTH 3        // minimax tree depth
#define COMP_ACTIONS 10     // number of computed actions
#define INITIAL_PIECES 12   // number of black and white pieces at start

typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

/*functions*/

void print_board(const board_t board);
int stage_0(board_t board);
void viol_checker(board_t board, char piece_clr, int from_row_num,
                  int from_col_num, int to_row_num, int to_col_num,
                  int move_no);
void stage0_move_extractor(board_t board, int move_no, int from_col_num,
                           int to_col_num, int from_row_num, int to_row_num);
void update_board(board_t board, int from_col_num, int to_col_num,
                  int from_row_num, int to_row_num);
char opp_piece_finder(char piece_clr);
char opp_tower_finder(char piece_clr);
void piece_captures(board_t board, char piece_clr, int from_col_num,
                    int to_col_num, int from_row_num, int to_row_num,
                    char opp_piece, char opp_tower);
void bpiece_captures(board_t board, int from_col_num, int to_col_num,
                     int from_row_num, int to_row_num, char opp_piece,
                     char opp_tower);
void wpiece_captures(board_t board, int from_col_num, int to_col_num,
                     int from_row_num, int to_row_num, char opp_piece,
                     char opp_tower);
void tower_captures(board_t board, char piece_clr, int from_col_num,
                    int to_col_num, int from_row_num, int to_row_num,
                    char opp_piece, char opp_tower);
void assign_towers(board_t board, char piece_clr, int to_row_num,
                   int to_col_num);
int board_cost(board_t board);
void stage1(board_t board1, int move_no);
void legal_move_finder(board_t board1, char turn, int move_no);
int* possible_moves(board_t board, char piece_clr, int curr_row, int curr_col,
                    int* possible_moves, int move_no);
int updated_board_cost(board_t board1, int from_row, int from_col, int to_row,
                       int to_col);
void update_board_display(board_t board2, int from_row_num, int from_col_num,
                          int to_row_num, int to_col_num, int move_no);
int check_upper_right(board_t board, int curr_row, int curr_col, char opp_piece,
                      char opp_tower);
int check_lower_right(board_t board, int curr_row, int curr_col, char opp_piece,
                      char opp_tower);
int check_lower_left(board_t board, int curr_row, int curr_col, char opp_piece,
                     char opp_tower);
int check_upper_left(board_t board, int curr_row, int curr_col, char opp_piece,
                     char opp_tower);

/*program starts*/

int main(int argc, char* argv[]) {
  int piece_no = 0;
  int move_no;
  int row, pair, i, j;
  char piece_clr = CELL_WPIECE;
  board_t board, board1;
  
  /* Store the initial board arrangement of a checkers game in the form of
  an 8x8 element array, simoultaneously printing its visual representation */
  printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
  printf("#BLACK PIECES: %d\n#WHITE PIECES: %d\n", INITIAL_PIECES,
         INITIAL_PIECES);
  
  for (row = 1; row <= BOARD_SIZE; row++) {
    for (pair = 0; pair < 4; pair++) {
      if (row == 4 || row == 5) {
        board[row - 1][pair * 2] = CELL_EMPTY;
        board[row - 1][(pair * 2) + 1] = CELL_EMPTY;
      } else if (row % 2 != 0) {
        board[row - 1][pair * 2] = CELL_EMPTY;
        board[row - 1][(pair * 2) + 1] = piece_clr;
      } else {
        board[row - 1][pair * 2] = piece_clr;
        board[row - 1][(pair * 2) + 1] = CELL_EMPTY;
      }
      piece_no++;
      if (piece_no == INITIAL_PIECES) {
        piece_clr = CELL_BPIECE;
      }
    }
  }
  print_board(board);
  
  /* Read in each move from the text file and update the board accordingly,
  printing the setup of the board after each move */
  move_no = stage_0(board);
  
  /* Copy the resultant board setup after computing the given moves, into a
  new 8X8 element array */
  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      board1[i][j] = board[i][j];
    }
  }
  
  /* Compute and print the next action of the player whose turn it is
  currently */
  stage1(board1, move_no);
  return EXIT_SUCCESS;  // exit program with the success code
}
/******************************************************************************/

/* Prints the current set up of the board */
void print_board(const board_t board) {
  int row, pair;
  printf("     A   B   C   D   E   F   G   H\n");
  printf("   +---+---+---+---+---+---+---+---+\n");
  for (row = 1; row <= BOARD_SIZE; row++) {
    printf("%2d |", row);
    for (pair = 0; pair < 4; pair++) {
      if (row == 4 || row == 5) {
        printf("%2c |", board[row - 1][pair * 2]);
        printf("%2c |", board[row - 1][(pair * 2) + 1]);
      } else if (row % 2 != 0) {
        printf("%2c |", board[row - 1][pair * 2]);
        printf("%2c |", board[row - 1][(pair * 2) + 1]);
      } else {
        printf("%2c |", board[row - 1][pair * 2]);
        printf("%2c |", board[row - 1][(pair * 2) + 1]);
      }
    }
    printf("\n   +---+---+---+---+---+---+---+---+\n");
  }
}
/******************************************************************************/

/* Read in each move from input and update the board accordingly,
printing the setup of the board after each move */

int stage_0(board_t board) {
  int move_no = 1;
  int from_row, to_row, from_col_num, to_col_num, from_row_num, to_row_num;
  char piece_clr, from_col, to_col;
  /* Read in each move from input */
  while (scanf(" %c%d-%c%d", &from_col, &from_row, &to_col, &to_row) != EOF) {
    if (from_row == 0) {
      /* If A is at the end of input, compute next action */
      if (from_col == 'A') {
        return move_no;
        /* If P is at the end of input, exit the program */
      } else if (from_col == 'P') {
        exit(0);
      }
    }
    /* Calculate the row number and coloumn number of the origin cell and
    destination cell of each move */
    from_col_num = from_col - 65;
    to_col_num = to_col - 65;
    from_row_num = from_row - 1;
    to_row_num = to_row - 1;
    
    /* Find the type of piece that occupies the origin cell */
    piece_clr = board[from_row_num][from_col_num];
    
    /* Check for any violations in the current move */
    viol_checker(board, piece_clr, from_row_num, from_col_num, to_row_num,
                 to_col_num, move_no);
    
    /* Check for violations in the move and print details of move */
    stage0_move_extractor(board, move_no, from_col_num, to_col_num,
                          from_row_num, to_row_num);
    
    /* Update the board with the current move */
    update_board(board, from_col_num, to_col_num, from_row_num, to_row_num);
    
    /* Print the current cost of the board */
    printf("BOARD COST: %d\n", board_cost(board));
    
    /* Print the resulting set up of the board after move is implemented */
    print_board(board);
    
    /* Increment move number and reset other parameters */
    move_no++;
    from_row = to_col = to_row = from_col = 0;
  }
  exit(0);
}
/******************************************************************************/
/* Check for any violations in the current move and report error details */
void viol_checker(board_t board, char piece_clr, int from_row_num,
                  int from_col_num, int to_row_num, int to_col_num,
                  int move_no) {
  char opp_piece, opp_tower;
  if (from_row_num > 7 || from_col_num > 7) {
    printf("ERROR: Source cell is outside of the board.\n");
    exit(0);
  } else if (to_row_num > 7 || to_col_num > 7) {
    printf("ERROR: Target cell is outside of the board.\n");
    exit(0);
  } else if (to_row_num == from_row_num || to_col_num == from_col_num) {
    printf("ERROR: Illegal action.\n");
    exit(0);
  } else if (piece_clr == CELL_EMPTY) {
    printf("ERROR: Source cell is empty.\n");
    exit(0);
  } else if (board[to_row_num][to_col_num] != CELL_EMPTY) {
    printf("ERROR: Target cell is not empty.\n");
    exit(0);
  } else if (piece_clr == CELL_WPIECE || piece_clr == CELL_WTOWER) {
    if (move_no % 2 != 0) {
      printf("ERROR: Source cell holds opponent's piece/tower.\n");
      exit(0);
    } else if (piece_clr == CELL_WPIECE && to_row_num < from_row_num) {
      printf("ERROR: Illegal action.\n");
      exit(0);
    }
  } else if (piece_clr == CELL_BPIECE || piece_clr == CELL_BTOWER) {
    if (move_no % 2 == 0) {
      printf("ERROR: Source cell holds opponent's piece/tower.\n");
      exit(0);
    } else if (piece_clr == CELL_BPIECE && to_row_num > from_row_num) {
      printf("ERROR: Illegal action.\n");
      printf("check1");
      exit(0);
    }
  } else {
    opp_piece = opp_piece_finder(piece_clr);
    opp_tower = opp_tower_finder(piece_clr);
    if (piece_clr == CELL_WPIECE || piece_clr == CELL_WTOWER ||
        piece_clr == CELL_BTOWER) {
      if ((to_col_num == from_col_num + 2) &&
          ((board[from_row_num + 1][from_col_num + 1] != opp_piece) &&
           board[from_row_num + 1][from_col_num + 1] != opp_tower)) {
        printf("ERROR: Illegal action.\n");
        exit(0);
      } else if ((to_col_num == from_col_num - 2) &&
                 ((board[from_row_num + 1][from_col_num - 1] != opp_piece) &&
                  board[from_row_num + 1][from_col_num - 1] != opp_tower)) {
        printf("ERROR: Illegal action.\n");
        exit(0);
      }
    } else if (piece_clr == CELL_BPIECE || piece_clr == CELL_WTOWER ||
               piece_clr == CELL_BTOWER) {
      if ((to_col_num == from_col_num + 2) &&
          ((board[from_row_num - 1][from_col_num + 1] != opp_piece) &&
           board[from_row_num - 1][from_col_num + 1] != opp_tower)) {
        printf("ERROR: Illegal action.\n");
        printf("check2");
        exit(0);
      } else if ((to_col_num == from_col_num - 2) &&
                 ((board[from_row_num - 1][from_col_num - 1] != opp_piece) &&
                  board[from_row_num - 1][from_col_num - 1] != opp_tower)) {
        printf("ERROR: Illegal action.\n");
        printf("check3");
        exit(0);
      }
    }
  }
}
/******************************************************************************/
/* Check for violations in current move and print details of move */
void stage0_move_extractor(board_t board, int move_no, int from_col_num,
                           int to_col_num, int from_row_num, int to_row_num) {
  char from_col, to_col;
  int from_row, to_row;
  
  /* Convert the row number and coloumn number of the origin cell and
  destination cell into alphanumeric characters for printing */
  from_col = from_col_num + 65;
  to_col = to_col_num + 65;
  from_row = from_row_num + 1;
  to_row = to_row_num + 1;
  
  /* Print details of move and current player with both players playing
  alternatively based on the current move number */
  if (move_no % 2 != 0) {
    printf("=====================================\n");
    printf(
        "BLACK ACTION #%d: %c%d-%c%d\
n",
        move_no, from_col, from_row, to_col, to_row);
  } else {
    printf("=====================================\n");
    printf(
        "WHITE ACTION #%d: %c%d-%c%d\
n",
        move_no, from_col, from_row, to_col, to_row);
  }
}
/******************************************************************************/
/* Given details of a move, updates the current board setup */
void update_board(board_t board, int from_col_num, int to_col_num,
                  int from_row_num, int to_row_num) {
  char opp_piece, opp_tower, piece_clr;
  
  /* Find the type of piece that is to be moved */
  piece_clr = board[from_row_num][from_col_num];
  
  /* Update changes in board */
  board[from_row_num][from_col_num] = CELL_EMPTY;
  board[to_row_num][to_col_num] = piece_clr;
  
  /* Identify type of pieces/towers of the opponent */
  opp_piece = opp_piece_finder(piece_clr);
  opp_tower = opp_tower_finder(piece_clr);
  
  /* Check if the move involves the capture of any opponent piece/tower and
  update the board */
  piece_captures(board, piece_clr, from_col_num, to_col_num, from_row_num,
                 to_row_num, opp_piece, opp_tower);
  tower_captures(board, piece_clr, from_col_num, to_col_num, from_row_num,
                 to_row_num, opp_piece, opp_tower);
  
  /* Convert piece to tower if it reaches the furthest row */
  assign_towers(board, piece_clr, to_row_num, to_col_num);
}
/******************************************************************************/
/* Identify type of pieces of the opponent */
char opp_piece_finder(char piece_clr) {
  char opp_piece;
 
  /* If current piece is black, then the opponent piece is a white piece and
  vice-versa */
  if (piece_clr == CELL_BTOWER || piece_clr == CELL_BPIECE) {
    opp_piece = CELL_WPIECE;
  } else if (piece_clr == CELL_WTOWER || piece_clr == CELL_WPIECE) {
    opp_piece = CELL_BPIECE;
  }
  return opp_piece;
}
/******************************************************************************/
/* Identify type of towers of the opponent */
char opp_tower_finder(char piece_clr) {
  char opp_tower;
  
  /* If current piece is black, then the opponent tower is a white tower and
  vice-versa */
  if (piece_clr == CELL_BTOWER || piece_clr == CELL_BPIECE) {
    opp_tower = CELL_WTOWER;
  } else if (piece_clr == CELL_WTOWER || piece_clr == CELL_WPIECE) {
    opp_tower = CELL_BTOWER;
  }
  return opp_tower;
}
/******************************************************************************/
/* Check if the move involves the capture of any opponent piece/tower
and update the board */
void piece_captures(board_t board, char piece_clr, int from_col_num,
                    int to_col_num, int from_row_num, int to_row_num,
                    char opp_piece, char opp_tower) {
  
  /* If the piece is black, checks for any captures of white pieces/towers */
  if (piece_clr == CELL_BPIECE) {
    bpiece_captures(board, from_col_num, to_col_num, from_row_num, to_row_num,
                    opp_piece, opp_tower);
    
    /* If the piece is white, checks for any captures of black pieces/towers */
  } else if (piece_clr == CELL_WPIECE) {
    wpiece_captures(board, from_col_num, to_col_num, from_row_num, to_row_num,
                    opp_piece, opp_tower);
  }
}
/******************************************************************************/
/* If the piece is black, checks for any potential captures of white
pieces/towers in its path to destination cell*/
void bpiece_captures(board_t board, int from_col_num, int to_col_num,
                     int from_row_num, int to_row_num, char opp_piece,
                     char opp_tower) {
  int y, x;
 
  /* Checks for an opponent piece/tower in the north-east direction*/
  if (to_col_num > from_col_num) {
    y = from_row_num - 1;
    for (x = from_col_num + 1; x < to_col_num; x++) {
      if (board[y][x] == opp_piece || board[y][x] == opp_tower) {
        board[y][x] = CELL_EMPTY;
      }
      y--;
    }
    
    /* Checks for an opponent piece/tower in the north-west direction*/
  } else if (to_col_num < from_col_num) {
    y = from_row_num - 1;
    for (x = from_col_num - 1; x > to_col_num; x--) {
      if (board[y][x] == opp_piece || board[y][x] == opp_tower) {
        board[y][x] = CELL_EMPTY;
      }
      y--;
    }
  }
}
/******************************************************************************/
/* If the piece is white, checks for any potential captures of white
pieces/towers in its path to destination cell*/
void wpiece_captures(board_t board, int from_col_num, int to_col_num,
                     int from_row_num, int to_row_num, char opp_piece,
                     char opp_tower) {
  int j, i;
 
  /* Checks for an opponent piece/tower in the south-west direction*/
  if (to_col_num > from_col_num) {
    j = from_row_num + 1;
    for (i = from_col_num + 1; i < to_col_num; i++) {
      if (board[j][i] == opp_piece || board[j][i] == opp_tower) {
        board[j][i] = CELL_EMPTY;
      }
      j++;
    }
    
    /* Checks for an opponent piece/tower in the south-east direction*/
  } else if (to_col_num < from_col_num) {
    j = from_row_num + 1;
    for (i = from_col_num - 1; i > to_col_num; i--) {
      if (board[j][i] == opp_piece || board[j][i] == opp_tower) {
        board[j][i] = CELL_EMPTY;
      }
      j++;
    }
  }
}
/******************************************************************************/
/* Check if the move involves the capture of any opponent piece/tower by a tower
and update the board */
void tower_captures(board_t board, char piece_clr, int from_col_num,
                    int to_col_num, int from_row_num, int to_row_num,
                    char opp_piece, char opp_tower) {
  
  /* Checks for an opponent piece/tower in all diagonal directions*/
  if (piece_clr == CELL_BTOWER || piece_clr == CELL_WTOWER) {
    bpiece_captures(board, from_col_num, to_col_num, from_row_num, to_row_num,
                    opp_piece, opp_tower);
    wpiece_captures(board, from_col_num, to_col_num, from_row_num, to_row_num,
                    opp_piece, opp_tower);
  }
}
/******************************************************************************/
/* Convert piece to tower if it reaches the furthest row */
void assign_towers(board_t board, char piece_clr, int to_row_num,
                   int to_col_num) {
  
  /* If piece is black and it reaches first row, then it turns to black
  tower */
  if (piece_clr == CELL_BPIECE) {
    if (to_row_num == 0) {
      board[to_row_num][to_col_num] = CELL_BTOWER;
    }
    
    /* If piece is white and it reaches first row, then it turns to white
    tower */
  } else if (piece_clr == CELL_WPIECE) {
    if (to_row_num == 7) {
      board[to_row_num][to_col_num] = CELL_WTOWER;
    }
  }
}
/******************************************************************************/
/* Calculate board cost of the current board */
int board_cost(board_t board) {
  int i, j, board_cost;
  int BPIECES = 0, WPIECES = 0, BTOWERS = 0, WTOWERS = 0;
  
  /* Search through the board and count number of pieces of each type */
  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == CELL_BPIECE) {
        BPIECES++;
      } else if (board[i][j] == CELL_WPIECE) {
        WPIECES++;
      } else if (board[i][j] == CELL_BTOWER) {
        BTOWERS++;
      } else if (board[i][j] == CELL_WTOWER) {
        WTOWERS++;
      }
    }
  }
 
  /* Calculate board cost using formula */
  board_cost =
      BPIECES + (COST_TOWER * BTOWERS) - WPIECES - (COST_TOWER * WTOWERS);
  return board_cost;
}
/******************************************************************************/
/* Compute and print the next action of the player whose turn it is currently */
void stage1(board_t board1, int move_no) {
  char turn;
  
  /* If it is an odd move number, it is BLACK's turn */
  if (move_no % 2 != 0) {
    turn = 'B';
    
    /*Find all the legal moves of each remaining black piece on the board*/
    legal_move_finder(board1, turn, move_no);
    
    /* If it is an even move number, it is WHITE's turn */
  } else {
    turn = 'W';
    
    /*Find all the legal moves of each remaining white piece on the board*/
    legal_move_finder(board1, turn, move_no);
  }
}
/******************************************************************************/
/* Find all legal moves of each remaining piece of player on the board */
void legal_move_finder(board_t boardx, char turn, int move_no) {
  int legal_row_num, legal_col_num;
  int zerocount, i, j, x;
  int from_row, from_col, to_row, to_col;
  int min_from_row, min_from_col, min_to_row, min_to_col, max_from_row,
      max_from_col, max_to_row, max_to_col;
  char piece_clr, same_piece, same_tower;
  int possible_move_arr[4];
  int legal_move_arr[48][3];
  int legal_pieces = 0, legal_moves = 0;
  int min, max, base_cost;
  
  /* Find pieces/towers of the same colour as the current player remaining on
  the board */
  if (turn == 'W') {
    same_piece = CELL_WPIECE;
    same_tower = CELL_WTOWER;
  } else {
    same_piece = CELL_BPIECE;
    same_tower = CELL_BTOWER;
  }
  
  /* Search through the entire board */
  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      piece_clr = boardx[i][j];
      
      /* If piece/tower is of the same colour as current player */
      if (piece_clr == same_piece || piece_clr == same_tower) {
        
        /* Find possible moves of that piece on the board */
        int* p_moves =
            possible_moves(boardx, piece_clr, i, j, possible_move_arr, move_no);
        zerocount = 0;
       
        /* Go through each of the possible moves to determine if
        they are valid */
        for (x = 0; x < 4; x++) {
          
          /* If move is invalid */
          if (p_moves[x] == 0) {
            zerocount++; /* If it is an even move number, it is WHITE's turn */
          } else {
            turn = 'W';
            
            /*Find all the legal moves of each remaining white piece on the
             * board*/
            legal_move_finder(board1, turn, move_no);
          }
        }
        /******************************************************************************/
       
        /* Find all legal moves of each remaining piece of player on the board
         */
        void legal_move_finder(board_t boardx, char turn, int move_no) {
          int legal_row_num, legal_col_num;
          int zerocount, i, j, x;
          int from_row, from_col, to_row, to_col;
          int min_from_row, min_from_col, min_to_row, min_to_col, max_from_row,
              max_from_col, max_to_row, max_to_col;
          char piece_clr, same_piece, same_tower;
          int possible_move_arr[4];
          int legal_move_arr[48][3];
          int legal_pieces = 0, legal_moves = 0;
          int min, max, base_cost;
          
          /* Find pieces/towers of the same colour as the current player
          remaining on the board */
          if (turn == 'W') {
            same_piece = CELL_WPIECE;
            same_tower = CELL_WTOWER;
          } else {
            same_piece = CELL_BPIECE;
            same_tower = CELL_BTOWER;
          }
          
          /* Search through the entire board */
          for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
              piece_clr = boardx[i][j];
              /* If piece/tower is of the same colour as current player */
              if (piece_clr == same_piece || piece_clr == same_tower) {
                /* Find possible moves of that piece on the board */
                int* p_moves = possible_moves(boardx, piece_clr, i, j,
                                              possible_move_arr, move_no);
                zerocount = 0;
                
                /* Go through each of the possible moves to determine if
                they are valid */
                for (x = 0; x < 4; x++) {
                  
                  /* If move is invalid */
                  if (p_moves[x] == 0) {
                    zerocount++;
                    
                    /* If move is valid */
                  } else {
                    
                    /* Assign the destination cell of the valid
                    move */
                    legal_row_num = (p_moves[x] / 10) - 1;
                    legal_col_num = (p_moves[x] % 10) - 1;
                    
                    /* Add it to the array containing all valid
                    moves */
                    legal_move_arr[legal_moves][0] = (i * 10) + j;
                    legal_move_arr[legal_moves][1] =
                        (legal_row_num * 10) + legal_col_num;
                    legal_move_arr[legal_moves][2] = board_cost(boardx);
                    
                    /* Count the total number of valid moves for
                    the player's turn */
                    legal_moves++;
                  }
                }
                if (zerocount == 4) {
                  zerocount = 0;
                } else {
                  
                  /* Count the total number of player's pieces that
                  have valid moves */
                  legal_pieces++;
                }
              }
            }
          }
          /* If there are no valid moves left for any of the player's pieces,
          opponent wins */
          if (legal_moves == 0) {
            if (turn == 'W') {
              printf("BLACK WIN!");
              exit(0);
            } else {
              printf("WHITE WIN!");
              exit(0);
            }
          }
          /* Calculate the move with the suitable board cost (maximum for BLACK)
          and (minimum for WHITE) and declare that as the next action */
          base_cost = board_cost(boardx);
          min = max = base_cost;
          
          /* update the board cost of the array of all legal moves */
          for (i = 0; i < legal_moves; i++) {
            
            /* Converting cell addresses into row number and column numbers */
            from_row = (legal_move_arr[i][0]) / 10;
            from_col = (legal_move_arr[i][0]) % 10;
            to_row = (legal_move_arr[i][1]) / 10;
            to_col = (legal_move_arr[i][1]) % 10;
            
            /* Find the board cost of each valid move */
            legal_move_arr[i][2] =
                updated_board_cost(boardx, from_row, from_col, to_row, to_col);
          }
          
          /* Find the max and min from the array of all legal moves */
          for (i = 0; i < legal_moves; i++) {
            if (min > legal_move_arr[i][2]) {
              min = legal_move_arr[i][2];
              min_from_row = (legal_move_arr[i][0]) / 10;
              min_from_col = (legal_move_arr[i][0]) % 10;
              min_to_row = (legal_move_arr[i][1]) / 10;
              min_to_col = (legal_move_arr[i][1]) % 10;
            } else if (max < legal_move_arr[i][2]) {
              max = legal_move_arr[i][2];
              max_from_row = (legal_move_arr[i][0]) / 10;
              max_from_col = (legal_move_arr[i][0]) % 10;
              max_to_row = (legal_move_arr[i][1]) / 10;
              max_to_col = (legal_move_arr[i][1]) % 10;
            }
          }
          
          /* If it is BLACK's turn, find move with maximum board cost */
          if (turn == 'B') {
            if (max == base_cost) {
              max_from_row = from_row;
              max_from_col = from_col;
              max_to_row = to_row;
              max_to_col = to_col;
            }
            
            /* Print details of action and player along with the
            updated board after the action */
            update_board_display(boardx, max_from_row, max_from_col, max_to_row,
                                 max_to_col, move_no);
            
            /* If it is WHITE's turn, find move with minimum board cost */
          } else if (turn == 'W') {
            if (min == base_cost) {
              min_from_row = from_row;
              min_from_col = from_col;
              min_to_row = to_row;
              min_to_col = to_col;
            }
            /* Print details of action and player along with the
            updated board after the action */
update_board_display(boardx,min_from_row,min_from_col,min_to_row,to_row = (legal_move_arr[i][1])/10;
to_col = (legal_move_arr[i][1])%10;
/* Find the board cost of each valid move */
legal_move_arr[i][2]= 
updated_board_cost(boardx,from_row,from_col,to_row,
to_col);
          }
          /* Find the max and min from the array of all legal moves */
          for (i = 0; i < legal_moves; i++) {
            if (min > legal_move_arr[i][2]) {
              min = legal_move_arr[i][2];
              min_from_row = (legal_move_arr[i][0]) / 10;
              min_from_col = (legal_move_arr[i][0]) % 10;
              min_to_row = (legal_move_arr[i][1]) / 10;
              min_to_col = (legal_move_arr[i][1]) % 10;
            } else if (max < legal_move_arr[i][2]) {
              max = legal_move_arr[i][2];
              max_from_row = (legal_move_arr[i][0]) / 10;
              max_from_col = (legal_move_arr[i][0]) % 10;
              max_to_row = (legal_move_arr[i][1]) / 10;
              max_to_col = (legal_move_arr[i][1]) % 10;
            }
          }
          /* If it is BLACK's turn, find move with maximum board cost */
          if (turn == 'B') {
            if (max == base_cost) {
              max_from_row = from_row;
              max_from_col = from_col;
              max_to_row = to_row;
              max_to_col = to_col;
            }
            /* Print details of action and player along with the
            updated
            board after the action */
            update_board_display(boardx, max_from_row, max_from_col, max_to_row,
                                 max_to_col, move_no);
            /* If it is WHITE's turn, find move with minimum board cost */
          } else if (turn == 'W') {
            if (min == base_cost) {
              min_from_row = from_row;
              min_from_col = from_col;
              min_to_row = to_row;
              min_to_col = to_col;
            }
            /* Print details of action and player along with the
            updated
            board after the action */
            update_board_display(boardx, min_from_row, min_from_col, min_to_row,
                                 min_to_col, move_no);
          }
        }
        /*****************************************************************************/
        /* Find all possible moves of a given piece on the board */
        int* possible_moves(board_t board, char piece_clr, int curr_row,
                            int curr_col, int* possible_move_arr, int move_no) {
          int upper_right, lower_right, lower_left, upper_left;
          char opp_piece, opp_tower;
          /* Identify type of pieces/towers of the opponent */
          opp_piece = opp_piece_finder(piece_clr);
          opp_tower = opp_tower_finder(piece_clr);
          /* Check all diagonal directions to see if a move is possible in that
          direction */
          upper_right = check_upper_right(board, curr_row, curr_col, opp_piece,
                                          opp_tower);
          lower_right = check_lower_right(board, curr_row, curr_col, opp_piece,
                                          opp_tower);
          lower_left =
              check_lower_left(board, curr_row, curr_col, opp_piece, opp_tower);
          upper_left =
              check_upper_left(board, curr_row, curr_col, opp_piece, opp_tower);
          /* If a move is possible, add the destination address of the piece to
          an array, otherwise add 0 */
          possible_move_arr[0] = upper_right;
          possible_move_arr[1] = lower_right;
          possible_move_arr[2] = lower_left;
          possible_move_arr[3] = upper_left;

          /* No possible moves for black piece in south-east and south-west
          directions */
          if (piece_clr == CELL_BPIECE) {
            possible_move_arr[1] = 0;
            possible_move_arr[2] = 0;

            /* No possible moves for white piece in north-east and north-west
            directions */
          } else if (piece_clr == CELL_WPIECE) {
            possible_move_arr[0] = 0;
            possible_move_arr[3] = 0;
          }

          return possible_move_arr;
        }
        /*****************************************************************************/
        /* Given a move, updates the board and computes the board cost */
        int updated_board_cost(board_t board1, int from_row, int from_col,
                               int to_row, int to_col) {
          board_t board2;
          int i, j, L1_board_cost;
          /* Makes a copy of the current board to implement the move */
          for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
              board2[i][j] = board1[i][j];
            }
          }
          /* Update the move changes in the board */
          update_board(board2, from_col, to_col, from_row, to_row);
          /* Calculate board cost of the updated board and print it */
          L1_board_cost = board_cost(board2);
          return L1_board_cost;
        }
        /******************************************************************************/
        /* Once next action is computed, print details of action and player
        along with the updated board after the action */
        void update_board_display(board_t board2, int from_row_num,
                                  int from_col_num, int to_row_num,
                                  int to_col_num, int move_no) {
          char from_col, to_col;
          int from_row, to_row;
          /* Convert the row number and coloumn number of the origin cell and
          destination cell into alphanumeric characters for printing */
          from_col = from_col_num + 65;
          to_col = to_col_num + 65;
          from_row = from_row_num + 1;
          to_row = to_row_num + 1;
          /* Print details of move and current player with both players playing
          alternatively based on the current move number */
          if (move_no % 2 != 0) {
            printf("=====================================\n");
            printf("*** BLACK ACTION #%d: %c%d-%c%d\n", move_no, from_col,
                   from_row, to_col, to_row);
          } else {
            printf("=====================================\n");
            printf("*** WHITE ACTION #%d: %c%d-%c%d\n", move_no, from_col,
                   from_row, to_col, to_row);
          }
          /* Update the move changes in the board */
          update_board(board2, from_col_num, to_col_num, from_row_num,
                       to_row_num);
          /* Display the board cost and print the updated board setup */
          printf("BOARD COST: %d\n", board_cost(board2));
          print_board(board2);
          exit(0);
        }
        /******************************************************************************/
        /* Check for possible moves for a piece in its north-east direction on
        the board */
        int check_upper_right(board_t board, int curr_row, int curr_col,
                              char opp_piece, char opp_tower) {
          char upper_right, upper_2right;
          /* Move is not possible if piece is in north-east corner of the board
           */
          if ((curr_row - 1) > 7 || (curr_row - 1) < 0) {
          }
          /* Update the move changes in the board */
          update_board(board2, from_col, to_col, from_row, to_row);
          /* Calculate board cost of the updated board and print it */
          L1_board_cost = board_cost(board2);
          return L1_board_cost;
        }
        /******************************************************************************/
        /* Once next action is computed, print details of action and player
        along with the updated board after the action */
        void update_board_display(board_t board2, int from_row_num,
                                  int from_col_num, int to_row_num,
                                  int to_col_num, int move_no) {
          char from_col, to_col;
          int from_row, to_row;
          /* Convert the row number and coloumn number of the origin cell and
          destination cell into alphanumeric characters for printing */
          from_col = from_col_num + 65;
          to_col = to_col_num + 65;
          from_row = from_row_num + 1;
          to_row = to_row_num + 1;
          /* Print details of move and current player with both players playing
          alternatively based on the current move number */
          if (move_no % 2 != 0) {
            printf("=====================================\n");
            printf("*** BLACK ACTION #%d: %c%d-%c%d\n", move_no, from_col,
                   from_row, to_col, to_row);
          } else {
            printf("=====================================\n");
            printf("*** WHITE ACTION #%d: %c%d-%c%d\n", move_no, from_col,
                   from_row, to_col, to_row);
          }
          /* Update the move changes in the board */
          update_board(board2, from_col_num, to_col_num, from_row_num,
                       to_row_num);
          /* Display the board cost and print the updated board setup */
          printf("BOARD COST: %d\n", board_cost(board2));
          print_board(board2);
          exit(0);
        }
        /******************************************************************************/
        /* Check for possible moves for a piece in its north-east direction on
        the board */
        int check_upper_right(board_t board, int curr_row, int curr_col,
                              char opp_piece, char opp_tower) {
          char upper_right, upper_2right;
          /* Move is not possible if piece is in north-east corner of the board
           */
          if ((curr_row - 1) > 7 || (curr_row - 1) < 0) {
            return 0;
          } else if ((curr_col + 1) > 7 || (curr_col + 1) < 0) {
            return 0;
          }
          /* Find the 2 pieces diagonally above the piece in the north-east
          direction */
          upper_right = board[curr_row - 1][curr_col + 1];
          upper_2right = board[curr_row - 2][curr_col + 2];
          /* If piece immediately diagonal in the north-east direction of the
          piece is empty, then a move is possible */
          if (upper_right == CELL_EMPTY) {
            return ((curr_row)*10) + (curr_col + 2);
            /* Check if piece immediately diagonal in the north-east direction
            of the piece is occupied by an opponent piece */
          } else if (upper_right == opp_piece || upper_right == opp_tower) {
            /* Capture cannot take place if the cell containing the opponent
            piece is in the corner of the board */
            /* So a move is not possible */
            if ((curr_row - 2) > 7 || (curr_row - 2) < 0) {
              return 0;
            } else if ((curr_col + 2) > 7 || (curr_col + 2) < 0) {
              return 0;
            }
            /* If piece diagonally above the opponent piece is empty, then
            capture is possible, so move is valid */
            if (upper_2right == CELL_EMPTY) {
              return ((curr_row - 1) * 10) + (curr_col + 3);
            }
          }
          return 0;
        }
        /******************************************************************************/
        /* Check for possible moves for a piece in its north-west direction on
        the board */
        int check_upper_left(board_t board, int curr_row, int curr_col,
                             char opp_piece, char opp_tower) {
          char upper_left, upper_2left;
          /* Move is not possible if piece is in north-west corner of the board
           */
          if ((curr_row - 1) > 7 || (curr_row - 1) < 0) {
            return 0;
          } else if ((curr_col - 1) > 7 || (curr_col - 1) < 0) {
            return 0;
          }
          /* Find the 2 pieces diagonally above the piece in the north-west
          direction */
          upper_left = board[curr_row - 1][curr_col - 1];
          upper_2left = board[curr_row - 2][curr_col - 2];
          /* If piece immediately diagonal in the north-west direction of the
          piece is empty, then a move is possible */
          if (upper_left == CELL_EMPTY) {
            return ((curr_row)*10) + (curr_col);
            /* Check if piece immediately diagonal in the north-west direction
            of the piece is occupied by an opponent piece */
          } else if (upper_left == opp_piece || upper_left == opp_tower) {
            /* Capture cannot take place if the cell containing the opponent
            piece is in the corner of the board */
            /* So a move is not possible */
            if ((curr_row - 2) > 7 || (curr_row - 2) < 0) {
              return 0;
            } else if ((curr_col - 2) > 7 || (curr_col - 2) < 0) {
              return 0;
            }
            /* If piece diagonally above the opponent piece is empty, then
            capture is possible, so move is valid */
            if (upper_2left == CELL_EMPTY) {
              return ((curr_row - 1) * 10) + (curr_col - 1);
            }
          }
          return 0;
        }
        /******************************************************************************/
        /* Check for possible moves for a piece in its south-east direction on
        the board */
        int check_lower_right(board_t board, int curr_row, int curr_col,
                              char opp_piece, char opp_tower) {
          char lower_right, lower_2right;
          /* Move is not possible if piece is in south-east corner of the board
           */
          if ((curr_row + 1) > 7 || (curr_row + 1) < 0) {
            return 0;
          } else if ((curr_col + 1) > 7 || (curr_col + 1) < 0) {
            return 0;
          }
          /* Find the 2 pieces diagonally below the piece in the south-east
          direction */
          lower_right = board[curr_row + 1][curr_col + 1];
          lower_2right = board[curr_row + 2][curr_col + 2];
          /* If piece immediately diagonal in the south-east direction of the
          piece is empty, then a move is possible */
          if (lower_right == CELL_EMPTY) {
            return ((curr_row + 2) * 10) + (curr_col + 2);
            /* Check if piece immediately diagonal in the south-east direction
            of the piece is occupied by an opponent piece */
          } else if (lower_right == opp_piece || lower_right == opp_tower) {
            /* Capture cannot take place if the cell containing the opponent
            piece is in the corner of the board */
            /* So a move is not possible */
            if ((curr_row + 2) > 7 || (curr_row + 2) < 0) {
              return 0;
            } else if ((curr_col + 2) > 7 || (curr_col + 2) < 0) {
              return 0;
            }
            /* If piece diagonally below the opponent piece is empty, then
            capture is possible, so move is valid */
            if (lower_2right == CELL_EMPTY) {
              return ((curr_row + 3) * 10) + (curr_col + 3);
            }
          }
          return 0;
        }
        /******************************************************************************/
        /* Check for possible moves for a piece in its south-west direction on
        the board */
        int check_lower_left(board_t board, int curr_row, int curr_col,
                             char opp_piece, char opp_tower) {
          char lower_left, lower_2left;
          /* Move is not possible if piece is in south-west corner of the board
           */
          if ((curr_row + 1) > 7 || (curr_row + 1) < 0) {
            return 0;
          } else if ((curr_col - 1) > 7 || (curr_col - 1) < 0) {
            return 0;
          }
          /* Find the 2 pieces diagonally below the piece in the south-west
          direction */
          lower_left = board[curr_row + 1][curr_col - 1];
          lower_2left = board[curr_row + 2][curr_col - 2];
          /* If piece immediately diagonal in the south-east direction of the
          piece is empty, then a move is possible */
          if (lower_left == CELL_EMPTY) {
            return ((curr_row + 2) * 10) + (curr_col);
            /* Check if piece immediately diagonal in the south-west direction
            of the piece is occupied by an opponent piece */
          } else if (lower_left == opp_piece || lower_left == opp_tower) {
            /* Capture cannot take place if the cell containing the opponent
            piece is in the corner of the board */
            /* So a move is not possible */
            if ((curr_row + 2) > 7 || (curr_row + 2) < 0) {
              return 0;
            } else if ((curr_col - 2) > 7 || (curr_col - 2) < 0) {
              return 0;
            }
            /* If piece diagonally below the opponent piece is empty, then
            capture is possible, so move is valid */
            if (lower_2left == CELL_EMPTY) {
              return ((curr_row + 3) * 10) + (curr_col - 1);
            }
          }
          return 0;
        }
      } else if ((curr_col + 2) > 7 || (curr_col + 2) < 0) {
        return 0;
      }
      /* If piece diagonally below the opponent piece is empty, then capture
      is possible, so move is valid */
      if (lower_2right == CELL_EMPTY) {
        return ((curr_row + 3) * 10) + (curr_col + 3);
      }
    }
    return 0;
  }
  /******************************************************************************/
  /* Check for possible moves for a piece in its south-west direction on the
  board */
  int check_lower_left(board_t board, int curr_row, int curr_col,
                       char opp_piece, char opp_tower) {
    char lower_left, lower_2left;
    /* Move is not possible if piece is in south-west corner of the board */
    if ((curr_row + 1) > 7 || (curr_row + 1) < 0) {
      return 0;
    } else if ((curr_col - 1) > 7 || (curr_col - 1) < 0) {
      return 0;
    }
    /* Find the 2 pieces diagonally below the piece in the south-west
    direction */
    lower_left = board[curr_row + 1][curr_col - 1];
    lower_2left = board[curr_row + 2][curr_col - 2];
    /* If piece immediately diagonal in the south-east direction of the piece is
    empty, then a move is possible */
    if (lower_left == CELL_EMPTY) {
      return ((curr_row + 2) * 10) + (curr_col);
      /* Check if piece immediately diagonal in the south-west direction of the
      piece is occupied by an opponent piece */
    } else if (lower_left == opp_piece || lower_left == opp_tower) {
      /* Capture cannot take place if the cell containing the opponent piece
      is in the corner of the board */
      /* So a move is not possible */
      if ((curr_row + 2) > 7 || (curr_row + 2) < 0) {
        return 0;
      } else if ((curr_col - 2) > 7 || (curr_col - 2) < 0) {
        return 0;
      }
      /* If piece diagonally below the opponent piece is empty, then capture
      is possible, so move is valid */
      if (lower_2left == CELL_EMPTY) {
        return ((curr_row + 3) * 10) + (curr_col - 1);
      }
    }
    return 0;
  }
  /******************************************************************************/
  /* Algorithms are fun!*/
  /* THE END
   * -------------------------------------------------------------------*/
