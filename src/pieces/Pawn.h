#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(int x, int y, Color color);

    virtual std::list<Move> available_moves(Board & board, const Move &) override;

    virtual PieceType type() const override;

    virtual std::shared_ptr<Piece> clone() const override;

private:
    virtual std::ostream & print(std::ostream & out, bool pretty) const override;

    std::list<Move> white_moves(const Board & board, const Move & last_move);

    std::list<Move> black_moves(const Board & board, const Move & last_move);

    /**
     * Generates all available pawn's move depending on the given operation (excluding en passant moves)
     * @tparam Op - determines what color this pawn is and how it moves forward
     * @param operation - either std::minus for white pawn or std::plus black pawn
     * @return list containing moves
     */
    template<typename Op>
    std::list<Move> generate_moves(const Board & board, const Op & operation);
};
