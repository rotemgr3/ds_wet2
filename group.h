#ifndef GROUP_H_
#define GROUP_H_


#include <memory>
#include "maxlevel.h"
#include "level.h"
#include "BST.h"

class Group {
    private:
    public:
        int groupId;
        MaxLevel maxLevel;
        BST<int, Level> levelsTree;
        int numOfPlayers;
        Group(const int groupId) : groupId(groupId), maxLevel(), levelsTree(), numOfPlayers(0) {}
        ~Group() = default;     

};

#endif /* GROUP_H */