//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "action.hpp"
#include "game_session.hpp"

using namespace RPSGame;


// Compare players m_player1 and m_player2 with action1 and action2
#define checkAction(action1, action2) \
    (static_cast<uint8_t>(m_player1.second) & (static_cast<uint8_t>(action1) | static_cast<uint8_t>(action2)) \
    && static_cast<uint8_t>(m_player2.second) & (static_cast<uint8_t>(action1) | static_cast<uint8_t>(action2)))

GameSession::GameSession()
    : m_player1(PlayerId(false), Action::Unknown)
    , m_player2(PlayerId(false), Action::Unknown)
{
}

bool GameSession::addPlayer(const PlayerId& player_id)
{
    if (player_id == m_player1.first || player_id == m_player2.first)
        return false;

    auto add_player = [&](PlayerId& id) {
        if (id.isNull()) {
            id = player_id;
            return true;
        }
        
        return false;
    };
    
    return add_player(m_player1.first) || add_player(m_player2.first);
}

void GameSession::removePlayer(const PlayerId& player_id)
{
    auto remove_player = [&](PlayerId& id) {
        if (!id.isNull() && id == player_id) {
            id = PlayerId(false);
            return true;
        }

        return false;
    };

    remove_player(m_player1.first) || remove_player(m_player2.first);
}

void GameSession::players(PlayerId& player1, PlayerId& player2)
{
    player1 = m_player1.first;
    player2 = m_player2.first;
}

bool GameSession::move(const PlayerId& player_id, Action move)
{
    PlayerId winner;
    PlayerId looser;
    auto make_move = [&](std::pair<PlayerId, Action>& player) -> bool {
        if (player.first == player_id && player.second == Action::Unknown)
        {
            player.second = move;
            
            return true;
        }
        
        return false;
    };
    
    return make_move(m_player1) || make_move(m_player2);
}

void GameSession::clear()
{
    m_player1.second = Action::Unknown;
    m_player2.second = Action::Unknown;
}

bool GameSession::gotResult() const
{
    return (m_player1.second != Action::Unknown && m_player2.second != Action::Unknown);
}

GameResult GameSession::getResult(PlayerId& winner, PlayerId& looser, std::string& message) const
{
    if (m_player1.second != Action::Unknown && m_player1.second == m_player2.second)
    {
        winner = m_player1.first;
        looser = m_player2.first;
        message = "Remis";
        return GameResult::Remis;
    }
    else if (checkAction(Action::Spock, Action::Scissors))
    {
        winner = (m_player1.second == Action::Spock ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Scissors ? m_player1.first : m_player2.first);
        message = "Spock smashes Scissors";
    }
    else if (checkAction(Action::Spock, Action::Rock))
    {
        winner = (m_player1.second == Action::Spock ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Rock ? m_player1.first : m_player2.first);
        message = "Spock vaporize Rock";
    }
    else if (checkAction(Action::Scissors, Action::Paper))
    {
        winner = (m_player1.second == Action::Scissors ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Paper ? m_player1.first : m_player2.first);
        message = "Scissors cut Paper";
    }
    else if (checkAction(Action::Scissors, Action::Lizard))
    {
        winner = (m_player1.second == Action::Scissors ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Lizard ? m_player1.first : m_player2.first);
        message = "Scissors decapitates Lizard";
    }
    else if (checkAction(Action::Paper, Action::Rock))
    {
        winner = (m_player1.second == Action::Paper ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Rock ? m_player1.first : m_player2.first);
        message = "Paper cover Rock";
    }
    else if (checkAction(Action::Paper, Action::Spock))
    {
        winner = (m_player1.second == Action::Paper ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Spock ? m_player1.first : m_player2.first);
        message = "Paper disproves Spock";
    }
    else if (checkAction(Action::Rock, Action::Scissors))
    {
        winner = (m_player1.second == Action::Rock ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Scissors ? m_player1.first : m_player2.first);
        message = "Rock crushes Scissors";
    }
    else if (checkAction(Action::Rock, Action::Lizard))
    {
        winner = (m_player1.second == Action::Rock ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Lizard ? m_player1.first : m_player2.first);
        message = "Rock crushes Lizard";
    }
    else if (checkAction(Action::Lizard, Action::Spock))
    {
        winner = (m_player1.second == Action::Lizard ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Spock ? m_player1.first : m_player2.first);
        message = "Lizard poisons Spock";
    }
    else if (checkAction(Action::Lizard, Action::Paper))
    {
        winner = (m_player1.second == Action::Lizard ? m_player1.first : m_player2.first);
        looser = (m_player1.second == Action::Paper ? m_player1.first : m_player2.first);
        message = "Lizard eats Paper";
    }
    else
    {
        return GameResult::Unknwon;
    }
    
    return GameResult::End;
}
