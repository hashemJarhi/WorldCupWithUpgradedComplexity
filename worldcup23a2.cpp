#include "worldcup23a2.h"

PlayerData* world_cup_t::findPlayerAndUnion(int playerID)
{
     PlayerData** playerNode = this->allPlayers->find(playerID);
    if(playerNode == nullptr ) return nullptr;
    if((*playerNode) == nullptr ) return nullptr;
    if((*playerNode)->getFather() == nullptr ||(*playerNode)->getFather()->getFather() == nullptr ) return *playerNode;
    PlayerData* player = *playerNode;
    PlayerData* head= player;
    PlayerData* playerToReturn = player;
    int PlayerGamePlayed =0;
    permutation_t oldSpirit = player->getSpirit();
    permutation_t playersSpirits = oldSpirit*(oldSpirit.inv());

    while(head->getFather() != nullptr)
    {
        playersSpirits = oldSpirit*playersSpirits;
        PlayerGamePlayed += head->getGamesPlayed();
        head =head->getFather();
        oldSpirit = head->getSpirit();
    }
    while(player->getFather() != head)
    {
        int oldGames = player->getGamesPlayed();
        player->setGamesPlayed(PlayerGamePlayed);
        permutation_t old = player->getSpirit();
        player->setSpirit(playersSpirits);
        playersSpirits =playersSpirits*old.inv();
        PlayerGamePlayed -= oldGames;
        PlayerData* tmp = player;
        player = player->getFather();
        tmp->setFather(head);
    }
    return playerToReturn;
}


world_cup_t::world_cup_t()
{
    allPlayers = new HashTable();
    avlTreeTeams = new Tree<int,TeamsData>();
    teamsByPower = new RankTree<teamSpecialID,TeamsData>();

}

world_cup_t::~world_cup_t()
{
	delete allPlayers;
    delete avlTreeTeams;
    delete teamsByPower;
}

