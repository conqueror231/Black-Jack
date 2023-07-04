﻿#include <SDL.h>
#include <SDL_image.h>
#include<iostream>
#include<list>
#include"PlayingCard.h"
#include"Deck.h"
#include"Bank.h"
#include<queue>
#include"Player.h"
#include"Dealer.h"
#include<thread>
#include"SDL_ttf.h"
#include<Windows.h>
#include<fstream>
#include <cstdlib>

SDL_Texture* ShowText(SDL_Color color, const char* text);

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int CARD_WIDTH = 100;
const int CARD_HEIGHT = 150;

SDL_Renderer* renderer = nullptr;
//Distanation For Dealer {}
SDL_Rect destinationRectForPlayer{ 300, 400, 150, 200 };
SDL_Rect destinationRectForDealer{ 300, 50, 150, 200 };

std::vector<std::pair<SDL_Rect, SDL_Rect>> Rects;
std::vector<std::pair<SDL_Texture*, SDL_Texture*>> Textures;


Dealer dealer;
Player player(500);
std::vector<PlayingCard>* dealerCardsPtr;
std::vector<PlayingCard>* playerCardsPtr;


int PlayerCardIndex = 0;
int DealerCardIndex = 0;
class dispenser {
public:
    void TakeCard(Player player_, int countOfCards_) {
        dealer.DealCards(player_, countOfCards_);

        for (int i = 0; i < countOfCards_; i++) {
            std::pair< SDL_Rect, SDL_Rect> temp({ 500, 200, 150, 200 }, { destinationRectForPlayer.x + 30 * PlayerCardIndex , destinationRectForPlayer.y,150,200 });

            Rects.push_back(temp);
            Textures.push_back(playerCardsPtr->at(PlayerCardIndex).GetTexture());
            PlayerCardIndex++;

        }

    };
    void TakeCard(Dealer dealer_, int countOfCards_) {
        dealer.DealCards(dealer_, countOfCards_);

        for (int i = 0; i < countOfCards_; i++) {
            std::pair< SDL_Rect, SDL_Rect> temp({ 500, 200, 150, 200 }, { destinationRectForDealer.x + 30 * DealerCardIndex, destinationRectForDealer.y, 150,200 });
            Rects.push_back(temp);
            Textures.push_back(dealerCardsPtr->at(DealerCardIndex).GetTexture());
            DealerCardIndex++;
        }

    };
    void DealerLogic(Dealer dealer_) {
            
        while (dealer.ShowScore().first < 17) {
            dealer.DealCards(dealer, 1);
            
         
            std::pair< SDL_Rect, SDL_Rect> temp({ 500, 200, 150, 200 }, { destinationRectForDealer.x + 30 * DealerCardIndex, destinationRectForDealer.y, 150,200 });
            Rects.push_back(temp);
            Textures.push_back(dealerCardsPtr->at(DealerCardIndex).GetTexture());
            DealerCardIndex++;

            if (dealer.ShowScore().first > 21 && dealer.ShowScore().second != 21) {
                dealer.setBusted();
                break;
            }
        
        }
    }

 };


double x = 1, y = 1;

void MoveCard();

bool isMoving = true;
int indexTexture = 0;
bool isRunning = true;
bool betStage = true;
bool startCardsStage = false;
bool hitOrStandStage = false;
bool dealerLogicStage = false;
bool winDefindingStage = false;

bool lastDealersCardMustBeShown = false;

SDL_Rect buttonRect = { 10, 50, 20, 60 };
SDL_Rect hitButtonRect = { 10, 500, 60, 60 };
void RenderThreadFunction(SDL_Renderer* renderer) {
    while (true) {

        if (isRunning == false)
            break;
        SDL_RenderClear(renderer);


        MoveCard();


        if (betStage || hitOrStandStage) {
            SDL_Surface* surface = IMG_Load("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/button.jpg");
            if (surface == nullptr)
                std::cout << "qweqwe";

            SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_RenderCopy(renderer, buttonTexture, nullptr, &buttonRect);
            
        }
        if (hitOrStandStage) {
            SDL_Surface* surface = IMG_Load("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/button.jpg");
            if (surface == nullptr)
                std::cout << "qweqwe";

            SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_RenderCopy(renderer, buttonTexture, nullptr, &hitButtonRect);
            
        }


        SDL_Texture* textScorePlayer = ShowText({ 0,0,0 }, { std::to_string(player.ShowScore().first).c_str() });

        SDL_Texture* textScoreDealer = ShowText({ 0,0,225 }, { std::to_string(dealer.ShowScore().first).c_str() });

        int time = 0;
        for (auto it : Textures) {
            if (it == dealerCardsPtr->at(dealerCardsPtr->size() - 1).GetTexture()) {
                if(lastDealersCardMustBeShown)
                    SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);
                else
                    SDL_RenderCopy(renderer, Textures[time].second, nullptr, &Rects[time].first);

                    SDL_RenderCopy(renderer, textScoreDealer, nullptr, &Rects[time].first);

            }
            else
                SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);

            if (it == playerCardsPtr->at(playerCardsPtr->size() - 1).GetTexture()) 
                SDL_RenderCopy(renderer, textScorePlayer, nullptr, &Rects[time].first);
            

            if (time == indexTexture)
                break;
            time++;
        }
        time = 0;


        // Оновлення відображення
        SDL_RenderPresent(renderer);
        if (isMoving == false) {
            if (indexTexture < Rects.size() - 1) {

                indexTexture++;
                isMoving = true;
            }


        }

    }
}
bool PointInRect(int x, int y, const SDL_Rect& rect) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}



