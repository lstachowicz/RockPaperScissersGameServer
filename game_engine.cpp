//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "game_engine.hpp"
#include "paths.hpp"
#include "messages.hpp"

#include <map>
#include <string>
#include <mutex>

using namespace RPSGame;

GameEngine::GameEngine()
    : m_detector(this)
{
    // Initialize it outside of that class as it is not responsible for game initialization
    RPSGame::createPlayerDir();

    m_rank.start();
    m_detector.start();
}

void GameEngine::rank(std::multimap<uint64_t, std::string>& rank)
{
    m_rank.rank(rank);
}

bool GameEngine::createPlayer(std::string& player_hash)
{
    if (!player_hash.empty())
    {
        PlayerId player_id{player_hash};
        std::scoped_lock<std::mutex> lock(m_players_lock);
        auto it = m_players.find(player_id);
        if (it != m_players.end())
        {
            return true;
        }
    }

    std::unique_ptr<Player> player;
    std::string hash;
    if (player_hash.empty())
    {
        player.reset(new Player());
        hash = { player->getId().toString() };
    }
    else
    {
        player.reset(new Player(player_hash));
        hash = player_hash;
    }
    
    std::shared_ptr<GameSession> game_session = createGame();

    m_detector.update(player->getId());
    if (player->setGame(game_session))
    {
        player->addEvent(joinMessage(game_session->getId().toString()));
        player->addEvent(yourTurn());
        std::scoped_lock<std::mutex> lock(m_players_lock);
        m_players[player->getId()].reset(player.release());
    }

    player_hash = hash;
    return !player_hash.empty();
}

std::shared_ptr<GameSession> GameEngine::createGame()
{
     std::shared_ptr<GameSession> game_session;
     if (m_last_game)
     {
         game_session = m_last_game;
         m_last_game.reset();
     }
     else
     {
         game_session.reset(new GameSession());
         m_last_game = game_session;
     }

     return game_session;
}

bool GameEngine::move(const std::string& player_id, const std::string& action)
{
    PlayerId player(player_id);
    Action game_action(fromString(action));

    if (player_id.empty() || player.isNull() || game_action == Action::Unknown)
    {
        return false;
    }

    m_detector.update(player);

    {
        Player* player1 { nullptr };
        std::lock_guard<std::mutex> lock(m_players_lock);
        auto it = m_players.find(player);
        if (it == m_players.end())
        {
            return false;
        }

        player1 = it->second.get();
        if (!it->second->game()->move(player, game_action))
        {
            return false;
        }
        else
        {
            it->second->addEvent(RPSGame::selectedMessage(action));
        }


        PlayerId player1_id;
        PlayerId player2_id;
        it->second->game()->players(player1_id, player2_id);
        it = m_players.find(player == player1_id ? player2_id : player1_id);
        if (it != m_players.end())
        {
            checkResult(it->second->game().get(), player1, it->second.get());
        }
    }

    return true;
}

void GameEngine::checkResult(GameSession* game_session, Player* player1, Player* player2)
{
    if (game_session == nullptr || player1 == nullptr || player2 == nullptr)
    {
        return;
    }

    if (game_session->gotResult())
    {
        PlayerId winner, looser;
        std::string message;
        auto result = game_session->getResult(winner, looser, message);
        game_session->clear();

        if (result == GameResult::End)
        {
            Player* player_winner = (winner == player1->getId() ? player1 : player2);
            Player* player_looser = (looser == player1->getId() ? player1 : player2);

            player_winner->addWins();
            m_rank.update({player_winner->getId(), player_winner->wins()});

            player_winner->addEvent(RPSGame::winnerMessage(message));
            player_looser->addEvent(RPSGame::looserMessage(message));
            player_winner->addEvent(RPSGame::yourTurn());
            player_looser->addEvent(RPSGame::yourTurn());
        }
        else if (result == GameResult::Remis)
        {
            auto addEventsTpPlayer = [](Player* player) {
                if (player == nullptr)
                {
                    return;
                }
                player->addEvent(RPSGame::remisMessage());
                player->addEvent(RPSGame::yourTurn());
            };

            addEventsTpPlayer(player1);
            addEventsTpPlayer(player2);
        }
    }
}

bool GameEngine::events(const std::string& player_id, std::list<std::string>& events)
{
    PlayerId player(player_id);

    std::scoped_lock<std::mutex> lock(m_players_lock);
    auto it = m_players.find(player);
    if (it != m_players.end())
    {
        m_detector.update(player);
        it->second->events(events);
        return true;
    }

    return false;
}

void GameEngine::inactive(const std::set<PlayerId>& inactive)
{
    {
        std::list<PlayerId> players;
        for (auto it = inactive.begin(); it != inactive.end(); ++it)
        {
            {
                std::scoped_lock<std::mutex> lock(m_players_lock);
                auto jt = m_players.find(*it);
                if (jt != m_players.end())
                {
                    // Remove player from the game
                    jt->second->game()->removePlayer(jt->first);

                    // Find another player that is stil in game
                    PlayerId player1, player2;
                    jt->second->game()->players(player1, player2);
                    players.push_front(player1.isNull() ? player2 : player1);

                    // remove player
                    m_players.erase(jt);
                }
            }
        }

        std::scoped_lock<std::mutex> lock(m_players_lock);
        for (auto it = players.begin(); it != players.end(); ++it)
        {
            auto jt = m_players.find(*it);
            if (jt != m_players.end())
            {
                jt->second->addEvent(leftGame());
                jt->second->setGame(createGame());
                jt->second->addEvent(joinMessage(jt->second->game()->getId().toString()));
                jt->second->addEvent(yourTurn());
            }
        }
    }
}
