#include <iostream>
#include <vector>
#include "mancalatron_v2.h"

int well;             // The well input by the user



int main() {


  std::cout << "\n\nMove number " << game_as_is.move_number << "\n";
  std::cout << "Player: " << game_as_is.player << "\n";

  print_board(game_as_is.board);

  well = get_valid_well(game_as_is);

  //std::cout << game_state.move_number;

  return (0);
}
