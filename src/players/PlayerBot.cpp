#include "PlayerBot.h"
#include <utility>

PlayerBot::PlayerBot(std::list<std::shared_ptr<Piece>> pieces) : Player(std::move(pieces)) {}
