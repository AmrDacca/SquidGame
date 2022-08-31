//
//  PlayersManager.cpp
//  WET2
//
//  Created by Amr Dacca on 29/12/2021.
//

#include "PlayersManager.h"
#include <climits>

void mergeTrees(LevelPlayers& dest,LevelPlayers& source);
static void sumOfLevelTillNode(LevelPlayers::Node r,PlayerAVLKey Key,int& sum);



int PlayersManager::find(int group_id){
    int current_group=group_id;
    while (parents[current_group] !=-1) {
        current_group = parents[current_group];
    }
    while (parents[group_id]!=-1) {
        int tmp_parent = parents[group_id];
        parents[group_id]=current_group;
        group_id = tmp_parent;
    }
    return current_group;
}





void PlayersManager::Union(int group_id1,int group_id2){
    int parent1=find(group_id1),parent2=find(group_id2);
    if (parent1 == parent2) {
        return;
    }
    //parent1 <------ parent2
    if (sizes[parent1] >= sizes[parent2]) {
        sizes[parent1]+=sizes[parent2];
        sizes[parent2]=-1;
        parents[parent2]=parent1;
        mergeGroupsData(groups_array[parent1], groups_array[parent2]);
        delete groups_array[parent2];
        groups_array[parent2]=nullptr;
        
    }
    else{ //parent2 <------parent1
        sizes[parent2]+=sizes[parent1];
        sizes[parent1]=-1;
        parents[parent1]=parent2;
        mergeGroupsData(groups_array[parent2],groups_array[parent1]);
        delete groups_array[parent1];
        groups_array[parent1]=nullptr;
    }
    return;
}


StatusType_cpp PlayersManager::MergeGroups(int GroupID1, int GroupID2){
    if (GroupID1<=0 || GroupID2 <=0 || GroupID1 > k || GroupID2 > k) {
        return INVALID_INPUT_cpp;
    }
    Union(GroupID1, GroupID2);
    return SUCCESS_cpp;
}



StatusType_cpp PlayersManager::AddPlayer(int PlayerID, int GroupID, int score){
    //checking input var's
    if(GroupID <=0 || GroupID>k || PlayerID <=0 || score <=0 || score > scale){
        return INVALID_INPUT_cpp;
    }
    //checking the player already exists
    if (players.isContain(PlayerID)) {
        return FAILURE_cpp;
    }
    //finding the group parent
    int group = find(GroupID);
    
    //adding the player to the players hash
    players.insert(PlayerID, PlayerData(score,GroupID,0));
    
    ///ADDING PLAYER TO THE PLAYERS HASH WITH LEVEL ZERO
    main_zero_level_hash.insert(PlayerID, PlayerData(score,GroupID,0));
    
    //ADDING THE PLAYER TO THE SPECEIFIC MAIN  SCORES ARRAY
    main_scores_array[score].zero_level_hash.insert(PlayerID, PlayerData(score,GroupID,0));
    //GETTING THE GROUP DATA
    GroupData* group_data = groups_array[group];
    
    //ADDING THE PLAYER TO THE GROUP PLAYERS ZERO LEVEL HASH
    group_data->group_zero_level_hash.insert(PlayerID, PlayerData(score,GroupID,0));
    
    //ADDING THE PLAYER  TO THE HASH OF A SPECIFIC SCORE
    group_data->group_scores_array[score].zero_level_hash.insert(PlayerID, PlayerData(score,GroupID,0));
    
    return SUCCESS_cpp;
}





StatusType_cpp PlayersManager::RemovePlayer(int PlayerID){
    //CHECKING INPUT Var's
    if (PlayerID<=0) {
        return INVALID_INPUT_cpp;
    }
    if (!players.isContain(PlayerID)) {
        return FAILURE_cpp;
    }
    //getting the player data
    PlayerData player_data = players.getData(PlayerID);
    
    //getting the parent group
    int group = find(player_data.group_id);
    
    //Getting the group data
    GroupData* group_data = groups_array[group];
    
    //checking if the player has level
    if (player_data.level == 0) {
        //removing from the main zero level hash
        main_zero_level_hash.remove(PlayerID);
        //removing from the specific score level zero hash
        main_scores_array[player_data.score].zero_level_hash.remove(PlayerID);
        //removing the player from the group zero level hash
        group_data->group_zero_level_hash.remove(PlayerID);
        //removing the player from the specific score in the group
        group_data->group_scores_array[player_data.score].zero_level_hash.remove(PlayerID);
    }
    else{
        PlayerAVLKey tmp_key(player_data.level,PlayerID);
        //removing from the level main tree
        main_avl.removeNode(tmp_key);
        //removing from the specific score level avl tree
        main_scores_array[player_data.score].level_avl.removeNode(tmp_key);
        
        //removing the player in the groups level tree
        group_data->group_players_avl.removeNode(tmp_key);
        
        //removing from the specific score tree
        group_data->group_scores_array[player_data.score].level_avl.removeNode(tmp_key);
        
    }
    //removing from the main players hash.
    players.remove(PlayerID);
    return SUCCESS_cpp;
}




