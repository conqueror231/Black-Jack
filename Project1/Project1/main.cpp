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

void ShowText(std::string text, SDL_Rect& place, SDL_Color color, std::string fontPath);

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int CARD_WIDTH = 100;
const int CARD_HEIGHT = 150;

SDL_Renderer* renderer = nullptr;

SDL_Rect destinationRectForPlayer{ 300, 400, 150, 200 };
SDL_Rect destinationRectForDealer{ 300, 50, 150, 200 };

std::vector<std::pair<SDL_Rect, SDL_Rect>> Rects;
std::vector<std::pair<SDL_Texture*, SDL_Texture*>> Textures;


Dealer dealer;
Player player(500);
std::vector<PlayingCard>* dealerCardsPtr;
std::vector<PlayingCard>* playerCardsPtr;


short int PlayerCardIndex = 0;
short int DealerCardIndex = 0;

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

void ShowText(std::string text, SDL_Rect& place, SDL_Color color, std::string fontPath) {


    // TTF_Font* font = TTF_OpenFont("E:/lazy.ttf", 30);
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 30);
    SDL_Color textColor = color;
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, textSurface), nullptr, &place);
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;

    TTF_CloseFont(font);
    font = nullptr;

}

double x = 1, y = 1;

void MoveCard();

bool isMoving = true;
int indexTexture = 0;
bool isRunning = true;

#pragma region Bools for stages
bool betStage = true;
bool startCardsStage = false;
bool hitOrStandStage = false;
bool dealerLogicStage = false;
bool winDefindingStage = false;

bool lastDealersCardMustBeShown = false;
#pragma endregion

#pragma region Rects for interface

SDL_Rect buttonRect = { 10, 500, 60, 60 };
SDL_Rect hitButtonRect = { 800, 400, 60, 60 };
SDL_Rect stayButtonRect = { 800, 500, 60, 60 };

SDL_Rect moneyToBetRect = { 10, 400, 100, 100 };
SDL_Rect totalBankMoneyRect = { 300, 300, 100, 60 };
#pragma endregion

