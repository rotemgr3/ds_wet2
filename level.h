#ifndef LEVEL_H_
#define LEVEL_H_

#include "player.h"

class Level {
    private:
    public:
        int levelId;
        BST<int, Player> playersTree;
        Level(const int level) : levelId(level), playersTree() {}
        ~Level() = default;
        bool operator<(const Level& level) { return this->levelId < level.levelId; }

        
};

#endif /* LEVEL_H */