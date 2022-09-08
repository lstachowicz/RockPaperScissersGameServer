//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include "game_id.hpp"
#include "player_id.hpp"
#include "interface_id.hpp"
#include "action.hpp"

#include <list>

namespace RPSGame
{

enum class GameResult : u_int8_t {
    Unknwon = 0,
    End = 1,
    Remis = 2 & End
};

class GameSession : public InterfaceId<GameId>
{
public:
    GameSession();

    bool addPlayer(const PlayerId& player_id);
    void removePlayer(const PlayerId& player_id);
    void players(PlayerId& player1, PlayerId& player2);

    bool move(const PlayerId& player_id, Action action);
    void clear();
    
    bool gotResult() const;
    GameResult getResult(PlayerId& winner, PlayerId& looser, std::string& message) const;

private:
    std::pair<PlayerId, Action> m_player1;
    std::pair<PlayerId, Action> m_player2;
};

}