StatusType_cpp PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease){
    if (LevelIncrease<=0 || PlayerID<= 0) {
        return INVALID_INPUT_cpp;
    }
    if(!players.isContain(PlayerID)){
        return FAILURE_cpp;
    }
    //getting the player data
    PlayerData player_data = players.getData(PlayerID);
    
    //getting the parent group
    int group = find(player_data.group_id);
    
    //Getting the group data
    GroupData* group_data = groups_array[group];
    
    if (player_data.level == 0) {
        //removing from the main zero level hash
        main_zero_level_hash.remove(PlayerID);
        //removing from the specific score level zero hash
        main_scores_array[player_data.score].zero_level_hash.remove(PlayerID);
        //removing the player from the group zero level hash
        group_data->group_zero_level_hash.remove(PlayerID);
        //removing the player from the specific score in the group
        group_data->group_scores_array[player_data.score].zero_level_hash.remove(PlayerID);
        
    }
    else{
        PlayerAVLKey tmp_key(player_data.level,PlayerID);
        //removing from the level main tree
        main_avl.removeNode(tmp_key);
        //removing from the specific score level avl tree
        main_scores_array[player_data.score].level_avl.removeNode(tmp_key);
        
        //removing the player in the groups level tree
        group_data->group_players_avl.removeNode(tmp_key);
        
        //removing from the specific score tree
        group_data->group_scores_array[player_data.score].level_avl.removeNode(tmp_key);
    }
    //increasing the player level
    players.increaseLevel(PlayerID, LevelIncrease);
    
    //getting the player_data
    player_data = players.getData(PlayerID);
    
    //creating the new player key for the avl
    PlayerAVLKey new_player_key(player_data.level,PlayerID);
    
    //inserting in the main level avl
    main_avl.insertNode(new_player_key, new_player_key.level);
    
    //inserting the main specific score tree
    main_scores_array[player_data.score].level_avl.insertNode(new_player_key,  new_player_key.level);
    
    //inserting in the group players level avl
    group_data->group_players_avl.insertNode(new_player_key, new_player_key.level);
    
    //inserting in the group specific score players level avl
    group_data->group_scores_array[player_data.score].level_avl.insertNode(new_player_key, new_player_key.level);
    
    return SUCCESS_cpp;
}





    StatusType_cpp PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore){
        if(PlayerID<=0 || NewScore<=0 || NewScore > scale){
            return INVALID_INPUT_cpp;
        }
        if(!players.isContain(PlayerID)){
            return FAILURE_cpp;
        }
        //getting the player data
        PlayerData player_data = players.getData(PlayerID);
        
        //getting the parent group
        int group = find(player_data.group_id);
        
        //Getting the group data
        GroupData* group_data = groups_array[group];
        
        //saving old score for move from diff hashes/avl
        int old_score = player_data.score;
        
        players.setScore(PlayerID, NewScore);
        
        player_data = players.getData(PlayerID);

        //if nothing needs to be changed
        if (old_score == NewScore) {
            return SUCCESS_cpp;
        }
        //removing from hashes
        if(player_data.level == 0){
            //removing from the specific score hash
            main_scores_array[old_score].zero_level_hash.remove(PlayerID);
            //adding the player to new place of his specific score
            main_scores_array[NewScore].zero_level_hash.insert(PlayerID, player_data);
            
            //removing from the group specific score hash
            group_data->group_scores_array[old_score].zero_level_hash.remove(PlayerID);
            //
            group_data->group_scores_array[NewScore].zero_level_hash.insert(PlayerID, player_data);
            return SUCCESS_cpp;
        }
        
        PlayerAVLKey player_key(player_data.level,PlayerID);
        
        //removing from the specific scores array
        main_scores_array[old_score].level_avl.removeNode(player_key);
        
        //adding to the specific scores array with the new score
        main_scores_array[NewScore].level_avl.insertNode(player_key, player_key.level);
        
        //removing from the group specific score
        group_data->group_scores_array[old_score].level_avl.removeNode(player_key);
        
        //adding to the group specific score with the new score
        group_data->group_scores_array[NewScore].level_avl.insertNode(player_key, player_key.level);
        return SUCCESS_cpp;

    }

