#include <iostream>
#include <vector>
#include "mancalatron.h"

int well;             // The well input by the user, a variable in update_board
int move_number = 0;  // Counts the number of (half-)moves
bool playing = true;  // Is game ongoing? Quit condition for main loop.
int player = 0;       // Whose turn is it? 0 or 1
int move_complete;    // Is the move finished? 0 means a double move.

std::vector<int> history;   // Log of all valid moves. A zero indicates a move passed because of a double move

std::vector<int> board = {0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3};
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




int main() {

  while (playing) {
    player = move_number % 2;

    std::cout << "\n\nMove number " << move_number << "\n";
    std::cout << "Player: " << player << "\n";

    print_board(board);

    std::cout << "\n\n\n";
    std::cout << "Enter well selection: ";
    std::cin >> well;


      // Test for valid well choice, then update board
      if ((well >= 1) && (well <= 6) && board[well] > 0) {


        // Add this move to the move history
        history.push_back(well);

        // Print the move history for diagnosis
        std::cout << "\n\nHistory: ";
        for(int i = 0; i < history.size(); ++i) {
          std::cout << history[i] << ", ";
        }
        std::cout << "\n";

        // Make the desired move
        board = update_board(board, well);

        // Decide if the game is finished
        if (check_for_finish(board)) playing = false;


      } else {
        std::cout << "\nInvalid well choice. Ignoring.\n";
        continue;
      }


        // Put this in an if to check if the move is a double
        ++move_number;
        if (player == 0) player = 1; else player = 0;
        board = flip_board(board);

      }

  return (0);
}



/*************************************************************************
To do:
  Compute final score
  Vectors a massive improvement, but need to re-establish double-moves
**************************************************************************/
