//
//  Created by Łukasz Stachowicz on 03/09/2022.
//


#pragma once

#include <filesystem>

namespace RPSGame
{
    extern const std::filesystem::path PLAYER_DATA_DIR;
    extern const std::filesystem::path RANK_FILE;

    void createPlayerDir();
};
