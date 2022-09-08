//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "action.hpp"

#include <map>

RPSGame::Action RPSGame::fromString(const std::string& action)
{
    static std::map<std::string, Action>  string_to_action = {
        {"unknown", Action::Unknown}
        , {"paper", Action::Paper}
        , {"rock", Action::Rock}
        , {"scissors", Action::Scissors}
        , {"lizard", Action::Lizard}
        , {"spock", Action::Spock}
    };

    return [&](const std::string& action) {
        std::string lower_action(action);
        std::transform(lower_action.begin(), lower_action.end()
                       , lower_action.begin()
                       , [](unsigned char c) { return std::tolower(c); });

        auto val = string_to_action.find(lower_action);
        if (val == string_to_action.end())
        {
            return Action::Unknown;
        }
        return val->second;
    }(action);
}
