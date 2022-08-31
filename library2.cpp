//
//  library2.cpp
//  WET2
//
//  Created by Amr Dacca on 29/12/2021.
//

#include <stdio.h>
#include "library2.h"
#include "PlayersManager.h"


void *Init(int k, int scale){
    if (k<=0 || scale >200 || scale<=0) {
        return NULL;
    }
    try {
        PlayersManager *DS = new PlayersManager(k,scale);
        return (void*)DS;
    }
    catch (...) {
        return NULL;
    }
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->MergeGroups(GroupID1, GroupID2);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->AddPlayer(PlayerID, GroupID, score);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->RemovePlayer(PlayerID);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->IncreasePlayerIDLevel(PlayerID,LevelIncrease);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->ChangePlayerIDScore(PlayerID,NewScore);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,double * players){
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->GetPercentOfPlayersWithScoreInBounds(GroupID,score,lowerLevel,higherLevel,players);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level)
{
    if(!DS){
        return INVALID_INPUT;
    }
    try {
        StatusType res = (StatusType) ((PlayersManager *) DS)->AverageHighestPlayerLevelByGroup(GroupID,m,level);
        return res;
    }
    catch (...) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,int * LowerBoundPlayers, int * HigherBoundPlayers){
    return SUCCESS;
}

void Quit(void** DS){
    delete (PlayersManager*)*DS;
    *DS=NULL;
    return;
}
