#include "playersmanager.h"
#include <exception>


PMStatusType PlayersManager::AddGroup(const int groupId)
{
    if (groupId <= 0)
        return PM_INVALID_INPUT;
    if (this->groupsTree.Find(groupId))
        return PM_FAILURE;
    
    try {
        Group *newGroup = new Group(groupId);//allocation
        this->groupsTree.Insert(groupId, *newGroup);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }

    return PM_SUCCESS;
}

void PlayersManager::AddPlayerGroupsTree(Group& group, const int playerId, Player& newPlayer)
{
    if (!group.levelsTree.Find(newPlayer.level)) {
            Level *newLevel = new Level(newPlayer.level);//allocation
            group.levelsTree.Insert(newPlayer.level, *newLevel);
    }
    group.levelsTree.Get(newPlayer.level)->playersTree.Insert(playerId, newPlayer);
    group.numOfPlayers++;

    PlayersManager::UpdateMaxLevel(group.maxLevel, newPlayer.level, newPlayer.playerId);
        
}

void PlayersManager::AddPlayerAllLevelsTree(BST<int, Level>& allLevelsTree, const int playerId, Player& newPlayer)
{
    if (!allLevelsTree.Find(newPlayer.level)){
            Level *newLevel = new Level(newPlayer.level);//allocation
            allLevelsTree.Insert(newPlayer.level, *newLevel);
    }
    allLevelsTree.Get(newPlayer.level)->playersTree.Insert(playerId, newPlayer);
}

void PlayersManager::UpdateMaxLevel(MaxLevel& maxLevel,const int level,const int playerId)
{
     if (maxLevel.level < level) {
            maxLevel.level = level;
            maxLevel.playerId = playerId;
        }
    else if (maxLevel.level == level && maxLevel.playerId > playerId)
            maxLevel.playerId = playerId;
}

