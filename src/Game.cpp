#include <sstream>
#include "Game.h"
#include "players/PlayerHuman.h"
#include "pieces/Pawn.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "enums/PieceType.h"
#include "exceptions/SaveFileException.h"
#include "players/PlayerEasyBot.h"
#include "players/PlayerMediumBot.h"
#include "players/PlayerHardBot.h"

void Game::initialize(GameMode mode) {
    std::list<std::shared_ptr<Piece>> white_pieces;
    std::list<std::shared_ptr<Piece>> black_pieces;

    black_pieces.push_back(std::make_shared<Rook>(0, 0, Color::BLACK));
    black_pieces.push_back(std::make_shared<Knight>(0, 1, Color::BLACK));
    black_pieces.push_back(std::make_shared<Bishop>(0, 2, Color::BLACK));
    black_pieces.push_back(std::make_shared<Queen>(0, 3, Color::BLACK));
    black_pieces.push_back(std::make_shared<King>(0, 4, Color::BLACK));
    black_pieces.push_back(std::make_shared<Bishop>(0, 5, Color::BLACK));
    black_pieces.push_back(std::make_shared<Knight>(0, 6, Color::BLACK));
    black_pieces.push_back(std::make_shared<Rook>(0, 7, Color::BLACK));

    black_pieces.push_back(std::make_shared<Pawn>(1, 0, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 1, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 2, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 3, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 4, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 5, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 6, Color::BLACK));
    black_pieces.push_back(std::make_shared<Pawn>(1, 7, Color::BLACK));

    white_pieces.push_back(std::make_shared<Pawn>(6, 0, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 1, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 2, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 3, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 4, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 5, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 6, Color::WHITE));
    white_pieces.push_back(std::make_shared<Pawn>(6, 7, Color::WHITE));

    white_pieces.push_back(std::make_shared<Rook>(7, 0, Color::WHITE));
    white_pieces.push_back(std::make_shared<Knight>(7, 1, Color::WHITE));
    white_pieces.push_back(std::make_shared<Bishop>(7, 2, Color::WHITE));
    white_pieces.push_back(std::make_shared<Queen>(7, 3, Color::WHITE));
    white_pieces.push_back(std::make_shared<King>(7, 4, Color::WHITE));
    white_pieces.push_back(std::make_shared<Bishop>(7, 5, Color::WHITE));
    white_pieces.push_back(std::make_shared<Knight>(7, 6, Color::WHITE));
    white_pieces.push_back(std::make_shared<Rook>(7, 7, Color::WHITE));

    board = Board(white_pieces, black_pieces);

    game_mode = mode;
    switch (game_mode) {
        case GameMode::VERSUS:
            init_versus(white_pieces, black_pieces);
            break;
        case GameMode::EASY:
            init_single_easy(white_pieces, black_pieces);
            break;
        case GameMode::MEDIUM:
            init_single_medium(white_pieces, black_pieces);
            break;
        case GameMode::HARD:
            init_single_hard(white_pieces, black_pieces);
            break;
    }
}

void Game::init_single_easy(const std::list<std::shared_ptr<Piece>> & white_pieces,
                            const std::list<std::shared_ptr<Piece>> & black_pieces) {
    player_white = std::make_unique<PlayerHuman>(white_pieces);
    player_black = std::make_unique<PlayerEasyBot>(black_pieces);
}

void Game::init_single_medium(const std::list<std::shared_ptr<Piece>> & white_pieces,
                              const std::list<std::shared_ptr<Piece>> & black_pieces) {
    player_white = std::make_unique<PlayerHuman>(white_pieces);
    player_black = std::make_unique<PlayerMediumBot>(black_pieces);
}

void Game::init_single_hard(const std::list<std::shared_ptr<Piece>> & white_pieces,
                            const std::list<std::shared_ptr<Piece>> & black_pieces) {
    player_white = std::make_unique<PlayerHuman>(white_pieces);
    player_black = std::make_unique<PlayerHardBot>(black_pieces);
}

