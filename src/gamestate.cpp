#include <fmt/format.h>
#include "mancalatron_v4.h"


std::string game_state::to_string() {
  auto board_string = fmt::format("Move number:\t{}\n", move_number);
  board_string += fmt::format("Player:\t\t{}\n", player);
  board_string += fmt::format("History:\t");
  for (auto v: history)
    board_string += fmt::format("{} ", v);
  board_string += "\n";
  board_string += fmt::format("Scores:\t\tPlayer 0: {}  Player 1: {}\n", score[0], score[1]);
  board_string += "\n\n\n";
  for (int i = 13; i >= 8; --i)
    board_string += fmt::format("\t({})", board[i]);
  board_string += fmt::format("\n ({})\t\t\t\t\t\t\t({})\n", board[0], board[7]);
  board_string += "\n";
  for (int i = 1; i < 7; ++i)
    board_string += fmt::format("\t({})", board[i]);
  board_string += "\n\n\t 1\t 2\t 3\t 4\t 5\t 6";
  return board_string;
}