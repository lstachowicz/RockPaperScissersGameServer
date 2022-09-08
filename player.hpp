//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include "player_id.hpp"
#include "interface_id.hpp"
#include "game_session.hpp"

#include <list>
#include <string>
#include <fstream>
#include <filesystem>

namespace RPSGame
{

class Player : public InterfaceId<PlayerId>
{
public:
    explicit Player();
    explicit Player(const std::string& hash);
    ~Player();

    inline std::shared_ptr<GameSession> game() { return m_game_session; }
    bool setGame(std::shared_ptr<GameSession> game_session);

    inline void addEvent(const std::string& event) { m_events.push_back(event); }
    inline void events(std::list<std::string>& events) { m_events.swap(events); }

    inline uint64_t wins() const { return m_wins; }
    inline void addWins() { ++m_wins; }

protected:
    bool load();
    void save();

    std::filesystem::path filePath() const;
    
private:
    std::list<std::string> m_events;

    uint64_t m_wins;
    std::shared_ptr<GameSession> m_game_session;
};

}
