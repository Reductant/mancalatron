struct game_state {
  int move_number;            // Counts the number of (half-moves)
  bool playing;               // Is the game ongoing?
  int player;                 // Whose turn is it? 0 or 1
  std::vector<int> history;   // Record of all valid well choices. 0 indicates a move passed by a player because of a double move

  bool in_double_move;        // Is this a double move? Should the next player's move be skipped?

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

  std::vector<int> score;    // Players' scores. Index is player ID.

};



/**********************************************************************
Represents the board to std::cout
Takes the game state as input
***********************************************************************/
void print_board(game_state game_as_is) {

  // Display statistics
  std::cout << "\n\nMove number:\t" << game_as_is.move_number << "\n";
  std::cout << "Player:\t\t" << game_as_is.player << "\n";
  std::cout << "History:\t";
  for (int i = 0; i < game_as_is.history.size(); ++i) {
    std::cout << game_as_is.history[i] << " ";
  }
  std::cout << "\nPlayer 0 score:\t" << game_as_is.score[0] <<
    "\nPlayer 1 score:\t" << game_as_is.score[1];


  std::vector<int> board = game_as_is.board;

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

    //return(0);
}




// Ask the user for a well choice.
// Check its validity: refers to a nonempty well 1-6
int get_valid_well(game_state game_as_is) {

  int well;     // User selection of well (should be 1-6)

  bool valid_well = false;

  while (valid_well == false) {
    std::cout << "\n\n";
    std::cout << "Enter well selection: ";
    std::cin >> well;

    if ((well >= 1) && (well <= 6) && (game_as_is.board[well] > 0)) {
      valid_well = true;
    } else {
      std::cout << "\nInvalid well choice. Ignoring.\n";
    }
  }

  return(well);
}


/***********************************************************************
The main engine of the game.

Takes a game state and a well number as inputs and implements the consequences of that action.

Returns a game state with all the stones repositioned and scores calculated.
************************************************************************/
game_state update_game_state(game_state game, int well) {

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

  // Handles stone capture
  if ((current_well < 7) && (current_well != 0) && (game.board[current_well] == 1) && (game.board[14 - current_well] != 0)) {
    game.board[current_well] = 0;
    game.board[7] = game.board[7] + game.board[14 - current_well] + 1;
    game.board[14 - current_well] = 0;
  }


  // If you land in a mancala, set flag for double move
  if (current_well == 7) game.in_double_move = true;

  // Set flags for next move
  game.score[game.player] = game.board[7];
  game.player = (game.player + 1) % 2;
  game.history.push_back(well);
  ++game.move_number;

  return(game);

}




// Flips the board around so the same update_board function can be used whoever is playing.
std::vector<int> flip_board(std::vector<int> board) {
  std::vector<int> new_board = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // Temporary empty board

  // Fill with old board's stones, with index rotated
  for (int i = 0; i < 14; ++i) {
    new_board[i] = board[(i + 7) % 14];
  }

  return(new_board);
}


// Add up each row of wells. If either total is zero, the game is over.
game_state check_for_finish(game_state game) {

  int bottom_row_total = 0;      // Running total of stones in wells
  for (int i = 1; i < 7; ++i) {
    bottom_row_total += game.board[i];
  }
  if (bottom_row_total == 0) game.playing = false;

  int top_row_total = 0;
  for (int i = 8; i < 14; ++i) {
    top_row_total += game.board[i];
  }
  if (top_row_total == 0) game.playing = false;

  if (game.playing == false) {
    game.score[game.player] += bottom_row_total;
    game.score[(game.player + 1) % 2] += top_row_total;
  }


  return(game);
}


/********************************************************************
The main AI decision making component. It takes the game_as_is as
input, then initialises a vector of six possible scores, one for each
well. It cycles through the wells, using the update_board function to
see the consequences of each move. The score here is the number of
stones in the AI player's mancala. The function returns this vector.
********************************************************************/
std::vector<int> predict_move_outcomes(game_state game_as_is) {

  // Initialise a post-move game_state
  struct game_state potential_game_state;

  // To hold predicted scores output by the score_predict function
  std::vector<int> predicted_score = {0, 0, 0, 0, 0, 0};

  // Get well and check that it's between 1 and 6 and nonempty
  for (int i = 0; i < 6; ++i) {

    // Awkwardly, wells are 1-indexed, predicted score is 0-indexed.
    // If the well is empty, use score = -1 as a flag for INVALID WELL
    if (game_as_is.board[i + 1] == 0) {
      predicted_score[i] = -1;
    } else {

      // If the well is valid, predict the new game state and count the stones in AI player's mancala.
      potential_game_state = update_game_state(game_as_is, i + 1);

      predicted_score[i] = potential_game_state.board[7];
    }
  }

  return(predicted_score);
}


/*********************************************************************
A simple function that accepts the predicted_score vector returned by
predict_move_outcomes, then returns the well that maximises the number
of stones in the AI player's mancala at the end of that move. Nothing
fancy.
**********************************************************************/
int ai_choose_well(game_state game_as_is) {
      std::vector<int> predicted_score;

      // Initialise a post-move game_state
      struct game_state potential_game_state;

      // Get a vector of six potential scores
      predicted_score = predict_move_outcomes(game_as_is);

      // Find the well that maximises predicted score
      int current_highest_score = -1;
      int current_best_well = 1;
      for(int i = 0; i < 6; ++i) {
        if (predicted_score[i] > current_highest_score) {
          current_highest_score = predicted_score[i];
          current_best_well = i + 1;
        }
      }

  return(current_best_well);
}
