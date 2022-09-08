//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#pragma once

#include <string>

namespace RPSGame
{
    const std::string& yourTurn();
    const std::string& leftGame();
    const std::string& remisMessage();
    std::string joinMessage(const std::string& message);
    std::string winnerMessage(const std::string& message);
    std::string looserMessage(const std::string& message);
    std::string selectedMessage(const std::string& message);
}

