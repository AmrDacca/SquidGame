# SquidGame

This project was written during the coursework Data structures in of which we implemented a text game of which given number k of groups
it adds players through text as explained below and uses data structures in of which Hashtabel,RankAVl Tree, AVl Tree, Union Find

Each player has an id ,current level , score , group id

The maximum score of the game is given at the start by the user.

Each group are differentiated by a group id in which each group contains a player 


# How the game works

To Initialze the game:
Init [k] [scale]
initializes the game with k groups and max score of scale
k is the number of groups , scale the maximum score for each player has to be between (0 , 200]
Invaild input in case of k<0 and scale is not in the range (0,200]

Commands to Enter:
(some of the commands groupid can be used as 0 then the infromation requested will be about all players from all the groups)

# ---MergeGroups <GroupId1> <GroupId2>---

merges the two groups into one group and all the players in the two groups are in the same group now

Every reference to any of the two groups it will be as refering to the merged group

FAILURE : GroupId1<=0 and/or GroupId2<=0 
or GroupId1>k and/or GroupIdk>k




## addPlayer <playerid> <groupid> <score>

adds a player with playerid to the group with group id with the given score and starts with level 1

returns:
INVALID_INPUT:

if groupdid >k or <= 0

if score > scale given at the start or < 0

if the player id <=0 

FALIURE: if A player with the same ID already exists.

increasePlayerIDLevel <PlayerID> <LevelIncrease>:

increases the player to a currentlevel + LevelIncrease

returns:
INVALID_INPUT:

if the player id <=0 

if the LevelIncrease <=0 

FALIURE: if A player with the same ID does not exist.

# ---changePlayerIDScore  <PlayerID> <NewScore>---

changes the player id score to newscore

returns:
INVALID_INPUT:

if the player id <=0 

if the NewScore <=0 or >scale

FALIURE: if A player with the same ID does not exist.

# ---getPercentOfPlayersWithScoreInBounds <GroupId> <score> <lowerLevel> <higherLevel>---

returns the precentage of players with the score equal to score that are between lowerlevel and higherlevel in the group with the given id

FALIURE: if the group doesnt have any players in the level range given.

INVALID_INPUT:

if groupdid >k or <= 0

# ---averageHighestPlayerLevelByGroup <GroupID> <m> <avgLevel>---

returns the average player level of the highest m players

INVALID_INPUT:

if groupdid >k or < 0

if m<=0

FALIURE: if the number of players in the group is smaller than m 


# ---Quit----
will terminate the game.


# How To Run

1.using this command to compile "g++ -std=c++11 -DNDEBUG -Wall *cpp -o game" after putting all the files into one folder

2 ./game

