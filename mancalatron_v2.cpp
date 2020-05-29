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


  std::cout << "\n\nMove number " << game_as_is.move_number << "\n";
  std::cout << "Player: " << game_as_is.player << "\n";

  print_board(game_as_is.board);

  well = get_valid_well(game_as_is);

  struct game_state new_game_state;


  return (0);
}