void PlayersManager::mergeGroupsData(GroupData* dest,GroupData* source){
    //FIRST WE UPDATE THE TREE AND THE HASHES OF THE GROUPDATAS
    mergeTrees(dest->group_players_avl, source->group_players_avl);
    
    dest->group_zero_level_hash.UnionHash(source->group_zero_level_hash);
    
    //updating the DATA in the arrays!!!!!
    for (int i=1; i<=scale; i++) {
        mergeTrees(dest->group_scores_array[i].level_avl, source->group_scores_array[i].level_avl);
        dest->group_scores_array[i].zero_level_hash.UnionHash(source->group_scores_array[i].zero_level_hash);
        
    }
}



void mergeTrees(LevelPlayers& dest,LevelPlayers& source){
    //dest tree data is enough
    if (source.size == 0) {
        return;
    }
    //dest tree needs to be updates
    if (dest.size == 0) {
        //creating keys for the new tree
        PlayerAVLKey* tmp_keys = new PlayerAVLKey[source.size]();
        int* tmp_data = new int[source.size]();
        
        //making the source tree into sorted arrays
        source.RankAVLtoArray(tmp_keys, tmp_data, source.min_node);
        
        //turning the array into a tree
        LevelPlayers::Node new_root = source.sortedArrayToAVL(tmp_keys, tmp_data, 0, source.size-1, nullptr);
        
        ///********BEAWARE OF MEMORY LEAK HERE*******************
        //updating the new tree data
        dest.root = new_root;
        dest.size = source.size;
        dest.updateAllNodesHeight(dest.root);
        dest.updateMin();
        dest.updateMax();
        
        delete [] tmp_data;
        delete [] tmp_keys;
        return;
        
    }
    else{//the two trees are full
        //creating keys for the source tree
        PlayerAVLKey* source_keys = new PlayerAVLKey[source.size]();
        int* source_data = new int[source.size]();
        //making the source tree into sorted array
        source.RankAVLtoArray(source_keys, source_data, source.min_node);
        
        //creating keys for the dest tree
        PlayerAVLKey* dest_keys = new PlayerAVLKey[dest.size]();
        int* dest_data = new int[dest.size]();
        
        //making the dest tree into sorted array
        dest.RankAVLtoArray(dest_keys, dest_data, dest.min_node);
        
        //creating the arrays for the NEW TREE to merge the data
        PlayerAVLKey* new_tree_keys = new PlayerAVLKey[dest.size+source.size]();
        int* new_tree_data = new int[dest.size+source.size]();
        
        int new_size = dest.size+source.size;
        //MERGING THE TWO ARRAYS
        mergeTreeArrays(dest_keys, source_keys, dest.size, source.size, new_tree_keys, dest_data, source_data, new_tree_data);
        
        
        //creating the new tree ROOT
        LevelPlayers::Node new_root = source.sortedArrayToAVL(new_tree_keys, new_tree_data, 0, source.size+dest.size-1, nullptr);
        
        //destroying the old tree!!!!! of the dest tree
        dest.destroyAVL(dest.root);
        
        //updating the root of the dest tree
        dest.root = new_root;
        
        //updating the new tree data
        dest.size = new_size;
        dest.updateAllNodesHeight(dest.root);
        dest.updateMax();
        dest.updateMin();
        
        //deleting the source arrays
        delete [] source_keys;
        delete [] source_data;
        
        //deleting the dest arrays
        delete [] dest_data;
        delete [] dest_keys;
        
        //deleting the new_tree arrays
        delete [] new_tree_data;
        delete [] new_tree_keys;
    }
}



