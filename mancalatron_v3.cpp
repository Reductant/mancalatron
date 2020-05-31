#include <iostream>
#include <vector>
#include "mancalatron_v3.h"


std::vector<int> predict_move_outcomes(game_state game_as_is) {

  // Initialise a post-move game_state
  struct game_state potential_game_state;

  // To hold predicted scores output by the score_predict function
  std::vector<int> predicted_score = {0, 0, 0, 0, 0, 0};

  // Get well and check that it's between 1 and 6 and nonempty
  for (int i = 0; i < 6; ++i) {
    if (game_as_is.board[i + 1] == 0) {
      predicted_score[i] = -1;    // -1 as a flag for INVALID WELL
    } else {
      // Implement the effects of this well choice
      potential_game_state = update_game_state(game_as_is, i + 1);
      predicted_score[i] = potential_game_state.board[7];
    }
  }

  return(predicted_score);
}


int main() {

  // The well input by the user
  int well;

  // Initialise game state
  struct game_state game_as_is;
  game_as_is.move_number = 0;
  game_as_is.playing = true;
  game_as_is.player = 0;
  game_as_is.board = {0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3};
  game_as_is.in_double_move = false;
  game_as_is.score = {0, 0};


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

    // Display
    print_board(game_as_is);


    // Human makes a move
    if (game_as_is.player == 0) {
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

    // AI makes a move, with 1/2-step lookahead
    {

      std::vector<int> predicted_score;

      // Initialise a post-move game_state
      struct game_state potential_game_state;


      predicted_score = predict_move_outcomes(game_as_is);

      int current_highest_score = -1;
      int current_best_well = 1;
      for(int i = 0; i < 6; ++i) {
        if (predicted_score[i] > current_highest_score) {
          current_highest_score = predicted_score[i];
          current_best_well = i + 1;
        }
      }

      potential_game_state = update_game_state(game_as_is, current_best_well);

      // The post-move game state becomes the new as_is game state
      game_as_is = potential_game_state;

      std::cout << "\n\nAI chooses well " << current_best_well << "\n\n";

    }

    // Rotate the board for the next user
    game_as_is.board = flip_board(game_as_is.board);

    // If finished, set game_as_is.playing to false and update score
    game_as_is = check_for_finish(game_as_is);

  }
  // End of main game loop

  // Display final scores
  if (game_as_is.score[0] > game_as_is.score[1]) {
    std::cout << "\n\n PLAYER 0 WINS!";
  } else if (game_as_is.score[0] < game_as_is.score[1]) {
    std::cout << "\n\n PLAYER 1 WINS!";
  } else std::cout << "\n\nIt's a draw!\n\n";

  return (0);
}
