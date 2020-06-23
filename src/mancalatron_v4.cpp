#include <iostream>
#include <vector>
#include <algorithm>
#include "mancalatron_v4.h"



int main() {

  // The well input by the user
  int well;

  // The well chosen by the AI
  int ai_well_choice;

  // Initialise game state
  struct game_state game_as_is;
  game_as_is.move_number = 0;
  game_as_is.playing = true;
  game_as_is.player = 0;
  game_as_is.board = {0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3};
  game_as_is.in_double_move = false;
  game_as_is.score = {0, 0};
  game_as_is.ai_player = {0, 0}; // Index is player ID, value is AI=true


  // Initialised to an invalid number as while condition
  int ai_player_choice = 2;

  // Ask user which player is AI
  while ((ai_player_choice != 0) && (ai_player_choice != 1)) {
    std::cout << "Which player is AI (0/1)? ";
    std::cin >> ai_player_choice;

    if ((ai_player_choice != 0) && (ai_player_choice != 1)) {
      std::cout << "\nInvalid player choice\n";
    }
  }
  game_as_is.ai_player[ai_player_choice] = 1;


  // Main game loop
  while (game_as_is.playing) {

    // If in a double move, player skips a turn.
    if (game_as_is.in_double_move == true) {

      // Update game state without moving
      ++game_as_is.move_number;
      game_as_is.history.push_back(0);
      game_as_is.in_double_move = false;
      game_as_is.player = (game_as_is.player + 1) % 2;
      game_as_is.board = flip_board(game_as_is.board);
      continue;
    }


    std::cout << "\n******************************************************\n";
    //print_board(game_as_is);


    // If player is human...
    if (game_as_is.ai_player[game_as_is.player] == 0) {

      print_board(game_as_is);

      // Get well and check that it's between 1 and 6 and nonempty
      well = get_valid_well(game_as_is);

      // Initialise a post-move game_state
      struct game_state new_game_state;

      // Implement the effects of this well choice
      new_game_state = update_game_state(game_as_is, well);

      // The post-move game state becomes the new as_is game state
      game_as_is = new_game_state;
    }

    else

    // AI makes a move, considering opponent retaliation
    {

      ai_well_choice = ai_choose_well(game_as_is);

      // The post-move game state becomes the new as_is game state
      game_as_is = update_game_state(game_as_is, ai_well_choice);

      std::cout << "\n\nAI chooses well " << ai_well_choice << "\n\n";

    }

    // Rotate the board for the next user
    game_as_is.board = flip_board(game_as_is.board);

    // If finished, set game_as_is.playing to false and update score
    game_as_is = check_for_finish(game_as_is);

  }
  // End of main game loop

  // Display final scores
  std::cout << "\nPlayer 0 score:\t" << game_as_is.score[0] <<
    "\nPlayer 1 score:\t" << game_as_is.score[1] << "\n\n";

  if (game_as_is.score[0] > game_as_is.score[1]) {
    std::cout << "\n\n PLAYER 0 WINS!\n\n";
  } else if (game_as_is.score[0] < game_as_is.score[1]) {
    std::cout << "\n\n PLAYER 1 WINS!\n\n";
  } else std::cout << "\n\nIt's a draw!\n\n";

  return (0);
}
