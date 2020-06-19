Mancalatron, written in bad C++ Adam Wilson, during 2020 COVID lockdown

Mancalatron is a program that plays the board game mancala (https://en.wikipedia.org/wiki/Mancala). Its AI is fairly simplistic. The AI assesses the outcome of a move from each well; then, for each potential well choice, it assesses the consequences of its opponent's retaliatory move. It assumes the opponent will always make the move that increases the number of stones in the opponent's mancala, given the AI's move. Based on this, it selects the well that maximises the difference in points in its favour for that move pair.

A struct game_state is defined, and game_as_is is an instance of this struct that describes the game at the instant of play. The initial declaration of game_as_is contains a vector ai_player. Right now this needs to be edited to decide which player is human (ai_player[n] = 0) and which is AI (ai_player[n] = 1).

Music listened to while programming: https://www.youtube.com/watch?v=GYyqpxnmIcE
