
#ifndef WORLDCUP23A2_CPP_PLAYERDATA_H
#define WORLDCUP23A2_CPP_PLAYERDATA_H
#include "wet2util.h"

class TeamsData;

class PlayerData{
private:
    int gamesPlayed;
    int cardsReceived;
    int ability;
    permutation_t spirit;
    bool headInTeam;
    int playerId;
    TeamsData* teamBelongTo;
    PlayerData* father;
    permutation_t overAllSpirit;
public:
    PlayerData(): gamesPlayed(0),
                   cardsReceived(0),
                   ability(0),
                   headInTeam(false),
                   playerId(0),
                   teamBelongTo(nullptr),
                   father(nullptr){}
    PlayerData(int playerID, const permutation_t &spiritx, int games_Played,
                int Ability, int cards):
            gamesPlayed(games_Played),
            cardsReceived(cards),
            ability(Ability),
            spirit(spiritx),
            headInTeam(false),
            playerId(playerID),
            teamBelongTo(nullptr),
            father(nullptr){}
    int getPlayerId() const { return this->playerId;}
    int getGamesPlayed() const {return this->gamesPlayed;}
    int getAbility() const {return this->ability;}
    int getCards() const {return this->cardsReceived;}
    bool isHeadInTeam() const {return this->headInTeam;}
    TeamsData* getTeamBelongTO() const {return this->teamBelongTo;}
    PlayerData* getFather() const { return this->father;}
    permutation_t getSpirit()const { return this->spirit;}
    permutation_t getOverAllSpirit()const {return this->overAllSpirit;}
    void incOverAllSpirit(permutation_t s){
        this->overAllSpirit= this->overAllSpirit*s;
    }
    void incOverAllSpiritFromBack(permutation_t s){
        this->overAllSpirit= s*this->overAllSpirit;
    }
    void setFather(PlayerData* headIn_Team) { this->father = headIn_Team;}
    void setTeamBelongTO(TeamsData* teamBelong_TO) { this->teamBelongTo = teamBelong_TO;}
    void setSpirit( permutation_t s) {this->spirit = s;}
    void setOverAllSpirit( permutation_t s) {this->overAllSpirit =s;}
    void setGamesPlayed(int games) {this->gamesPlayed = games;}
    void addCards(int cards){
        this->cardsReceived += cards;
    }
    void setCaptain(){
        this->headInTeam = true;
    }
    void turnOffCaptain(){
        this->headInTeam = false;
    }
    void teamPlayed(){
        this->gamesPlayed++;
    }
    void pushSpirit(permutation_t s)
    {
        this->spirit= this->spirit*s;
    }
    void pushSpiritFromBack(permutation_t s)
    {
        this->spirit= s*this->spirit;
    }

};



#endif //WORLDCUP23A2_CPP_PLAYERDATA_H
