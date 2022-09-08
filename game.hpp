//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include <map>
#include <list>
#include <string>

namespace RPSGame
{

class GameInterface
{
public:
    virtual ~GameInterface() = default;

    // Get global rank
    virtual void rank(std::multimap<uint64_t, std::string>& rank) = 0;

    // Create player
    virtual bool createPlayer(std::string& player_id) = 0;

    // Make player move
    virtual bool move(const std::string& player_id, const std::string& action) = 0;

    // Return player events
    virtual bool events(const std::string& player_id, std::list<std::string>& events) = 0;
};

GameInterface* createGame();

}
