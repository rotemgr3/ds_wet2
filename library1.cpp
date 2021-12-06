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
    StatusType res = (StatusType)((PlayersManager *)DS)->AddGroup(GroupID);
    return res;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->AddPlayer(PlayerID, GroupID, Level); 
    return res;
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->RemovePlayer(PlayerID); 
    return res;
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->ReplaceGroup(GroupID, ReplacementID);
    return res;
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->IncreaseLevel(PlayerID, LevelIncrease);
    return res; 
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->GetHighestLevel(GroupID, PlayerID);
    return res;
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->GetAllPlayersByLevel(GroupID, Players, numOfPlayers);
    return res;  
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players)
{
    StatusType res = (StatusType)((PlayersManager *)DS)->GetGroupsHighestLevel(numOfGroups, Players);
    return res;
}

void Quit(void** DS)
{
    delete (*(PlayersManager **)DS);
    *DS = nullptr;
}