PMStatusType PlayersManager::AddPlayer(const int playerId, const int groupId, const int level, std::shared_ptr<Group> groupPtr)
{
    if (playerId <= 0 || groupId <= 0 || level < 0)
        return PM_INVALID_INPUT;

    if (!(this->groupsTree.Find(groupId)) || this->allPlayersTree.Find(playerId))
        return PM_FAILURE;
    
    try {
        Player *newPlayer = new Player(playerId, groupId, level);//allocation
        if (groupPtr == nullptr)
            groupPtr = this->groupsTree.Get(groupId);
        newPlayer->groupPtr = groupPtr;
        
        this->allPlayersTree.Insert(playerId, *newPlayer);

        PlayersManager::UpdateMaxLevel(this->maxLevel, level, playerId);

        PlayersManager::AddPlayerGroupsTree(*groupPtr, playerId, *newPlayer);

        PlayersManager::AddPlayerAllLevelsTree(this->allLevelsTree, playerId, *newPlayer);

        this->nonEmptyGroupsTree.Insert(groupId, groupPtr);

    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    
    return PM_SUCCESS;
}

void PlayersManager::RemovePlayerGroupsTree(Group& group, const Player& playerToRemove)
{
    std::shared_ptr<Level> playerLevel = group.levelsTree.Get(playerToRemove.level);
    playerLevel->playersTree.Remove(playerToRemove.playerId);
    if (playerLevel->playersTree.size == 0)
        playerLevel.reset();// delete allocation
    group.numOfPlayers--;
}

void PlayersManager::RemovePlayerNonEmptyGroupsTree(BST<int, std::shared_ptr<Group>>& nonEmptyGroupsTree, const int groupId, const int numOfPlayers)
{
    if (numOfPlayers == 0)
        nonEmptyGroupsTree.Remove(groupId);
}

void PlayersManager::RemovePlayerallLevelsTree(BST<int, Level>& allLevelsTree, const Player& playerToRemove)
{
    std::shared_ptr<Level> level = allLevelsTree.Get(playerToRemove.level);
    level->playersTree.Remove(playerToRemove.playerId);
    if (level->playersTree.size == 0)
        level.reset();// delete allocation
}

void PlayersManager::SearchAndUpdateMaxLevel(BST<int, Level>& levelsTree, MaxLevel& maxlevel)
{
    Level maxLevel = levelsTree.GetMax();
    int newMaxLevel = maxLevel.levelId;
    int newMaxPlayerId = maxLevel.playersTree.GetMin().playerId;
    PlayersManager::UpdateMaxLevel(maxlevel, newMaxLevel, newMaxPlayerId);
}

PMStatusType PlayersManager::RemovePlayer(const int playerId)
{
    if (playerId <= 0)
        return PM_INVALID_INPUT;

    std::shared_ptr<Player> playerToRemovePtr = this->allPlayersTree.Get(playerId);
    if (playerToRemovePtr == nullptr)
        return PM_FAILURE;

    int playerToRemoveId = playerToRemovePtr->playerId;
    this->allPlayersTree.Remove(playerToRemoveId);

    PlayersManager::RemovePlayerGroupsTree(*playerToRemovePtr->groupPtr, *playerToRemovePtr);
    PlayersManager::RemovePlayerNonEmptyGroupsTree(this->nonEmptyGroupsTree, playerToRemovePtr->groupId, playerToRemovePtr->groupPtr->numOfPlayers);
    PlayersManager::RemovePlayerallLevelsTree(this->allLevelsTree, *playerToRemovePtr);

    PlayersManager::SearchAndUpdateMaxLevel(this->allLevelsTree, this->maxLevel);
    PlayersManager::SearchAndUpdateMaxLevel(playerToRemovePtr->groupPtr->levelsTree, playerToRemovePtr->groupPtr->maxLevel);
    playerToRemovePtr.reset(); // delete allocation
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::ReplaceGroup(const int groupId, const int replacementId)
{
    if (groupId <= 0 || replacementId <= 0 || groupId == replacementId)
        return PM_INVALID_INPUT;
        
    std::shared_ptr<Group> group = this->groupsTree.Get(groupId);
    std::shared_ptr<Group> replacement = this->groupsTree.Get(replacementId);
    if (group == nullptr || replacement == nullptr)
        return PM_FAILURE;
    
    if (group->numOfPlayers == 0) {
        this->groupsTree.Remove(groupId);
        return PM_SUCCESS;
    }

    Map* map = nullptr;
    try {
        map = BST<int, Level>::MergeToArr(group->levelsTree, replacement->levelsTree);
        int newSize = PlayersManager::RemoveDuplicates(map, group->levelsTree.size + replacement->levelsTree.size);
        BST<int, Level> merged = *(BST<int, Level>::ArrToBST(map, newSize));
        PlayersManager::UpdateLevels(merged.root, replacement, replacementId);
        replacement->numOfPlayers += group->numOfPlayers;
        replacement->levelsTree = merged;
        PlayersManager::UpdateMaxLevel(replacement->maxLevel, group->maxLevel.level,  group->maxLevel.playerId);
        this->nonEmptyGroupsTree.Remove(groupId);
        this->groupsTree.Remove(groupId);
        group.reset(); // delete allocation
        MapDestroy(map);
    } catch(std::bad_alloc& err) {
        MapDestroy(map);
        return PM_ALLOCATION_ERROR;
    }
    return PM_SUCCESS;
}

void PlayersManager::UpdateLevels(std::shared_ptr<Node<int, Level>> root, std::shared_ptr<Group> replacement, int replacementId)
{
    if(root == nullptr)
        return;
    PlayersManager::UpdateLevels(root->left, replacement, replacementId);
    PlayersManager::UpdatePlayers(root->data->playersTree.root, replacement, replacementId);
    PlayersManager::UpdateLevels(root->right, replacement, replacementId);

}
void PlayersManager::UpdatePlayers(std::shared_ptr<Node<int, Player>> root, std::shared_ptr<Group> replacement, int replacementId)
{
     if(root == nullptr)
        return;
    PlayersManager::UpdatePlayers(root->left, replacement, replacementId);
    root->data->groupId = replacementId;
    root->data->groupPtr = replacement;
    PlayersManager::UpdatePlayers(root->right, replacement, replacementId);  
}

int PlayersManager::RemoveDuplicates(Map *map, int n)
{
    int newsize = n;
    std::shared_ptr<int> *keyArr = (std::shared_ptr<int> *)(map->key);
    std::shared_ptr<Level> *dataArr = (std::shared_ptr<Level> *)(map->data);
    for (int i = 0; i < n - 1; i++) {
        if (dataArr[i] == nullptr)
            continue;
        if (*(keyArr[i]) == *(keyArr[i + 1])) {
            dataArr[i]->playersTree = *(BST<int, Player>::Merge(dataArr[i]->playersTree, dataArr[i + 1]->playersTree));
            dataArr[i + 1].reset();
            dataArr[i + 1] = nullptr;
            newsize--;
        }
    }
    return newsize;
}


PMStatusType PlayersManager::IncreaseLevel(const int playerId, const int levelIncrease)
{
    if (playerId <= 0 || levelIncrease <= 0)
        return PM_INVALID_INPUT;
        
    std::shared_ptr<Player> playerToIncreasePtr = this->allPlayersTree.Get(playerId);
    if (playerToIncreasePtr == nullptr)
        return PM_FAILURE;

    Player newPlayer = Player(*playerToIncreasePtr);
    newPlayer.level += levelIncrease;
    
    PlayersManager::RemovePlayer(playerToIncreasePtr->playerId);
    PMStatusType insertRes =  PlayersManager::AddPlayer(newPlayer.playerId, newPlayer.groupId, newPlayer.level, newPlayer.groupPtr);
    if (insertRes == PM_ALLOCATION_ERROR)   
        return PM_ALLOCATION_ERROR;
    
    return PM_SUCCESS;
}

PMStatusType PlayersManager::GetHighestLevel(const int groupId, int* playerId)
{
    if (groupId == 0 || playerId == nullptr)
        return PM_INVALID_INPUT;
        
    if (groupId < 0) {
        *playerId = this->maxLevel.playerId;
        return PM_SUCCESS;
    }
    
    std::shared_ptr<Group> group = this->groupsTree.Get(groupId);
    if (group == nullptr)
        return PM_FAILURE;

    *playerId = group->maxLevel.playerId;
    return PM_SUCCESS;
}

void PlayersManager::InOrderByLevel(std::shared_ptr<Node<int, Level>> level, int **players, int *numOfPlayers, int i)
{
    if (level == nullptr)
        return;

    PlayersManager::InOrderByLevel(level->left, players, numOfPlayers, i);
    PlayersManager::InOrderByPlayer(level->data->playersTree.root, *players, i);
    
    int left = i;
    int right = i + level->data->playersTree.size - 1;
    PlayersManager::Reverse(players, left, right);
    
    PlayersManager::InOrderByLevel(level->right, players, numOfPlayers, right + 1);
    return;
}

void PlayersManager::Reverse(int **players, int left, int right)
{
    while (left < right) {
        int temp = (*players)[left];
        (*players)[left] = (*players)[right];
        (*players)[right] = temp;
        right--;
        left++;
    }
}
void PlayersManager::InOrderByPlayer(std::shared_ptr<Node<int, Player>> player, int *players, int i)
{
    if (player == nullptr)
        return;

    PlayersManager::InOrderByPlayer(player->left, players, i);
    players[i] = player->data->playerId;
    PlayersManager::InOrderByPlayer(player->right, players, i + 1);
    return;
}

void PlayersManager::GetAllPlayersByLevelAux(BST<int, Level>& levelsTree, int **players, int *numOfPlayers)
{
    *numOfPlayers = levelsTree.size;
    *players = (int *)malloc(sizeof(int) * (*numOfPlayers));
    PlayersManager::InOrderByLevel(levelsTree.root, players, numOfPlayers, 0);
}

PMStatusType PlayersManager::GetAllPlayersByLevel(const int groupId, int **players, int *numOfPlayers)
{
    if (groupId == 0 || players == nullptr || numOfPlayers == nullptr)
        return PM_INVALID_INPUT;
    
    if (groupId < 0) {
        try { 
            PlayersManager::GetAllPlayersByLevelAux(this->allLevelsTree, players, numOfPlayers);
        } catch (std::bad_alloc& error) {
            return PM_ALLOCATION_ERROR;
        }
        return PM_SUCCESS;
    }  
        
    std::shared_ptr<Group> group = this->groupsTree.Get(groupId);
    if (group == nullptr)
        return PM_FAILURE;

    try { 
        PlayersManager::GetAllPlayersByLevelAux(group->levelsTree, players, numOfPlayers);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    return PM_SUCCESS; 
}

void PlayersManager::InOrderNElements(std::shared_ptr<Node<int, std::shared_ptr<Group>>> root, int numOfGroups, int *players)
{
    if (root == nullptr)
        return;
    if (numOfGroups == 0)
        return;

    PlayersManager::InOrderNElements(root->left, numOfGroups, players);
    *players = (*(root->data))->maxLevel.playerId;
    PlayersManager::InOrderNElements(root->right, numOfGroups - 1, players + 1);

    return;
}

PMStatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **players)
{
    if (numOfGroups < 1 || players == nullptr)
        return PM_INVALID_INPUT;
    
    if (numOfGroups > this->nonEmptyGroupsTree.size)
        return PM_FAILURE;

    try {
        *players = (int *)malloc(sizeof(int) * numOfGroups); 
        PlayersManager::InOrderNElements(this->nonEmptyGroupsTree.root, numOfGroups, *players);
    } catch (std::bad_alloc& error) {
        return PM_ALLOCATION_ERROR;
    }
    return PM_SUCCESS; 
}