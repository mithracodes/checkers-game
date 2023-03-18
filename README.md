# checkers-game

## Overview

Checkers, or draughts, is a strategy board game played by two players. There are many variants of checkers. For a guide to checker’s families and rules, see *https://www.fmjd.org/downloads/Checkers_families_and_rules.pdf*.

## Gameplay
The program reads, prints, and plays a special variant of the checkers game. The game is played on an 8x8 chessboard with 12 black and 12 white pieces initially positioned as shown in Figure 1a.

Each player plays all pieces of the same color. Black open the game by making a move, then white makes a move, and then players alternate their turns. In a single turn, the player either makes a move or capture. For example, the arrow in Figure 1a denotes an opening move of the black piece from cell G6 to cell F5.

## Rules
A piece may move to an empty cell diagonally forward (toward the opponent; north for black and south for white) one square. When a piece reaches the furthest row (the top row for black and the bottom row for white), it becomes a tower (a pile of pieces). The only move of the white piece at cell D7 in Figure 1b promotes it to the tower. A tower may move to an empty cell diagonally, both forward and backward, one square.

To capture a piece or a tower of the opponent, a piece or a tower of the player jumps over it and lands in a straight diagonal line on the other side. This landing cell must be empty. When a piece or tower is captured, it is removed from the board. Only one piece or tower may be captured in a single jump, and, in this variant of the game, only one jump is allowed in a single turn. Hence, if another capture is available after the first jump, it cannot be taken in this turn. Also, in this variant of the game, if a player can make a move or a capture, they may decide which of the two to complete. A piece always jumps forward (toward the opponent), while a tower can jump forward and backward.

A player wins the game if it is the opponent’s turn and they cannot take action, move, or capture, either because no their pieces and towers are left on the board or because no legal move or capture is possible.

## Input and Output

The program reads input from stdin and writes output to stdout. The input should list actions, one per line, starting from the initial setup and a black move. The input of moves and captures can be followed by a single command character, either ‘A’ or ‘P’. Action should be specified as a pair of the source cell and the target cell, separated by the minus character ‘-’. For example, “G6-F5” specifies the move from cell G6 to cell F5.

The program should print the initial setup and all legal actions. The program should terminate immediately after printing an error message if an illegal action is encountered in the input. The conditions for the errors are self-explanatory and should be evaluated in the order the messages are listed.

## Stage 0 – Reading, Analyzing, and Printing Input Data

The program's first version should read input and print the initial setup and all legal actions. The output for the `test1.txt` input file should contain 42 lines, starting with lines 1-21, which report the board configuration and specify the initial setup from Figure 1a. `b` and `w` characters represent black and white pieces, respectively. Lines 22-42 print the first move specified in the input, and each action's output starts with the delimiter line of 37 `=` characters (line 22). The next two lines print information about the action taken and the board's cost (line 23 and 24).

![image](https://user-images.githubusercontent.com/95140934/224549195-4894ff0b-96fe-40e4-b67f-748698724b95.png)

The board's cost is computed as `b + 3B − w − 3W`, where `b`, `w`, `B`, and `W` are the number of black pieces, white pieces, black towers, and white towers on the board, respectively. Towers cost three pieces.

If the program encounters an illegal action in the input, it should select and print one of the six error messages provided below. The program should terminate immediately after printing the error message. The error messages are evaluated in the order they are listed, and only the first encountered error should be reported.

- `ERROR: Source cell is outside of the board`
* `ERROR: Target cell is outside of the board`
+ `ERROR: Source cell is empty`
- `ERROR: Target cell is not empty`
* `ERROR: Source cell holds opponent's piece/tower`
+ `ERROR: Illegal action.`

For example, if line 2 of the `test1.txt` file is updated to state "`G2-A8`," line 43 of the corresponding output should report: "`ERROR: Target cell is not empty.`" The complete output generated in Stage 0 for the `test1.txt` input file is provided in the `test1-out.txt` output file.

## Stage 1: Compute and Print Next Action
If the input command is `A`, the program should compute and print information about the next action of the player with the turn using the minimax decision rule for a tree depth of three.

The first step is to construct the tree of all reachable board configurations starting from the current configuration and of the requested depth, replicating nodes for multiple occurrences of the same board.

![image](https://user-images.githubusercontent.com/95140934/224549220-0713b74d-b9ef-41e9-8d64-ab5eb9c0912b.png)

Next, the cost of all leaf boards is computed, where a positive cost indicates a win for black, a negative cost indicates a win for white, and the magnitude of the cost indicates the advantage of one player over the other.

Finally, for each possible action of the player, the program checks all possible actions of the opponent and chooses the next action of the player to be the first action on the path from the root of the tree towards a leaf node for which the player maximizes their gain while the opponent aims to minimize the player's gain. The computed action is printed with the `***` marker.

If the next action cannot be computed, the program prints a message indicating the winner. Black and white towers are denoted by `B` and `W` characters, respectively, and boards in which black or white cannot take an action cost `INT MIN` and `INT MAX`, respectively.

## Stage 2: Machines Game

If the `P` command is given as input, the program should play ten next actions or play until the end of the game, whichever comes first. The game should start from the board configuration obtained after processing the Stage 0 input. If the game ends within the next ten turns (including the last turn when no action is possible), the winner should be reported based on the rules from Stage 1. The computation of actions and reporting of the winner should follow the same rules as in Stage 1.

*** Note: This is my submission for *Assignment 2 of COMP10002 Foundations of Algorithms in Sem 2 2021.* ***