void Game::init_versus(const std::list<std::shared_ptr<Piece>> & white_pieces,
                       const std::list<std::shared_ptr<Piece>> & black_pieces) {
    player_white = std::make_unique<PlayerHuman>(white_pieces);
    player_black = std::make_unique<PlayerHuman>(black_pieces);
}

void Game::print(std::ostream & out, const std::string & move_str) const {
    board.print(out);
    if (resigned) {
        out << "Game resigned! ";
        color_on_turn == Color::WHITE ? out << "Black wins!" : out << "White wins!";
        out << std::endl;
        return;
    }
    if (checkmate) {
        out << "Checkmate! ";
        color_on_turn == Color::WHITE ? out << "White wins!" : out << "Black wins!";
        out << std::endl;
        return;
    }
    if (stalemate) {
        out << "Stalemate! It's a draw. " << std::endl;
        return;
    }
    out << move_str << std::endl;
    out << "Side on turn: " << color_on_turn << std::endl;
    if (check)
        out << "Check!" << std::endl;
}

std::ostream & operator<<(std::ostream & out, const Game & game) {
    out << game.game_mode << std::endl;
    game.color_on_turn == Color::WHITE ? out << 0 << std::endl : out << 1 << std::endl;
    out << std::noboolalpha << game.check << std::endl;
    out << game.board;

    auto white = game.player_white->get_pieces();
    white.erase(std::remove_if(white.begin(), white.end(), [](const std::shared_ptr<Piece> & piece) {
        return not piece->is_available();
    }), white.end());
    auto black = game.player_black->get_pieces();
    black.erase(std::remove_if(black.begin(), black.end(), [](const std::shared_ptr<Piece> & piece) {
        return not piece->is_available();
    }), black.end());

    out << (white.size() + black.size()) << std::endl;
    for (const auto & piece: white)
        out << piece->get_x() << " " << piece->get_y() << " " << std::noboolalpha << piece->has_already_moved()
            << std::endl;
    for (const auto & piece: black)
        out << piece->get_x() << " " << piece->get_y() << " " << std::noboolalpha << piece->has_already_moved()
            << std::endl;


    auto captured = game.captured();
    out << (captured.first.size() + captured.second.size()) << std::endl;
    for (const auto & piece: captured.first)
        out << *piece << std::endl;
    for (const auto & piece: captured.second)
        out << *piece << std::endl;
    return out << std::endl;
}