int moneyToBet = 100;
void RenderThreadFunction(SDL_Renderer* renderer) {
    while (true) {
        std::string text;

        if (isRunning == false)
            break;
        SDL_RenderClear(renderer);


        MoveCard();
        //player score
        
        if (player.ShowScore().second == 21)
            text = std::to_string(player.ShowScore().second);
        text = std::to_string(player.ShowScore().first);

        SDL_Rect HandScore { destinationRectForPlayer.x + 100 + 40 * playerCardsPtr->size(), destinationRectForPlayer.y, 100, 100};
       
        ShowText(text, HandScore, {0,0,0}, "E:/lazy.ttf");
       

        //dealer score
     
        if (dealer.ShowScore().second == 21)
            text = std::to_string(dealer.ShowScore().second);
        text = std::to_string(dealer.ShowScore().first);

        SDL_Rect HandScoreDealer{ destinationRectForDealer.x + 100 + 40 * playerCardsPtr->size(), destinationRectForDealer.y, 100, 100 };
        ShowText(text, HandScoreDealer, { 0,0,0 }, "E:/lazy.ttf");



        //total bank money
        SDL_Texture* totalBankMoneyTexture = nullptr;
        text = "Bank:";
        text.append(std::to_string(Bank::GetInstance().GetTotalMoney()).c_str());

        ShowText(text, totalBankMoneyRect, { 0,0,0 }, "E:/lazy.ttf");

        //Player Money
        text = "Money: ";
        text.append(std::to_string(player.GetMoney()));
   
        SDL_Rect PlayerMoney{ destinationRectForPlayer.x - 150, destinationRectForPlayer.y, 150, 60 };
        ShowText(text, PlayerMoney, { 0,0,0 }, "E:/lazy.ttf");
        
      
        if (betStage) {
          SDL_Surface* surface = IMG_Load("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/button.jpg");
           // SDL_Surface* surface = IMG_Load("E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/button.jpg");
            
          
          //  ShowText({ 0,0,225 }, { std::to_string(moneyToBet).c_str() }, moneyToBetTexture);
            ShowText(std::to_string(moneyToBet), moneyToBetRect, { 0,0,0 }, "E:/lazy.ttf");

            if (surface == nullptr)
                std::cout << "Surface didnt load" << std::endl;

            SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_RenderCopy(renderer, buttonTexture, nullptr, &buttonRect);
            SDL_DestroyTexture(buttonTexture);
           
            SDL_FreeSurface(surface);
        }
        if (hitOrStandStage) {
            //SDL_Surface* surface = IMG_Load("E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/button.jpg");
            SDL_Surface* surface = IMG_Load("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/button.jpg");
            if (surface == nullptr)
                std::cout << "qweqwe";

            SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_RenderCopy(renderer, buttonTexture, nullptr, &hitButtonRect);

            SDL_RenderCopy(renderer, buttonTexture, nullptr, &stayButtonRect);

            SDL_DestroyTexture(buttonTexture);
            SDL_FreeSurface(surface);

            
            
        }

  
        int time = 0;
        for (auto& it: Textures) {
            if (it == dealerCardsPtr->at(dealerCardsPtr->size() - 1).GetTexture()) {
               
                if(lastDealersCardMustBeShown)
                    SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);
                else
                    SDL_RenderCopy(renderer, Textures[time].second, nullptr, &Rects[time].first);
            }
            else
                SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);

               
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



    
    SDL_Event event;

    dispenser disp;

    SDL_Rect StartRect{ 500, 200, 150, 200 };


    std::thread renderThread(RenderThreadFunction, renderer);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(0)); // ~60 FPS


    SDL_SetRenderDrawColor(renderer, 0, 183, 0, 255);
    SDL_RenderClear(renderer);


    bool takeCard = false;
    bool waitToEndCardsMoving = false;

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

                        
                        Bank::GetInstance().SendMoney(moneyToBet, player);

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
                    if (PointInRect(mouseX, mouseY, hitButtonRect)) {

                        std::cout << "Hit card" << std::endl;

                        takeCard = true;

                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (PointInRect(mouseX, mouseY, stayButtonRect)) {

                        player.SetBusted();
                        hitOrStandStage = false;
                        dealerLogicStage = true;

                    }
                }


            }



        }


        if (betStage) {
            
            if (player.GetMoney() == 0) {
                std::cout << "Player dont have money";
                player.SetLost();
                betStage = false;
            }
       
        }

        if (startCardsStage) {
            disp.TakeCard(dealer, 2);
            disp.TakeCard(player, 2);
            startCardsStage = false;
            hitOrStandStage = true;
        }

        if (hitOrStandStage) {

            if (player.getStanding())
            {
                hitOrStandStage = false;
                dealerLogicStage = true;

            }

            if (player.ShowScore().first > 21 && player.ShowScore().second != 21) {
                player.SetBusted();
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

            if (player.ShowScore().second == 21)
                playerScore = player.ShowScore().second;

            if (dealer.ShowScore().second == 21)
                dealerScore = dealer.ShowScore().second;


            std::cout << "player score:" << playerScore << std::endl;
            std::cout << "dealer score:" << dealerScore << std::endl;
         
           if (dealerScore == playerScore) {
                std::cout << "draw" << std::endl;
                Bank::GetInstance().GiveMoney(moneyToBet/2, player);
            }



            if (playerScore > dealerScore && playerScore < 22)
            {
                Bank::GetInstance().GiveMoney(moneyToBet, player);
                std::cout << "player won (playerScore > dealerScore) ";
                std::cout << "Player money:" << player.GetMoney() << std::endl;
            }
            if (playerScore < 22 && dealerScore > 21)
            {
                std::cout << "dealer lost (dealer > 21)" << std::endl;
                Bank::GetInstance().GiveMoney(moneyToBet, player);
            }
            
            winDefindingStage = false;
            waitToEndCardsMoving = true;
            
        }
      
        if (waitToEndCardsMoving) {
                if (indexTexture + 1 == (PlayerCardIndex + DealerCardIndex)) {
                
                if (isMoving == false) {
                    winDefindingStage = false;
                    betStage = true;
                    Rects.clear();
                    Textures.clear();
                    PlayerCardIndex = 0;
                    DealerCardIndex = 0;
                    moneyToBet = 100;
                    indexTexture = 0;

                    player.Reload();
                    dealer.Reload();
                    Bank::GetInstance().ReloadBank();
                    waitToEndCardsMoving = false;
                    lastDealersCardMustBeShown = false;
                    isMoving = true;
                    
                }
            }
        }
    }





    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

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