bool isButtonPressed = false;
int main(int argc, char* argv[])

//int SDL_main(int argc, char* argv[]) 
{


    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init() == -1)
    {
        // Помилка при ініціалізації SDL_ttf
        std::cout << "Помилка при ініціалізації SDL_ttf: " << TTF_GetError() << std::endl;
        return -1; // або обробіть помилку за вашими потребами
    }


  
    SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    Deck& deck = Deck::GetInstance();
    deck.SetRenderer(renderer);
    deck.InitDeck();



    playerCardsPtr = player.GetCardInstance();
    dealerCardsPtr = dealer.GetCardInstance();



    Bank bank;
    SDL_Event event;

    dispenser disp;




    SDL_Rect StartRect{ 500, 200, 150, 200 };


    std::thread renderThread(RenderThreadFunction, renderer);

    std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS


    SDL_SetRenderDrawColor(renderer, 0, 0, 100, 100);
    SDL_RenderClear(renderer);

  
    bool takeCard = false;
    int moneyToBet = 0;
    while (isRunning) {
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                renderThread.join();
            }
            if (betStage == true)
            {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (PointInRect(mouseX, mouseY, buttonRect)) {
                        player.Bet(moneyToBet, bank);
                        std::cout << "Player money:" << player.GetMoney() << std::endl;
                        betStage = false;
                        startCardsStage = true;
                    }
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                    if (player.GetMoney() != moneyToBet)
                        moneyToBet += 100;
                    std::cout << "Money to bet:" << moneyToBet << std::endl;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                    if (moneyToBet > 100)
                        moneyToBet -= 100;
                    std::cout << "Money to bet:" << moneyToBet << std::endl;
                }

            }
            
            if (hitOrStandStage == true) {
               
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (PointInRect(mouseX, mouseY, buttonRect)) {
                        
                        std::cout << "Hit card" << std::endl;

                        takeCard = true;
                     
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (PointInRect(mouseX, mouseY, hitButtonRect)) {

                        player.setStanding();
                       
                    }
                }
                

            }
            


        }


        if (betStage) {
            continue;
        }

        if (startCardsStage) {
            disp.TakeCard(dealer, 2);
            disp.TakeCard(player, 2);
            startCardsStage = false;
            hitOrStandStage = true;
        }
        
        if (hitOrStandStage) {

            if (player.ShowScore().first > 21) {
                player.SetBusted();
                hitOrStandStage = false;
                dealerLogicStage = true;
            }
            if (player.getStanding())
            {
                hitOrStandStage = false;
                dealerLogicStage = true;

            }

            if (takeCard) {
                disp.TakeCard(player, 1);
                takeCard = false;
            }

        }

        if (dealerLogicStage) {
            lastDealersCardMustBeShown = true;
            if (player.GetBusted()) {
                winDefindingStage = true;
                dealerLogicStage = false;
            }
        

            disp.DealerLogic(dealer);
            

            if (dealer.GetBusted()) {
                winDefindingStage = true;
                dealerLogicStage = false;
            }

        }

        if (winDefindingStage)
        {
          
            int playerScore = player.ShowScore().first;
            int dealerScore = dealer.ShowScore().first;

            if(player.ShowScore().second == 21 )
                playerScore = player.ShowScore().second;
           
            if (dealer.ShowScore().second == 21)
                dealerScore = dealer.ShowScore().second;

            std::cout << "player score:" << playerScore;
            std::cout << "dealer score:" << dealerScore;

            if (player.GetBusted() == false) 
            {

                if (dealer.GetBusted()) {
                    std::cout << "player won";
                    winDefindingStage = false;
                }


                if ( playerScore > dealerScore && dealer.GetBusted() == false) {
                    std::cout << "player won";
                }
                if(dealerScore > playerScore && dealer.GetBusted() == false)
                    std::cout << "dealer won";
                }
                else {
                std::cout << "dealer won";
                winDefindingStage = false;
                     }
             }
      
            winDefindingStage = false;
        }


     




    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}

SDL_Texture* ShowText(SDL_Color color, const char* text) {

    //TTF_Font* font = TTF_OpenFont("E:/02 c+/01 myProjects/04 Black Jack/Project1/Project1/fonts/lazy.ttf", 30);
    TTF_Font* font = TTF_OpenFont("E:/lazy.ttf", 30); 
    SDL_Color textColor = color;
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    return textTexture;
}



void MoveCard() {
    if (Rects.size() == 0)
        return;

    if (isMoving) {
        if (Rects[indexTexture].first.x < Rects[indexTexture].second.x) {
            x += 0.001;
            Rects[indexTexture].first.x += x;
        }
        if (Rects[indexTexture].first.x > Rects[indexTexture].second.x) {
            x -= 0.001;
            Rects[indexTexture].first.x += x;
        }
        if (Rects[indexTexture].first.y < Rects[indexTexture].second.y) {
            y += 0.001;
            Rects[indexTexture].first.y += y;
        }
        if (Rects[indexTexture].first.y > Rects[indexTexture].second.y) {
            y -= 0.001;
            Rects[indexTexture].first.y += y;
        }

        if (std::abs(Rects[indexTexture].first.x - Rects[indexTexture].second.x) < 0.1 && std::abs(Rects[indexTexture].first.y - Rects[indexTexture].second.y) < 0.1) {
            isMoving = false;
            x = 1, y = 1;

        }
    }
}