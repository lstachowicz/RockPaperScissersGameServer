//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#pragma once

#include <string>
#include <stdint.h>

namespace RPSGame {

enum class Action : uint8_t {
    Unknown  = 0,
    Paper    = 1,
    Rock     = 2,
    Scissors = 4,
    Lizard   = 8,
    Spock    = 16
};

Action fromString(const std::string& action);

}
