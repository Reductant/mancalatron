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

  std::vector<int> score;              // Players' scores. Index is player ID.
  std::vector<int> ai_player;          // Index is player ID, value is AI=true

};



struct score_table {
  std::vector<int> predicted_ai_score;
  std::vector<int> best_opponent_score;
} scores;


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
score_table predict_move_outcomes(game_state game_as_is) {

  // Initialise a potential post-move game_state
  struct game_state ai_move;

  // Initialise a potential post-move game_state for the opponent's move
  struct game_state opponent_move;

  // To hold predicted scores output by the score_predict function
  std::vector<int> predicted_ai_score = {0, 0, 0, 0, 0, 0};

  std::vector<int> predicted_opponent_score = {0, 0, 0, 0, 0, 0};

  std::vector<int> best_opponent_score = {0, 0, 0, 0, 0, 0};

  // Get well and check that it's between 1 and 6 and nonempty
  for (int i = 0; i < 6; ++i) {

    // Awkwardly, wells are 1-indexed, predicted score is 0-indexed.
    // If the well is empty, use score = -1 as a flag for INVALID WELL
    if (game_as_is.board[i + 1] == 0) {
      predicted_ai_score[i] = -1;
    } else {

      // If the well is valid, predict the new game state and count the stones in AI player's mancala.
      ai_move = update_game_state(game_as_is, i + 1);

      //std::cout << "\n\n AI CONTEMPLATES MOVING FROM WELL " << i + 1 << "\n";

      std::vector<int> predicted_opponent_score = {0, 0, 0, 0, 0, 0};

      for (int j = 0; j < 6; ++j) {

      // The AI has moved; now cycle through opponent moves
      opponent_move = ai_move;
      opponent_move.board = flip_board(opponent_move.board);

        //std::cout<< "\n";
        if (opponent_move.board[j + 1] == 0) {
          predicted_opponent_score[j] = -1;
        } else {
          opponent_move = update_game_state(opponent_move, j + 1);
          predicted_opponent_score[j] = opponent_move.board[7];
          //std::cout << j + 1 << " " << predicted_opponent_score[j] << "\n";
        }

      }

      //for (int score = 0; score < 6; ++score) std::cout << predicted_opponent_score[score];
      int opponent_highest_score = *std::max_element(predicted_opponent_score.begin(), predicted_opponent_score.end());

      //std::cout << "IF AI MOVES AT WELL " << i << " OPPONENT BEST SCORE IS " << opponent_highest_score;



/**************************************************
 We've worked out the opponent highest score. Now make this function return a struct containing:

a vector giving the predicted AI score for each well
a vector containing the opponent highest score a subsequent move at that well

This vector goes back to ai_choose_well
  which then makes a decision based on maybe a subtraction of each vector

And for God's sake, tidy up this function, it's a mess!
At the very least, run some experiments to find a way of returning the highest value from a vector and get rid of at least one of these for loops.

***********************************************/


      // Insert the predicted mancala value into predicted_score
      predicted_ai_score[i] = ai_move.board[7];

      best_opponent_score[i] = opponent_highest_score;


    }
  }


  struct score_table scores;

  scores.predicted_ai_score = predicted_ai_score;
  scores.best_opponent_score = best_opponent_score;

  //std::cout << "\nOPPONENT SCORE PREDICTIONS: ";
  //for (int score = 0; score < 6; ++score) std::cout << scores.best_opponent_score[score] << " ";


  return(scores);
}


/*********************************************************************
A simple function that accepts the predicted_score vector returned by
predict_move_outcomes, then returns the well that maximises the number
of stones in the AI player's mancala at the end of that move. Nothing
fancy.
**********************************************************************/
int ai_choose_well(game_state game_as_is) {
      std::vector<int> ai_scores;
      std::vector<int> opponent_scores;

      std::vector<int> score_differential = {0, 0, 0, 0, 0, 0};

      struct score_table predicted_scores;

      // Initialise a post-move game_state
      struct game_state potential_game_state;

      // Get a score_table struct of scores
      predicted_scores = predict_move_outcomes(game_as_is);

      ai_scores = predicted_scores.predicted_ai_score;
      opponent_scores = predicted_scores.best_opponent_score;

      // Subtract opponent_scores from ai_scores
      for (int i = 0; i < 6; ++i) {
        score_differential[i] = ai_scores[i] - opponent_scores[i];
      }

      // Find the well that maximises predicted score
      int current_highest_score = -1;
      int current_best_well = 1;
      for(int i = 0; i < 6; ++i) {
        if (score_differential[i] > current_highest_score) {
          current_highest_score = score_differential[i];
          current_best_well = i + 1;
        }
      }

  return(current_best_well);
}
