#pragma once

#include "Player.h"

class PlayerBot : public Player {
public:
    explicit PlayerBot(std::list<std::shared_ptr<Piece>> pieces);
};
