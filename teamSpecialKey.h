
#ifndef WORLDCUP23A2_CPP_TEAMSPECIALKEY_H
#define WORLDCUP23A2_CPP_TEAMSPECIALKE

class teamSpecialID{
private:
    long long int teamAbilitY;//#1
    int teamID;//#2
public:
    teamSpecialID() : teamAbilitY(0), teamID(0) {};
    teamSpecialID( long long int teamAbility, int teamID):
            teamAbilitY(teamAbility),
            teamID(teamID)
    {};

    long long int getAbility() const{return this->teamAbilitY;}
    int getTeamID() const{return this->teamID;}


    bool operator>(teamSpecialID& key) const
    {
        if(this->getAbility() > key.getAbility())
            return true;
        if(this->getAbility() == key.getAbility())
        {
            if(this->getTeamID() > key.getTeamID())
                 return true;
            return false;
        }
        return false;
    }

    bool operator==(teamSpecialID& key) const
    {
        return (this->getAbility() == key.getAbility()
                && this->getTeamID() == key.getTeamID());
    }

    bool operator<(teamSpecialID& key)const
    {
        return !((*this)>key) && !(*this==key);
    }

    bool operator<=(teamSpecialID& key)const
    {
        return (*this < key || *this == key);
    }

    bool operator>=(teamSpecialID& key)const
    {
        return (*this > key || *this == key);
    }


    bool operator!=(teamSpecialID& key)const
    {
        return !(*this==key);
    }

    void updateKey(teamSpecialID& key)
    {
        this->teamAbilitY = key.getAbility();
        this->teamID = key.getTeamID();
    }



};

#endif //WORLDCUP23A2_CPP_TEAMSPECIALKEY_H
