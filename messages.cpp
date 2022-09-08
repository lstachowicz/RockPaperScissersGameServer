//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#include "messages.hpp"

const std::string& RPSGame::yourTurn()
{
    static std::string message { "Your turn" };
    return message;
}

const std::string& RPSGame::leftGame()
{
    static std::string message { "Your opponent left the game" };
    return message;
}

const std::string& RPSGame::remisMessage()
{
    static std::string message { "Remis" };
    return message;
}

std::string RPSGame::joinMessage(const std::string& message)
{
    return "Joined to the game " + message;
}

std::string RPSGame::winnerMessage(const std::string& message)
{
    return "Won! " + message;
}

std::string RPSGame::looserMessage(const std::string& message)
{
    return "Lost! " + message;
}

std::string RPSGame::selectedMessage(const std::string& message)
{
    return "Selected " + message;
}
