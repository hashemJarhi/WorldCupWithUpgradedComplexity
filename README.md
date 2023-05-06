# WorldCupWithUpgradedComplexity

in this task we used hashtable with chainhashing and list and rank tree and UnionFind method to support those function complextites

N represents number of the players
K represents number of the teams

function complexties:

1)world_cup_t()- O(1)

2)virtual ~world_cup_t()- O(N+K)

3)add_team(int teamId) - O(logK)

4)remove_team(int teamId) - O(logK)

5)add_player(int playerId, int teamId, const permutation_t &spirit, int gamesPlayed,
                                             int ability, int cards, bool goalKeeper) - O(logK)
                                             
6)play_match(int teamId1, int teamId2) - O(logk)

7)num_played_games_for_player(int playerId) - O(log*N)

8)add_player_cards(int playerId, int cards) - O(log*N)

9)get_player_cards(int playerId) - O(1) on avearage for input

10)get_team_points(int teamId) - O(logK)

11)get_ith_pointless_ability(int i) -  O(logK)

12)get_partial_spirit(int playerId) - O(log*N)

13)buy_team(int buyerId, int boughtId) - O(logK + log*N)

provided main code for the input.
