#include <iostream>
#include <vector>
#include "mancalatron_v2.h"

int well;             // The well input by the user



int main() {

  // Initialise game state
  struct game_state game_as_is;
  game_as_is.move_number = 0;
  game_as_is.playing = true;
  game_as_is.player = 0;
  game_as_is.board = {0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3};
  game_as_is.in_double_move = false;

  while (game_as_is.playing) {


    // If in a double move, player skips a turn.
    if (game_as_is.in_double_move == true) {
      ++game_as_is.move_number;
      game_as_is.history.push_back(0);
      game_as_is.in_double_move = false;
      game_as_is.player = (game_as_is.player + 1) % 2;
      game_as_is.board = flip_board(game_as_is.board);
      continue;

    } else {      // Otherwise, proceed with move.

      // Display statistics
      std::cout << "\n\nMove number " << game_as_is.move_number << "\n";
      std::cout << "Player: " << game_as_is.player << "\n";
      std::cout << "History: ";
      for (int i = 0; i < game_as_is.history.size(); ++i) {
        std::cout << game_as_is.history[i] << " ";
      }

      print_board(game_as_is.board);

      well = get_valid_well(game_as_is);
    }


    // Initialise a post-move game_state
    struct game_state new_game_state;

    // Implement the effects of this well choice
    new_game_state = update_game_state(game_as_is, well);

    // The post-move game state becomes the new as_is game state
    game_as_is = new_game_state;

    // Check for finish

    // Flip board
    game_as_is.board = flip_board(game_as_is.board);
  }






  return (0);
}
