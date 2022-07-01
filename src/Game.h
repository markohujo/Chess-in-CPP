#pragma once


#include <memory>
#include "players/Player.h"
#include "Board.h"
#include "enums/Color.h"
#include "Interface.h"
#include "enums/GameMode.h"

class Interface;

class Player;

class Game {
public:
    Game() = default;

    /**
     * Initializes game in the given mode.
     * Creates both players' pieces and places them on the board
     */
    void initialize(GameMode mode);

    /**
     * Moves player and checks for checkmate or stalemate
     * If this game is single player, moves again with the opponent
     *
     * @return true if game continues and move was successfully executed
     * @return false if game ends after this move (checkmate or stalemate) or if user input was invalid
     */
    bool move_player(const Interface & interface);

    /**
     * Finds both players' captured pieces
     * @return pair of white and black captured pieces
     */
    std::pair<std::list<std::shared_ptr<Piece>>, std::list<std::shared_ptr<Piece>>> captured() const;

    /**
     * @return true if this game is played in single player mode (game_mode is not GameMode::VERSUS)
     * @return false otherwise
     */
    bool is_single() const;

    /** Used for pretty print when showing the user */
    void print(std::ostream & out, const std::string & move_str) const;

    /** Used for outputting to file when saving the game */
    friend std::ostream & operator<<(std::ostream & out, const Game & game);

    /** Used for inputting from file when loading the game */
    friend std::istream & operator>>(std::istream & in, Game & game);


    void resign(const Interface & interface);

private:
    Board board;

    std::unique_ptr<Player> player_white;

    std::unique_ptr<Player> player_black;

    Color color_on_turn = Color::WHITE;

    Move last_move = Move(-1, -1, -1, -1);

    GameMode game_mode = GameMode::VERSUS;

    bool checkmate = false;

    bool stalemate = false;

    bool check = false;

    bool resigned = false;

    bool move_white(const Interface & interface, std::string & move_str);

    bool move_black(const Interface & interface, std::string & move_str);

    /**
     * Initializes easy single player game
     */
    void init_single_easy(const std::list<std::shared_ptr<Piece>> & white_pieces,
                          const std::list<std::shared_ptr<Piece>> & black_pieces);

    /**
     * Initializes medium single player game
     */
    void init_single_medium(const std::list<std::shared_ptr<Piece>> & white_pieces,
                            const std::list<std::shared_ptr<Piece>> & black_pieces);

    /**
     * Initializes hard single player game
     */
    void init_single_hard(const std::list<std::shared_ptr<Piece>> & white_pieces,
                          const std::list<std::shared_ptr<Piece>> & black_pieces);

    /**
     * Initializes versus 1v1 game
     */
    void init_versus(const std::list<std::shared_ptr<Piece>> & white_pieces,
                     const std::list<std::shared_ptr<Piece>> & black_pieces);

    void change_color_on_turn();

    std::unordered_map<int, char> coords_map_y = {
            {0, 'a'},
            {1, 'b'},
            {2, 'c'},
            {3, 'd'},
            {4, 'e'},
            {5, 'f'},
            {6, 'g'},
            {7, 'h'}
    };

    std::unordered_map<int, char> coords_map_x = {
            {7, '1'},
            {6, '2'},
            {5, '3'},
            {4, '4'},
            {3, '5'},
            {2, '6'},
            {1, '7'},
            {0, '8'}
    };
};
