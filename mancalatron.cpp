#include <iostream>

int well;             // The well input by the user, a variable in update_board
int move_number = 0;  // Counts the number of (half-)moves
bool playing = true;  // Is game ongoing? Quit condition for main loop.
int player;           // Whose turn is it? 0 or 1


int board[] = {0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3};
/********************************************************************
This is the board from the POV of the player playing right now: player_zero.
Each int represents the contents of a well.
board[0] is player_zero's mancala. board[7] is the opponent's mancala.
player_zero has access to board[1-6].
To allow easy redistribution, wells are numbered clockwise as follows:

                13  12  11  10  9   8
            0                           7
                1   2   3   4   5   6

***********************************************************************/



/**********************************************************************
Represents the board to std::cout
Takes the board array as input
***********************************************************************/
int print_board(int board[]) {

  std::cout << "\n\n\n\t("
    << board[13] << ")\t("
    << board[12] << ")\t("
    << board[11] << ")\t("
    << board[10] << ")\t("
    << board[9] << ")\t("
    << board[8] << ")\t"
    << "\n (" << board[0] << ")\t\t\t\t\t\t\t(" << board[7] << ")\n\t("
    << board[1] << ")\t("
    << board[2] << ")\t("
    << board[3] << ")\t("
    << board[4] << ")\t("
    << board[5] << ")\t("
    << board[6] << ")\t";

  // Well numbering for the user
  std::cout << "\n\n\t 1\t 2\t 3\t 4\t 5\t 6";

    return(0);
}


/**********************************************************************
This handles board updating (from player_zero perspective only).
The player 'picks' up the stones in well, then redistributes them.
Opponent's mancala is skipped.
***********************************************************************/
int update_board(int board[], int well) {

  int stones_to_distribute = board[well];     // Stones 'picked up' by the user
  int current_well;     // The well into which user 'drops' a stone

  board[well] = 0;

  current_well = well;

  // Redistribute stones
  while (stones_to_distribute > 0) {
    ++current_well;
    if (current_well == 14) current_well = 0;

    // Ensure opponent's mancala is skipped
    if (current_well != 0) {
      ++board[current_well];
      --stones_to_distribute;
    }
  }

  if ((current_well == 6) && (board[current_well] == 1)) {
    board[6] = 0;
    board[7] = board[7] + board[8] + 1;
    board[8] = 0;
  }


  return(0);
}


int check_for_finish(int board[]) {

    int total = 0;        // Counts the stones in player_zero's wells

    // Add up player_zero's stones
    for (int i = 1; i < 7; ++i) {
      total += board[i];
    }

    // If there are no stones in the player's wells, add the rest to opponent's mancala.
    if (total == 0) {

      for (int i = 8; i < 14; ++i) {
        board[0] += board[i];
        board[i] = 0;
      }

      print_board(board);

      std::cout << "\n\nGAME OVER!\n";

      playing = false;

    }


  return (0);
}


int main() {

  // Main game loop
  while (playing) {

    std::cout << "Move number " << move_number;

    print_board(board);

    std::cout << "\n\n\n";
    std::cout << "Enter well selection: ";
    std::cin >> well;

    // Test for valid well choice, then update board
    if ((well >= 1) && (well <= 6) && board[well] > 0) {

      update_board(board, well);
      check_for_finish(board);
      ++move_number;

    } else {
      std::cout << "\nInvalid well choice. Ignoring.\n";
    }


  }


  return (0);
}


/*************************************************************************
To do:
  Change update_board to include capture
  Function for flipping the board
**************************************************************************/
