//
//  PlayersManager.h
//  WET2
//
//  Created by Amr Dacca on 29/12/2021.
//

#ifndef PlayersManager_h
#define PlayersManager_h
#include "HashTable.h"
#include "linkedlist.h"
#include "RankAVL.h"
typedef enum {
    SUCCESS_cpp = 0,
    FAILURE_cpp = -1,
    ALLOCATION_ERROR_cpp = -2,
    INVALID_INPUT_cpp = -3
} StatusType_cpp;

class PlayerData;
class PlayerAVLKey;
class GroupData;
typedef hashTable<PlayerData> PlayerHash;
typedef RankAVL<PlayerAVLKey, int> LevelPlayers;




template<class Key,class Data>
void mergeTreeArrays(Key arr1[], Key arr2[], int n1,int n2, Key combined[]
                     ,Data data1[], Data data2[],  Data combined_data[])
{
    int i = 0, j = 0, k = 0;
    while (i<n1 && j <n2)
    {
        if (arr1[i] < arr2[j]) {
            combined[k] = arr1[i];
            combined_data[k++] = data1[i++];
        }
        else{
            combined_data[k] = data2[j];
            combined[k++] = arr2[j++];
        }
    }
    while (i < n1){
        combined[k] = arr1[i];
        combined_data[k++] = data1[i++];
    }
    while (j < n2){
        combined_data[k] = data2[j];
        combined[k++] = arr2[j++];
    }
}


class ScoreArrayData{
public:
    PlayerHash zero_level_hash;
    LevelPlayers level_avl;
    
    ScoreArrayData():zero_level_hash(PlayerHash()),level_avl(LevelPlayers()){};
    ~ScoreArrayData(){};
    ScoreArrayData(const ScoreArrayData& other)=delete;
};







class GroupData{
public:
    PlayerHash group_zero_level_hash;
    ScoreArrayData* group_scores_array;
    LevelPlayers group_players_avl;
    
    GroupData(int max):group_zero_level_hash(PlayerHash()),group_scores_array(new ScoreArrayData[max+1]),group_players_avl(LevelPlayers()){}
    ~GroupData(){
        delete[] group_scores_array;
    }
    
};



class PlayerData{
public:
    int score;
    int group_id;
    int level;
    
    PlayerData();
    PlayerData(int score,int groupid,int level):score(score),group_id(groupid),level(level){};
    ~PlayerData()=default;
    PlayerData(const PlayerData& other):score(other.score),group_id(other.group_id),level(other.level){};
    PlayerData& operator=(const PlayerData& other)=default;
    
    
};


class PlayerAVLKey{
public:
    int level;
    int player_id;
    
    PlayerAVLKey()=default;
    PlayerAVLKey(int level,int id):level(level),player_id(id){};
    ~PlayerAVLKey()=default;
    PlayerAVLKey(const PlayerAVLKey& other):level(other.level),player_id(other.player_id){};
    PlayerAVLKey& operator=(const PlayerAVLKey& other)=default;
    bool operator==(const PlayerAVLKey& other){
        return ((level==other.level)&&(player_id==other.player_id));
    }
    bool operator<(const PlayerAVLKey& other){
        if(level < other.level){
            return true;
        }
        if(level== other.level &&  player_id < other.player_id){
            return true;
        }
        return false;
    }
    bool operator>(const PlayerAVLKey& other){
        if(level > other.level){
            return true;
        }
        
        if(level== other.level && player_id > other.player_id){
            return true;
        }
        return false;
    }
};


class PlayersManager{
public:
    int* sizes;
    int* parents;
    GroupData**  groups_array;
    ScoreArrayData* main_scores_array;
    LevelPlayers    main_avl;
    PlayerHash      players;
    PlayerHash      main_zero_level_hash;
    int k;
    int scale;
    
    
    PlayersManager()=delete;
    PlayersManager(int k, int scale):sizes(new int[k+1]),parents(new int[k+1]),groups_array(new GroupData*[k+1]()),main_scores_array(new ScoreArrayData[scale+1]),main_avl(LevelPlayers()),players(PlayerHash()),main_zero_level_hash(PlayerHash()),k(k),scale(scale){
        
        for (int i=0; i<=k; i++) {
            sizes[i]=1;
            parents[i]=-1;
            groups_array[i] = new GroupData(scale);
            for (int j=0; j<=scale; j++) {
                groups_array[i]->group_scores_array[j].level_avl.size=0;
            }
        }
    }
    
    
    
    ~PlayersManager(){
        for (int i=0; i<k+1; i++) {
            delete groups_array[i];
        }
        delete [] sizes;
        delete [] parents;
        delete [] groups_array;
        delete [] main_scores_array;
    }
    int find(int group_id);
    void Union(int group_id1,int group_id2);
    StatusType_cpp MergeGroups(int GroupID1, int GroupID2);
    StatusType_cpp AddPlayer(int PlayerID, int GroupID, int score);
    StatusType_cpp RemovePlayer(int PlayerID);
    StatusType_cpp IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType_cpp ChangePlayerIDScore(int PlayerID, int NewScore);
    
    StatusType_cpp GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,double * players);
    
    StatusType_cpp AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
    
    void mergeGroupsData(GroupData* dest,GroupData* source);
};


#endif /* PlayersManager_h */
