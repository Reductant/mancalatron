#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

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


  std::string to_string();
};



struct score_table {
  std::vector<int> predicted_ai_score;
  std::vector<int> best_opponent_score;
};

extern score_table scores;

/**********************************************************************
Represents the board to std::cout
Takes the game state as input
***********************************************************************/
void print_board(game_state &game_as_is);


// Ask the user for a well choice.
// Check its validity: refers to a nonempty well 1-6
int get_valid_well(game_state game_as_is);



/***********************************************************************
The main engine of the game.

Takes a game state and a well number as inputs and implements the consequences of that action.

Returns a game state with all the stones repositioned and scores calculated.
************************************************************************/
game_state update_game_state(game_state game, int well);



// Flips the board around so the same update_board function can be used whoever is playing.
std::vector<int> flip_board(std::vector<int> board);


// Add up each row of wells. If either total is zero, the game is over.
game_state check_for_finish(game_state game);


/********************************************************************
The main AI decision making component. It takes the game_as_is as
input, then initialises a vector of six possible scores, one for each
well. It cycles through the wells, using the update_board function to
see the consequences of each move. The score here is the number of
stones in the AI player's mancala. The function returns this vector.
********************************************************************/
score_table predict_move_outcomes(game_state game_as_is);

/*********************************************************************
This function uses the output from predict_move_outcomes to decide the
best AI well. predict_move_outcomes gives a vector of AI scores for each
well and a vector of best possible opponent scores for each well in the
next move. score_differential is a subtraction of one from the other.
The AI chooses the well that gives the biggest difference.
**********************************************************************/
int ai_choose_well(game_state game_as_is);

/*******************************
The AI is ignoring move chaining. If an AI move would end in a mancala,
it is ignoring the fact that the opponent would not have a chance to retaliate.
Maybe use the in_double_move flag?
********************************/
