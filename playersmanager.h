#ifndef PLAYERS_MANAGER_H_
#define PLAYERS_MANAGER_H_

#include <memory>
#include "BST.h"
#include "group.h"
#include "level.h"
#include "player.h"
#include "maxlevel.h"

typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} PMStatusType;


class PlayersManager {
    private:
        static void AddPlayerGroupsTree(Group& group, const int playerId, const Player& newPlayer);
        static void AddPlayerAllLevelsTree(BST<int, Level>& allLevelsTree, const int playerId, const Player& newPlayer);
        static void UpdateMaxLevel(MaxLevel& maxLevel,const int level,const int playerId);
        static void RemovePlayerGroupsTree(Group& group, const Player& playerToRemove);
        static void PlayersManager::RemovePlayerNonEmptyGroupsTree(BST<int, std::shared_ptr<Group>>& nonEmptyGroupsTree,
                                                                   const int groupId, const int numOfPlayers);
        static void RemovePlayerallLevelsTree(BST<int, Level>& allLevelsTree, const Player& playerToRemove);
        static void SearchAndUpdateMaxLevel(BST<int, Level>& levelsTree, MaxLevel& maxlevel);
        static void InOrderByPlayer(std::shared_ptr<Node<int, Player>> player, int *players, int i);
        static void InOrderByLevel(std::shared_ptr<Node<int, Level>> level, int **players, int *numOfPlayers, int i);
        static void Reverse(int **players, int left, int right);
        static void PlayersManager::GetAllPlayersByLevelAux(BST<int, Level>& levelsTree, int **players, int *numOfPlayers);
        static void PlayersManager::InOrderNElements(std::shared_ptr<Node<int, std::shared_ptr<Group>>> root, int numOfGroups, int *players);
        
    public:
        BST<int, Group> groupsTree;
        BST<int, std::shared_ptr<Group>> nonEmptyGroupsTree;
        BST<int, Level> allLevelsTree;
        BST<int, Player> allPlayersTree;
        MaxLevel maxLevel;
        PlayersManager() = default;
        ~PlayersManager() = default;
        PMStatusType AddGroup(const int groupId);
        PMStatusType AddPlayer(const int playerId, const int groupId, const int level, std::shared_ptr<Group> groupPtr = nullptr);
        PMStatusType RemovePlayer(const int playerId);
        PMStatusType ReplaceGroup(const int groupId, const int replacementId);
        PMStatusType IncreaseLevel(const int playerId, const int levelIncrease);
        PMStatusType GetHighestLevel(const int groupId, int *playerId);
        PMStatusType GetAllPlayersByLevel(const int groupId, int **players, int *numOfPlayers);
        PMStatusType GetGroupsHighestLevel(int numOfGroups, int **players);
        
};

#endif /* LEVEL_H */