std::istream & operator>>(std::istream & in, Game & game) {
    int mode;
    if (!(in >> mode)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (mode < 0 or mode > 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    int color;
    if (!(in >> color)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (color == 0)
        game.color_on_turn = Color::WHITE;
    else if (color == 1)
        game.color_on_turn = Color::BLACK;
    else {
        in.setstate(std::ios::failbit);
        return in;
    }

    int check;
    if (!(in >> check)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (check == 0)
        game.check = false;
    else if (check == 1)
        game.check = true;
    else {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::list<std::shared_ptr<Piece>> white_pieces;
    std::list<std::shared_ptr<Piece>> black_pieces;

    size_t white_king_cnt = 0, black_king_cnt = 0, white_pawn_cnt = 0, black_pawn_cnt = 0;

    for (size_t i = 0; i < 8; i++) {
        std::string row;
        if (!(in >> row)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        if (row.length() != 8) {
            in.setstate(std::ios::failbit);
            return in;
        }
        for (size_t j = 0; j < 8; j++) {
            std::shared_ptr<Piece> piece;
            switch (row[j]) {
                case 'P':
                    piece = std::make_shared<Pawn>(i, j, Color::WHITE);
                    white_pawn_cnt++;
                    break;
                case 'p':
                    piece = std::make_shared<Pawn>(i, j, Color::BLACK);
                    black_pawn_cnt++;
                    break;
                case 'R':
                    piece = std::make_shared<Rook>(i, j, Color::WHITE);
                    break;
                case 'r':
                    piece = std::make_shared<Rook>(i, j, Color::BLACK);
                    break;
                case 'N':
                    piece = std::make_shared<Knight>(i, j, Color::WHITE);
                    break;
                case 'n':
                    piece = std::make_shared<Knight>(i, j, Color::BLACK);
                    break;
                case 'B':
                    piece = std::make_shared<Bishop>(i, j, Color::WHITE);
                    break;
                case 'b':
                    piece = std::make_shared<Bishop>(i, j, Color::BLACK);
                    break;
                case 'Q':
                    piece = std::make_shared<Queen>(i, j, Color::WHITE);
                    break;
                case 'q':
                    piece = std::make_shared<Queen>(i, j, Color::BLACK);
                    break;
                case 'K':
                    piece = std::make_shared<King>(i, j, Color::WHITE);
                    white_king_cnt++;
                    break;
                case 'k':
                    piece = std::make_shared<King>(i, j, Color::BLACK);
                    black_king_cnt++;
                    break;
                case '-':
                    piece = nullptr;
                    break;
                default:
                    in.setstate(std::ios::failbit);
                    return in;
            }

            game.board[i][j] = piece;
            if (piece != nullptr)
                piece->get_color() == Color::WHITE ? white_pieces.push_back(piece) : black_pieces.push_back(piece);
        }
    }

    if (white_pawn_cnt > 8 or black_pawn_cnt > 8 or white_king_cnt != 1 or black_king_cnt != 1) {
        in.setstate(std::ios::failbit);
        return in;
    }

    switch (mode) {
        case 0:
            game.game_mode = GameMode::VERSUS;
            game.init_versus(white_pieces, black_pieces);
            break;
        case 1:
            game.game_mode = GameMode::EASY;
            game.init_single_easy(white_pieces, black_pieces);
            break;
        case 2:
            game.game_mode = GameMode::MEDIUM;
            game.init_single_medium(white_pieces, black_pieces);
            break;
        case 3:
            game.game_mode = GameMode::HARD;
            game.init_single_hard(white_pieces, black_pieces);
            break;
        default:
            in.setstate(std::ios::failbit);
            return in; // should not happen, was checked before
    }

    size_t pieces_cnt;
    if (!(in >> pieces_cnt)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (size_t i = 0; i < pieces_cnt; i++) {
        int x, y, f;
        if (!(in >> x >> y >> f)) {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (x < 0 or x > 7 or
            y < 0 or y > 7 or
            f < 0 or f > 1) {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (game.board[x][y] == nullptr) {
            in.setstate(std::ios::failbit);
            return in;
        }

        game.board[x][y]->set_has_moved(f);
    }

    if (!(in >> pieces_cnt)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (size_t i = 0; i < pieces_cnt; i++) {
        char type;
        if (!(in >> type)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::shared_ptr<Piece> p;
        switch (type) {
            case 'R':
                p = std::make_shared<Rook>(-1, -1, Color::WHITE);
                p->capture();
                game.player_white->get_pieces().push_back(p);
                break;

            case 'r':
                p = std::make_shared<Rook>(-1, -1, Color::BLACK);
                p->capture();
                game.player_black->get_pieces().push_back(p);
                break;

            case 'N':
                p = std::make_shared<Knight>(-1, -1, Color::WHITE);
                p->capture();
                game.player_white->get_pieces().push_back(p);
                break;
            case 'n':
                p = std::make_shared<Knight>(-1, -1, Color::BLACK);
                p->capture();
                game.player_black->get_pieces().push_back(p);
                break;
            case 'B':
                p = std::make_shared<Bishop>(-1, -1, Color::WHITE);
                p->capture();
                game.player_white->get_pieces().push_back(p);
                break;
            case 'b':
                p = std::make_shared<Bishop>(-1, -1, Color::BLACK);
                p->capture();
                game.player_black->get_pieces().push_back(p);
                break;
            case 'Q':
                p = std::make_shared<Queen>(-1, -1, Color::WHITE);
                p->capture();
                game.player_white->get_pieces().push_back(p);
                break;
            case 'q':
                p = std::make_shared<Queen>(-1, -1, Color::BLACK);
                p->capture();
                game.player_black->get_pieces().push_back(p);
                break;
            case 'P':
                p = std::make_shared<Pawn>(-1, -1, Color::WHITE);
                p->capture();
                game.player_white->get_pieces().push_back(p);
                break;
            case 'p':
                p = std::make_shared<Pawn>(-1, -1, Color::BLACK);
                p->capture();
                game.player_black->get_pieces().push_back(p);
                break;
            default: // aj king
                in.setstate(std::ios::failbit);
                return in;
        }
    }

    return in;
}

bool Game::move_player(const Interface & interface) {

    std::string move_str;
    bool move_result;

    color_on_turn == Color::WHITE ? move_result = move_white(interface, move_str)
                                  : move_result = move_black(interface, move_str);

    if (not move_result) {
        if (checkmate or stalemate) {
            interface.print_game(*this, "");
            return false;
        }
        return true;
    }

    if (is_single()) {
        interface.print_game(*this, move_str);
        color_on_turn == Color::WHITE ? move_result = move_white(interface, move_str)
                                      : move_result = move_black(interface, move_str);
        if (not move_result) {
            interface.print_game(*this, "");
            return false;
        }
    }

    interface.print_game(*this, move_str);

    return true;
}

bool Game::move_white(const Interface & interface, std::string & move_str) {
    bool move_result = player_white->take_turn(interface, board, last_move, player_black->get_pieces(),
                                               check, checkmate, stalemate);
    if (move_result) {
        move_str = to_string(color_on_turn) + "'s move: " +
                   coords_map_y[last_move.get_y_from()] + coords_map_x[last_move.get_x_from()] + " ==> " +
                   coords_map_y[last_move.get_y_to()] + coords_map_x[last_move.get_x_to()] + " (" +
                   to_string(board[last_move.get_x_to()][last_move.get_y_to()]->type()) + ")";
        change_color_on_turn();
    }

    return move_result;
}

bool Game::move_black(const Interface & interface, std::string & move_str) {
    bool move_result = player_black->take_turn(interface, board, last_move, player_white->get_pieces(),
                                               check, checkmate, stalemate);

    if (move_result) {
        move_str = to_string(color_on_turn) + "'s move: " +
                   coords_map_y[last_move.get_y_from()] + coords_map_x[last_move.get_x_from()] + " ==> " +
                   coords_map_y[last_move.get_y_to()] + coords_map_x[last_move.get_x_to()] + " (" +
                   to_string(board[last_move.get_x_to()][last_move.get_y_to()]->type()) + ")";
        change_color_on_turn();
    }

    return move_result;
}

bool Game::is_single() const {
    return game_mode != GameMode::VERSUS;
}

void Game::change_color_on_turn() {
    color_on_turn == Color::WHITE ? color_on_turn = Color::BLACK : color_on_turn = Color::WHITE;
}

std::pair<std::list<std::shared_ptr<Piece>>, std::list<std::shared_ptr<Piece>>> Game::captured() const {
    std::list<std::shared_ptr<Piece>> white = player_white->get_pieces();
    std::list<std::shared_ptr<Piece>> black = player_black->get_pieces();

    white.erase(std::remove_if(white.begin(), white.end(), [](const std::shared_ptr<Piece> & piece) {
        return not piece->was_captured();
    }), white.end());

    black.erase(std::remove_if(black.begin(), black.end(), [](const std::shared_ptr<Piece> & piece) {
        return not piece->was_captured();
    }), black.end());

    return {white, black};
}

void Game::resign(const Interface & interface) {
    resigned = true;
    interface.print_game(*this, "");
}
