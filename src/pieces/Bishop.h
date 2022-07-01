#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(int x, int y, Color color);

    virtual std::list<Move> available_moves(Board & board, const Move &) override;

    virtual PieceType type() const override;

    virtual std::shared_ptr<Piece> clone() const override;

private:
    virtual std::ostream & print(std::ostream & out, bool pretty) const override;

};