StatusType_cpp PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,double * players){
    if (players == NULL || GroupID >k ||GroupID<0) {
        return INVALID_INPUT_cpp;
    }
    (*players)=0.00;
    if(score >scale || score<=0 ||(higherLevel<lowerLevel)){
        return FAILURE_cpp;
    }
    //creating keys for the trees
    PlayerAVLKey low(lowerLevel,-1),high(higherLevel,INT_MAX);
    double all_players=0;
    //the case we need to check from all players
    if (GroupID==0) {
        //inputing the lower bound into the tree
        main_avl.insertNode(low, lowerLevel);
        
        //inputing the higher bound to the tree
        bool flag1 = main_avl.insertNode(high, higherLevel);
        
        //using rank on the high and lower bound to find how many players are in that range
        all_players = main_avl.rank(high) - main_avl.rank(low) - 1 +(!flag1);
    
        //getting the tree with specific score
        LevelPlayers &level_specific_score_tree = main_scores_array[score].level_avl;
        
        //inserting the lower and higher in the tree
        level_specific_score_tree.insertNode(low, lowerLevel);
        bool flag2 = level_specific_score_tree.insertNode(high, higherLevel);
        
        //calculating how many players are in the range of low and high in the specific score
        *(players) = level_specific_score_tree.rank(high) - level_specific_score_tree.rank(low) - 1 +(!flag2);
        //if the lowerlevel <=0 we have to the add how many players has a level zero from the hash zeros
        if (lowerLevel <=0 && higherLevel >=0) {
            //adding the players with zero level from the main
            all_players+=main_zero_level_hash.elements;
            
            //adding how many players from the specfic score has level zero
            *(players)+= main_scores_array[score].zero_level_hash.elements;
            
        }
        //removing the nodes we inserted
        main_avl.removeNode(low);
        if (flag1) {//if the
            main_avl.removeNode(high);
        }
        level_specific_score_tree.removeNode(low);
        if(flag2){
            level_specific_score_tree.removeNode(high);
        }
        if (all_players==0) {
            return FAILURE_cpp;
        }
    }
    else{//case from a specific group.
        
        //getting the group data.
        GroupData* group_data = groups_array[find(GroupID)];
        
        //adding the high bound and low bound to all players tree i the group
        group_data->group_players_avl.insertNode(low, lowerLevel);
        bool flag1 = group_data->group_players_avl.insertNode(high, higherLevel);
        
        //getting how many players are in the given level range i the groups.
        all_players = group_data->group_players_avl.rank(high) - group_data->group_players_avl.rank(low) - 1 +(!flag1);
        
        //getting the tree from the specific score.
        LevelPlayers &group_specific_score_tree = group_data->group_scores_array[score].level_avl;
        
        //insertnig the bounds to the tree
        group_specific_score_tree.insertNode(low, lowerLevel);
        bool flag2 = group_specific_score_tree.insertNode(high, higherLevel);
        
        //calculating how many players between the given levels
        *(players) = group_specific_score_tree.rank(high) - group_specific_score_tree.rank(low) - 1 + (!flag2);
        
        
        if (lowerLevel <=0 && higherLevel >=0) {//incase we need to add the players who has level zero
            
            //adding the player will all levels zero
            all_players+= group_data->group_zero_level_hash.elements;
            //adding how many players with te specific score has zero levels
            *(players) += group_data->group_scores_array[score].zero_level_hash.elements;
        }
        //removing the added keys from the trees
        group_data->group_players_avl.removeNode(low);
        if (flag1) {
            group_data->group_players_avl.removeNode(high);
        }
        //removing from the score specific tree.
        group_specific_score_tree.removeNode(low);
        if (flag2) {
            group_specific_score_tree.removeNode(high);
        }
        if (all_players == 0) {
            return FAILURE_cpp;
        }
        
    }
    *(players) = ((*(players))/all_players)*100;
    return SUCCESS_cpp;
}




StatusType_cpp PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * avglevel){
    if (avglevel == NULL || GroupID<0 || GroupID > k || m <=0) {
        return INVALID_INPUT_cpp;
    }
    if (GroupID==0) {
        //checking if we have less than m players in the whole game
        if (m > players.elements) {
            return FAILURE_cpp;
        }
        //if we have at least one player of the m players in the zero hash so we summ all and divide by the number
        if (m > main_avl.size) {
            (*avglevel) = double(LevelPlayers::getSum(main_avl.root))/m;
            return SUCCESS_cpp;
        }
        //here we find the player of the the select n-m+1
        LevelPlayers::Node node_to_sum = main_avl.select(main_avl.size-m+1);
        int sum=0;
        //we sum over the search path for the wanted sum
        sumOfLevelTillNode(main_avl.root, node_to_sum->key, sum);
        (*avglevel) = (double)sum/m;
        return SUCCESS_cpp;
    }
    //finding the group parent
    int parent = find(GroupID);
    GroupData* group_data = groups_array[parent];
    
    //checking we have less than m players
    if (m > group_data->group_players_avl.size + group_data->group_zero_level_hash.elements) {
        return FAILURE_cpp;
    }
    if (m > group_data->group_players_avl.size) {
        (*avglevel) = (double)LevelPlayers::getSum(group_data->group_players_avl.root)/m;
        return SUCCESS_cpp;
    }
    LevelPlayers::Node node_to_sum = group_data->group_players_avl.select(group_data->group_players_avl.size-m+1);
    int sum=0;
    sumOfLevelTillNode(group_data->group_players_avl.root, node_to_sum->key, sum);
    (*avglevel) = (double)sum/m;
    return SUCCESS_cpp;
}


static void sumOfLevelTillNode(LevelPlayers::Node r,PlayerAVLKey Key,int& sum){
    if(!r){return;}
    if (r->key < Key) {
        sumOfLevelTillNode(r->right,Key,sum);
    }
    else if(r->key > Key){
        sum+=r->sum-LevelPlayers::getSum(r->left);
        sumOfLevelTillNode(r->left,Key,sum);
    }
    else if (r->key == Key) {
        sum+=r->data + LevelPlayers::getSum(r->right);
        return;
    }
}

