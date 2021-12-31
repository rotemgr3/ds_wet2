#include "library1.h"
#include "playersmanager.h"

void* Init()
{
    try { 
        PlayersManager *PM = new PlayersManager();
        return (void *)PM;     
    } catch (std::bad_alloc& err) {
        return NULL;
    }
}

StatusType AddGroup(void *DS, int GroupID)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->AddGroup(GroupID);
    return res;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->AddPlayer(PlayerID, GroupID, Level); 
    return res;
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->RemovePlayer(PlayerID); 
    return res;
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->ReplaceGroup(GroupID, ReplacementID);
    return res;
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->IncreaseLevel(PlayerID, LevelIncrease);
    return res; 
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->GetHighestLevel(GroupID, PlayerID);
    return res;
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->GetAllPlayersByLevel(GroupID, Players, numOfPlayers);
    return res;  
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players)
{
    if (DS == nullptr) return INVALID_INPUT;
    StatusType res = (StatusType)((PlayersManager *)DS)->GetGroupsHighestLevel(numOfGroups, Players);
    return res;
}

void Quit(void** DS)
{   
    if (DS == nullptr) return;
    delete (*(PlayersManager **)DS);
    *DS = nullptr;
}