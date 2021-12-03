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
    return (StatusType)((PlayersManager *)DS)->AddGroup(GroupID);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level)
{
    return (StatusType)((PlayersManager *)DS)->AddPlayer(PlayerID, GroupID, Level);   
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    return (StatusType)((PlayersManager *)DS)->RemovePlayer(PlayerID);   
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID)
{
    return (StatusType)((PlayersManager *)DS)->ReplaceGroup(GroupID, ReplacementID);
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease)
{
    return (StatusType)((PlayersManager *)DS)->IncreaseLevel(PlayerID, LevelIncrease);   
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID)
{
    return (StatusType)((PlayersManager *)DS)->GetHighestLevel(GroupID, PlayerID);
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers)
{
    return (StatusType)((PlayersManager *)DS)->GetAllPlayersByLevel(GroupID, Players, numOfPlayers);   
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players)
{
    return (StatusType)((PlayersManager *)DS)->GetGroupsHighestLevel(numOfGroups, Players);
}

void Quit(void** DS)
{
    delete (*(PlayersManager **)DS);
    *DS = nullptr;
}