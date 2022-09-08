//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "game.hpp"
#include "game_engine.hpp"

RPSGame::GameInterface* RPSGame::createGame()
{
    return new GameEngine();
}