StatusType world_cup_t::add_team(int teamId)
{
    if(teamId<=0) return StatusType::INVALID_INPUT;
    NodeInTree<int,TeamsData>* teamNode = avlTreeTeams->find(teamId);
    if(teamNode != nullptr) return StatusType::FAILURE;

    try{
        TeamsData* newTeam = new TeamsData(teamId);
        teamSpecialID* specialKeyTeam = new teamSpecialID(0,teamId);
        avlTreeTeams->insert(teamId,*newTeam);
        teamsByPower->insert(*specialKeyTeam,*newTeam);
        delete newTeam;
        delete specialKeyTeam;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    try{
    if(teamId<=0) return StatusType::INVALID_INPUT;
    NodeInTree<int,TeamsData>* teamNode = avlTreeTeams->find(teamId);
    if(teamNode == nullptr) return StatusType::FAILURE;
    TeamsData* currTeam = teamNode->data;
    if(currTeam->getPlayersNumber() >0)
    {
        PlayerData* headTeam = currTeam->getHeadTeam();
        headTeam->setTeamBelongTO(nullptr);
    }
    long long int ability = currTeam->getplayersAbilities();
    teamSpecialID* specialKeyTeam = new teamSpecialID(ability,teamId);
    avlTreeTeams->remove(teamId);
    teamsByPower->remove(*specialKeyTeam);
    delete specialKeyTeam;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;

}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId<=0 || teamId<=0 || gamesPlayed <0|| cards<0) return StatusType::INVALID_INPUT;
    if(!spirit.isvalid())return StatusType::INVALID_INPUT;
    try{
    PlayerData** player = allPlayers->find(playerId);
    NodeInTree<int,TeamsData>* teamNode = avlTreeTeams->find(teamId);
    if(teamNode == nullptr ||player != nullptr ) return StatusType::FAILURE;
    TeamsData* currTeam = teamNode->data;

    PlayerData* newPlayer = new PlayerData(playerId,spirit,gamesPlayed,ability,cards);
    teamSpecialID* specialKeyTeam = new teamSpecialID(currTeam->getplayersAbilities(),teamId);
    teamsByPower->remove(*specialKeyTeam);
    currTeam->incAbilities(ability);
    teamSpecialID* newSpecialKeyTeam = new teamSpecialID(currTeam->getplayersAbilities(),teamId);
    teamsByPower->insert(*newSpecialKeyTeam,*currTeam);
     allPlayers->insert(newPlayer);
    if(currTeam->getPlayersNumber() <=0 ){
        newPlayer->setOverAllSpirit(spirit);
        newPlayer->setSpirit(spirit);
        newPlayer->setTeamBelongTO(currTeam);
        newPlayer->setCaptain();
        newPlayer->setFather(nullptr);
        currTeam->setHeadTeam(newPlayer);
    }else{
        newPlayer->setTeamBelongTO(nullptr);
        PlayerData* teamCaptain = currTeam->getHeadTeam();
        newPlayer->setFather(teamCaptain);
        permutation_t oldOverAllSpirit = teamCaptain->getOverAllSpirit();
        newPlayer->pushSpiritFromBack(oldOverAllSpirit);
        newPlayer->pushSpiritFromBack((teamCaptain->getSpirit().inv()));
        int oldCaptainGames = teamCaptain->getGamesPlayed();
        int gamesToInsert = gamesPlayed -oldCaptainGames;
        newPlayer->setGamesPlayed(gamesToInsert);
        teamCaptain->incOverAllSpirit(spirit);
    }
        currTeam->addPlayer();
        if(goalKeeper) currTeam->addGoalKeeper();
        delete specialKeyTeam;
        delete newSpecialKeyTeam;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2)
    {
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }

    try{
        NodeInTree<int,TeamsData>* team1Node = avlTreeTeams->find(teamId1);
        NodeInTree<int,TeamsData>* team2Node = avlTreeTeams->find(teamId2);
        if(team1Node == nullptr || team2Node == nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        TeamsData* team1 = team1Node->data;
        TeamsData* team2 = team2Node->data;
        if(team1->getTeamGoalKeeperNum() <=0 || team2->getTeamGoalKeeperNum() <=0)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        long long int power1 = team1->getplayersAbilities() + team1->getPoints();
        long long int power2 = team2->getplayersAbilities() + team2->getPoints();
        PlayerData* captain1 = team1->getHeadTeam();
        PlayerData* captain2 = team2->getHeadTeam();
        permutation_t team1Spirit = captain1->getOverAllSpirit();
        permutation_t team2Spirit = captain2->getOverAllSpirit();
        int team1Strength = team1Spirit.strength();
        int team2Strength = team2Spirit.strength();
        captain1->teamPlayed();
        captain2->teamPlayed();
        static const int TEAM1_WON_BY_POWER = 1;
        static const int TEAM2_WON_BY_POWER = 3;
        static const int TEAM1_WON_BY_STRENGTH = 2;
        static const int TEAM2_WON_BY_STRENGTH = 4;
        static const int DRAW = 0;
        if(power1 > power2)
        {
            team1->teamWon();
            output_t<int> out = output_t<int>(TEAM1_WON_BY_POWER);
            return out;
        }else if(power2 > power1)
        {
            team2->teamWon();
            output_t<int> out = output_t<int>(TEAM2_WON_BY_POWER);
            return out;
        }else{
            if(team1Strength > team2Strength)
            {
                team1->teamWon();
                output_t<int> out = output_t<int>(TEAM1_WON_BY_STRENGTH);
                return out;
            }else if(team1Strength < team2Strength){
                team2->teamWon();
                output_t<int> out = output_t<int>(TEAM2_WON_BY_STRENGTH);
                return out;
            }else{
                team1->draw();
                team2->draw();
                return DRAW;
            }
        }
    }catch (bad_alloc& e){
        output_t<int> out = output_t<int>(StatusType::ALLOCATION_ERROR);
        return out;
    }
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if(playerId <= 0 )
    {
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }
	PlayerData* player = findPlayerAndUnion(playerId);
    if(player == nullptr)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }
    int games = player->getGamesPlayed();
    if(player->getFather() != nullptr)
    {
        games += player->getFather()->getGamesPlayed();
    }
    output_t<int> out = output_t<int>(games);
    return out;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards <0 )
    {
        return StatusType::INVALID_INPUT;
    }
    PlayerData* player = findPlayerAndUnion(playerId);
    if(player == nullptr) return StatusType::FAILURE;
    if(player->getFather() != nullptr)
    {
       if(player->getFather()->getTeamBelongTO() == nullptr) return StatusType::FAILURE;
    }else{
        if(player->getTeamBelongTO() == nullptr) return StatusType::FAILURE;
    }
    player->addCards(cards);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId <= 0)
    {
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }

    PlayerData** player = allPlayers->find(playerId);
    if(player == nullptr)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }
    int cards = (*player)->getCards();
    return cards;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    try{
        if(teamId <=0)
        {
            output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
            return out;
        }
        NodeInTree<int,TeamsData>* teamNode = avlTreeTeams->find(teamId);
        if(teamNode == nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        TeamsData* team = teamNode->data;
        if(team == nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }

        int points = team->getPoints();
        output_t<int> out = output_t<int>(points);
        return out;

    }catch (bad_alloc& e){
    output_t<int> out = output_t<int>(StatusType::ALLOCATION_ERROR);
    return out;
    }
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    try {
        int size = avlTreeTeams->getSize();
        if(i<0 || i>= size|| size <=0)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
       nodeInRankTree<teamSpecialID ,TeamsData>* teamNode = teamsByPower->getIndex(teamsByPower->getRoot(), i + 1);
        if(teamNode == nullptr) {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        TeamsData* team = teamNode->data;
        if(team == nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        int teamID = team->getTeamId();
        output_t<int> out = output_t<int>(teamID);
        return out;
    }catch (bad_alloc& e){
        output_t<int> out = output_t<int>(StatusType::ALLOCATION_ERROR);
        return out;
    }
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    try{
        if(playerId<=0) return StatusType::INVALID_INPUT;
        PlayerData* player = findPlayerAndUnion(playerId);
        if(player == nullptr) return StatusType::FAILURE;
        permutation_t spirit = player->getSpirit();
        if(player->getFather() != nullptr)
        {
            if(player->getFather()->getTeamBelongTO() == nullptr) return StatusType::FAILURE;
            spirit = (player->getFather()->getSpirit())*spirit;
        }else{
            if(player->getTeamBelongTO() == nullptr) return StatusType::FAILURE;
        }
        return spirit;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId2 <=0 || teamId1 <=0 || teamId2== teamId1) return StatusType::INVALID_INPUT;
    NodeInTree<int,TeamsData>* team1Node = avlTreeTeams->find(teamId1);
    NodeInTree<int,TeamsData>* team2Node = avlTreeTeams->find(teamId2);
    if(team1Node == nullptr || team2Node== nullptr) return StatusType::FAILURE;
    TeamsData* team1 = team1Node->data;
    TeamsData* team2 = team2Node->data;
    if(team1 == nullptr || team2== nullptr) return StatusType::FAILURE;
    teamSpecialID* specialId1old = new teamSpecialID(team1->getplayersAbilities(),teamId1);
    teamSpecialID* specialId2old = new teamSpecialID(team2->getplayersAbilities(),teamId2);
    int sizeBuyer = team1->getPlayersNumber();
    int sizeBought = team2->getPlayersNumber();
    if(sizeBought <=0)
    {
        teamsByPower->remove(*specialId2old);
        avlTreeTeams->remove(teamId2);
        delete specialId1old;
        delete specialId2old;
        return StatusType::SUCCESS;
    }else if(sizeBought>0 && sizeBuyer<=0){
        *team1 += team2;
        teamSpecialID* specialId1New = new teamSpecialID(team1->getplayersAbilities(),teamId1);
        PlayerData* boughtCaptain = team2->getHeadTeam();
        boughtCaptain->setTeamBelongTO(team1);
        team1->setHeadTeam(boughtCaptain);
        teamsByPower->remove(*specialId1old);
        teamsByPower->remove(*specialId2old);
        avlTreeTeams->remove(teamId2);
        teamsByPower->insert(*specialId1New,*team1);
        delete specialId1old;
        delete specialId2old;
        delete specialId1New;
        return StatusType::SUCCESS;
    }
    PlayerData* buyerCaptain = team1->getHeadTeam();
    PlayerData* boughtCaptain = team2->getHeadTeam();
    if(sizeBuyer >= sizeBought){
        boughtCaptain->setFather(buyerCaptain);
        boughtCaptain->setTeamBelongTO(nullptr);
        boughtCaptain->pushSpiritFromBack(buyerCaptain->getOverAllSpirit());
        boughtCaptain->pushSpiritFromBack(buyerCaptain->getSpirit().inv());
        buyerCaptain->incOverAllSpirit(boughtCaptain->getOverAllSpirit());
        int gamesToInsert = boughtCaptain->getGamesPlayed() - buyerCaptain->getGamesPlayed();
        boughtCaptain->setGamesPlayed(gamesToInsert);
    }else{
       boughtCaptain->incOverAllSpiritFromBack(buyerCaptain->getOverAllSpirit());
       boughtCaptain->pushSpiritFromBack(buyerCaptain->getOverAllSpirit());
       buyerCaptain->pushSpiritFromBack(boughtCaptain->getSpirit().inv());
       int gamesToInserts = buyerCaptain->getGamesPlayed()-boughtCaptain->getGamesPlayed();
        buyerCaptain->setGamesPlayed(gamesToInserts);
        buyerCaptain->setFather(boughtCaptain);
        boughtCaptain->setTeamBelongTO(team1);
        team1->setHeadTeam(boughtCaptain);
        buyerCaptain->setTeamBelongTO(nullptr);
    }
    *team1 += team2;
    avlTreeTeams->remove(teamId2);
    teamSpecialID* specialId1New = new teamSpecialID(team1->getplayersAbilities(),teamId1);
    teamsByPower->remove(*specialId1old);
    teamsByPower->remove(*specialId2old);
    teamsByPower->insert(*specialId1New,*team1);
    delete specialId1old;
    delete specialId2old;
    delete specialId1New;
	return StatusType::SUCCESS;
}
