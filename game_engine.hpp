//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include "rank.hpp"
#include "game.hpp"
#include "player.hpp"
#include "game_session.hpp"
#include "inactive_reciver.hpp"
#include "inactive_detection.hpp"


#include <map>
#include <string>
#include <memory>

namespace RPSGame
{

class GameEngine : public GameInterface, public InactiveReciver<PlayerId>
{
public:
    GameEngine();

    // Implement Game interface
    void rank(std::multimap<uint64_t, std::string>& rank) override;
    bool createPlayer(std::string& player_id) override;
    bool move(const std::string& player_id, const std::string& action) override;
    bool events(const std::string& player_id, std::list<std::string>& events) override;

protected:
    void inactive(const std::set<PlayerId>& inactive) override;
    std::shared_ptr<GameSession> createGame();
    void checkResult(GameSession* game_session, Player* player1, Player* player2);
    
private:
//    std::mutex m_games_lock;
//    std::map<GameId, std::shared_ptr<GameSession> > m_games;

    std::mutex m_players_lock;
    std::map<PlayerId, std::unique_ptr<Player> > m_players;
    std::shared_ptr<GameSession> m_last_game;

    Rank m_rank;
    InactiveDetection<PlayerId> m_detector;
};

}
