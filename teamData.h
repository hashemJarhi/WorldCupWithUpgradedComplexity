
#ifndef WORLDCUP23A2_CPP_TEAMDATA_H
#define WORLDCUP23A2_CPP_TEAMDATA_H

class PlayerData;

class TeamsData{
private:
    int points;
    long long int playersAbilities;
    int playersNumber;
    int GoalKeepers;
    int teamID;
    PlayerData* headTeam;
public:
    TeamsData(): points(0),
                 playersAbilities(0),
                 playersNumber(0),
                 GoalKeepers(0),
                 teamID(0),
                 headTeam(nullptr)
    {}

    TeamsData(int ID): points(0),
                       playersAbilities(0),
                       playersNumber(0),
                       GoalKeepers(0),
                       teamID(ID),
                       headTeam(nullptr)
    {}


    int getTeamId()const{return this->teamID;}
    int getPoints() const {return this->points;}
    int getplayersAbilities() const {return this->playersAbilities;}
    int getPlayersNumber() const {return this->playersNumber;}
    int getTeamGoalKeeperNum() const {return this->GoalKeepers;}
    PlayerData* getHeadTeam() const {return this->headTeam;}


    void setPoints(int Points) { this->points = Points;}
    void setplayersAbilities(int playersAbilities) { this->playersAbilities = playersAbilities;}
    void setPlayersNumber(int PlayersNumber) { this->playersNumber = PlayersNumber;}
    void setTeamGoalKeeperNum(int goalKeeper) { this->GoalKeepers = goalKeeper;}
    void setHeadTeam(PlayerData* head){ this->headTeam = head; }

    void teamWon() {
        this->points = this->points +3;
    }

    void draw(){
        this->points = this->points +1;
    }

    void operator+=(TeamsData* t)
    {
        this->points += t->getPoints();
        this->playersNumber += t->getPlayersNumber();
        this->playersAbilities +=t->getplayersAbilities();
        this->GoalKeepers += t->getTeamGoalKeeperNum();
    }
    void addPlayer(){
            this->playersNumber++;
    }
    void addGoalKeeper(){
        this->GoalKeepers++;
    }
    void incAbilities(int playersAbilities) { this->playersAbilities += playersAbilities;}

};



#endif //WORLDCUP23A2_CPP_TEAMDATA_H
