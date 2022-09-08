//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "player.hpp"
#include "paths.hpp"

#include <uuid/uuid.h>
#include <string>
#include <fstream>
#include <filesystem>

using namespace RPSGame;

Player::Player()
    : InterfaceId<PlayerId>()
    , m_wins(0)
{
    if (!load())
        save();
}

Player::Player(const std::string& hash)
    : InterfaceId<PlayerId>(hash)
    , m_wins(0)
{
    if (!load())
        save();
}

Player::~Player()
{
    save();
}


bool Player::setGame(std::shared_ptr<GameSession> game_session)
{
    if (game_session->addPlayer(getId()))
    {
        m_game_session = game_session;
        return true;
    }
    return false;
}

std::filesystem::path Player::filePath() const
{
    return std::filesystem::path(PLAYER_DATA_DIR) /= getId().toString().data();
}

bool Player::load()
{
    auto file = std::ifstream(filePath().c_str());
    if (file.is_open())
    {
        file >> m_wins;
    }

    return false;
}

void Player::save()
{
    auto file = std::ofstream(filePath().c_str());
    if (file.is_open())
    {
        file << m_wins;
    }
}
