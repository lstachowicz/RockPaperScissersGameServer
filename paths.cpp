//
//  Created by Łukasz Stachowicz on 01/09/2022.
//


#include "paths.hpp"

const std::filesystem::path RPSGame::PLAYER_DATA_DIR("players");
const std::filesystem::path RPSGame::RANK_FILE {"rank.txt"};

void RPSGame::createPlayerDir()
{
    if (!std::filesystem::exists(PLAYER_DATA_DIR))
    {
        std::filesystem::create_directory(PLAYER_DATA_DIR);
    }
}
