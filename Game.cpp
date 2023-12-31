#include "Game.h"
#include <iostream>
#include "Globals.h"
#include "Input.h"
Game::Game()
{
    this->_window = NULL;
    this->_renderer = NULL;
    this->_quitFlag = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 || TTF_Init() < 0)
    {
        std::cout << "Couldn't Initialize. SDL_Eror :" << SDL_GetError();
    }
    else
    {

        SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, NULL, &_window, &_renderer);
        if (this->_window == NULL || this->_renderer == NULL)
        {
            std::cout << "Couldn't Create Window or Renderer. SDL_Error :" << SDL_GetError();
        }
        else
        {
            SDL_SetWindowTitle(this->_window, "Pong");

            player1 = Player(globals::SCREEN_WIDTH * globals::PLAYER_GAP_PERCENT, (globals::SCREEN_HEIGHT - globals::PLAYER_HEIGHT) / 2);
            player2 = Player(globals::SCREEN_WIDTH * (1 - globals::PLAYER_GAP_PERCENT) - globals::PLAYER_WIDTH, (globals::SCREEN_HEIGHT - globals::PLAYER_HEIGHT) / 2);

            score1 = Score(globals::SCREEN_WIDTH * 0.25, 50);
            score2 = Score(globals::SCREEN_WIDTH * 0.75, 50);

            // if everything initialised fine - start game Loop
            this->gameLoop();
        }

    }
}

Game::~Game()
{

}

void Game::draw()
{
    // Clears the backsurface
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->_renderer);

    // Draw every object on backsurface:

    // Draw central line
    SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(this->_renderer, globals::SCREEN_WIDTH / 2, 0, globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT);

    // Draw Players
    player1.draw(this->getRenderer());
    player2.draw(this->getRenderer());
    ball.draw(this->getRenderer());
    score1.draw(this->getRenderer());
    score2.draw(this->getRenderer());

    //Switch renderer with backsurface
    SDL_RenderPresent(this->_renderer);
}

void Game::update()
{

    //update players
    player1.update();
    player2.update();
    ball.update();

    //check for collision
    ball.collisionCheck(player1, player2);

    // check if someone wins
    if (ball.getX() < 0)
    {
        ball.resetBall();
        score1.increment();
    }

    if (ball.getX() + ball.getW() > globals::SCREEN_WIDTH)
    {
        ball.resetBall();
        score2.increment();
    }
}


void Game::gameLoop()
{
    SDL_Event event;
    Input input;

    while (!this->_quitFlag)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                _quitFlag = true;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                input.ButtonPressed(event.key.keysym.sym);
            }
            else if (event.type == SDL_KEYUP)
            {
                input.ButtonReleased(event.key.keysym.sym);
            }
        }

        // Controls
        if (input.IsKeyHeld(SDLK_UP))
        {
            player2.move(0, -globals::PLAYER_SPEED);
        }
        if (input.IsKeyHeld(SDLK_DOWN))
        {
            player2.move(0, globals::PLAYER_SPEED);
        }
        if (input.IsKeyHeld(SDLK_w))
        {
            player1.move(0, -globals::PLAYER_SPEED);
        }
        if (input.IsKeyHeld(SDLK_s))
        {
            player1.move(0, globals::PLAYER_SPEED);
        }




        this->update();
        this->draw();

    }
}