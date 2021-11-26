#ifndef PLAYER_H_
#define PLAYER_H_


#include <memory>
#include "group.h"

class Player {
    private:
    public:
        int playerId;
        int groupId;
        int level;
        std::shared_ptr<Group> groupPtr;
        Player(const int playerId, const int groupId, const int level) : playerId(playerId), groupId(groupId), level(level), groupPtr(nullptr) {}
        Player(const Player& player) = default;
        ~Player() = default;
};

#endif /* PLAYER_H */