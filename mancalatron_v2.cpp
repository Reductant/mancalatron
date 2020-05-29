#include <iostream>
#include <vector>
#include "mancalatron_v2.h"

int well;             // The well input by the user


game_state update_game_state(game_state game, int well) {

    std::cout << "Well: " << well;

    int stones_to_distribute = game.board[well];     // Stones 'picked up' by the user

    int current_well;     // The well into which user 'drops' a stone

    game.board[well] = 0; // We've emptied the well.

    current_well = well;

    // Redistribute stones
    while (stones_to_distribute > 0) {
      ++current_well;
      if (current_well == 14) current_well = 0;

      // Ensure opponent's mancala is skipped
      if (current_well != 0) {
        ++game.board[current_well];
        --stones_to_distribute;
      }
    }

    // This needlessly repetitive block deals with capture. Landing in an empty well is understood as the current_well containing 1 stone.
    // A better programmer would put this into a swtich statement. If you find a better programmer, let me know.

    if ((current_well == 1) && (game.board[current_well] == 1) && (game.board[13] != 0)) {
      game.board[1] = 0;
      game.board[7] = game.board[7] + game.board[13] + 1;
      game.board[13] = 0;
    }

  if ((current_well == 2) && (game.board[current_well] == 1) && (game.board[12] != 0)) {
    game.board[2] = 0;
    game.board[7] = game.board[7] + game.board[12] + 1;
    game.board[12] = 0;
  }

  if ((current_well == 3) && (game.board[current_well] == 1) && (game.board[11] != 0)) {
    game.board[3] = 0;
    game.board[7] = game.board[7] + game.board[11] + 1;
    game.board[11] = 0;
  }

  if ((current_well == 4) && (game.board[current_well] == 1) && (game.board[10] != 0)) {
    game.board[4] = 0;
    game.board[7] = game.board[7] + game.board[10] + 1;
    game.board[10] = 0;
  }


  if ((current_well == 5) && (game.board[current_well] == 1) && (game.board[9] != 0)) {
    game.board[5] = 0;
    game.board[7] = game.board[7] + game.board[9] + 1;
    game.board[9] = 0;
  }

  if ((current_well == 6) && (game.board[current_well] == 1) && (game.board[8] != 0)) {
    game.board[6] = 0;
    game.board[7] = game.board[7] + game.board[8] + 1;
    game.board[8] = 0;
  }

  if (current_well == 7) {
    std::cout << "DOUBLE MOVE";
  }

  return(game);

}



int main() {

  // Initialise game state
  struct game_state game_as_is;
  game_as_is.move_number = 0;
  game_as_is.playing = true;
  game_as_is.player = 0;
  game_as_is.board = {0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3};


  std::cout << "\n\nMove number " << game_as_is.move_number << "\n";
  std::cout << "Player: " << game_as_is.player << "\n";

  print_board(game_as_is.board);

  well = get_valid_well(game_as_is);

  struct game_state new_game_state;

  new_game_state = update_game_state(game_as_is, well);

  print_board(new_game_state.board);


  return (0);
}
