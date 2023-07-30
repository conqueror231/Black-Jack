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
#include <mutex>
#include <chrono>
#include <SDL_mixer.h>

Mix_Music* gMusic = NULL;


void ShowText(std::string text, SDL_Rect& place, SDL_Color color, std::string fontPath);

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 900;
const int CARD_WIDTH = 500;
const int CARD_HEIGHT = 500;

SDL_Renderer* renderer = nullptr;

SDL_Rect destinationRectForPlayer{ WINDOW_WIDTH / 4, WINDOW_HEIGHT / 1.76, CARD_WIDTH, CARD_HEIGHT };
SDL_Rect destinationRectForDealer{ WINDOW_WIDTH / 4, -150, CARD_WIDTH, CARD_HEIGHT };

std::vector<std::pair<SDL_Rect, SDL_Rect>> Rects;
std::vector<std::pair<SDL_Texture*, SDL_Texture*>> Textures;


Dealer dealer;
Player player(500);
std::vector<PlayingCard>* dealerCardsPtr;
std::vector<PlayingCard>* playerCardsPtr;


short int PlayerCardIndex = 0;
short int DealerCardIndex = 0;

std::mutex rendererMutex;

class smartPtrShowImg {
private:
    const std::string path;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect positionOnScreen;

public:
    smartPtrShowImg(const char* path_, SDL_Rect positionOnScreen_)
        : path("Assets/" + std::string(path_)), positionOnScreen(positionOnScreen_)
    {

    }

    ~smartPtrShowImg()
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        SDL_FreeSurface(surface);
        surface = nullptr;
    }
    void showImg() {
        surface = IMG_Load(path.c_str());
        std::cout << path <<std::endl;
      // std::cout << surface <<std::endl;

        rendererMutex.lock();
        texture = SDL_CreateTextureFromSurface(renderer, surface);
       // std::cout << SDL_GetError() << std::endl;
        SDL_RenderCopy(renderer, texture, nullptr, &positionOnScreen);
       // std::cout << SDL_GetError() << std::endl;
        rendererMutex.unlock();
       
       
    }
   
};
class dispenser {
public:
    void TakeCard(Player& player, int countOfCards_) {
        Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/cardPlacedSound.mp3");
        Mix_PlayChannel(-1, dealCardEffect, 0);
        Mix_Volume(-1, MIX_MAX_VOLUME / 8);
        Mix_FreeChunk(dealCardEffect);
        for (int i = 0; i < countOfCards_; i++)
            Deck::GetInstance().AddCard(rendererMutex);

        dealer.DealCards(player, countOfCards_);

        for (int i = 0; i < countOfCards_; i++) {
            std::pair<SDL_Rect, SDL_Rect> temp({ 700, 200, CARD_WIDTH, CARD_HEIGHT }, { destinationRectForPlayer.x + 30 * PlayerCardIndex, destinationRectForPlayer.y, CARD_WIDTH, CARD_HEIGHT });

            Rects.push_back(temp);
            Textures.push_back(playerCardsPtr->at(PlayerCardIndex).GetTexture());
            PlayerCardIndex++;

        }
    }

    void TakeCard(Dealer& dealer, int countOfCards_) {
        for (int i = 0; i < countOfCards_; i++)
            Deck::GetInstance().AddCard(rendererMutex);
        //Deck::GetInstance().AddCard();
        dealer.DealCards(dealer, countOfCards_);

        for (int i = 0; i < countOfCards_; i++) {
            std::pair<SDL_Rect, SDL_Rect> temp({ 700, 200,CARD_WIDTH, CARD_HEIGHT }, { destinationRectForDealer.x + 30 * DealerCardIndex, destinationRectForDealer.y, CARD_WIDTH, CARD_HEIGHT });
            Rects.push_back(temp);
            Textures.push_back(dealerCardsPtr->at(DealerCardIndex).GetTexture());
            DealerCardIndex++;
        }
    }

    void DealerLogic(Dealer& dealer) {
        while (dealer.ShowScore().first < 17) {

            Deck::GetInstance().AddCard(rendererMutex);
            dealer.DealCards(dealer, 1);

            std::pair<SDL_Rect, SDL_Rect> temp({ 700, 200, CARD_WIDTH, CARD_HEIGHT }, { destinationRectForDealer.x + 30 * DealerCardIndex, destinationRectForDealer.y, CARD_WIDTH, CARD_HEIGHT });
            Rects.push_back(temp);
            Textures.push_back(dealerCardsPtr->at(DealerCardIndex).GetTexture());
            DealerCardIndex++;

            if (dealer.ShowScore().first > 21 && dealer.ShowScore().second != 21) {
                dealer.SetBusted();
                break;
            }
        }
    }
};


void ShowText(std::string text, SDL_Rect& place, SDL_Color color, std::string fontPath) {
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 30);
    SDL_Color textColor = color;
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    // rendererMutex.lock();
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, textTexture, nullptr, &place);
    // rendererMutex.unlock();
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
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
bool GoToNextDeal = false;
bool waitToEndCardsMoving = false;
#pragma region Rects for interface

SDL_Rect buttonRect = { 10, WINDOW_HEIGHT - 70, 60, 60 };
SDL_Rect hitButtonRect = { WINDOW_WIDTH - 70,  WINDOW_HEIGHT - 170, 60, 60 };
SDL_Rect stayButtonRect = { WINDOW_WIDTH - 70,  WINDOW_HEIGHT - 70, 60, 60 };
SDL_Rect moneyToBetRect = { 10, 400, 100, 100 };
SDL_Rect totalBankMoneyRect = { 300, 300, 100, 60 };
SDL_Rect GoToNextDealRect = { 450, 300, 100, 60 };
SDL_Rect SwitchCardsTexturesRect = { 10, 10, 60, 60 };

SDL_Rect WalletRect = { WINDOW_WIDTH - 70,100,500,500};
SDL_Rect CalculatorForBetRect = { WINDOW_WIDTH - 70, WINDOW_HEIGHT - 70,500,500};
SDL_Rect DeckRect = { WINDOW_WIDTH-70,WINDOW_HEIGHT/2,500,500 };
SDL_Rect PlusOrMinusBetRect = { WINDOW_WIDTH - 170,WINDOW_HEIGHT-70,500,500 };

#pragma endregion

int moneyToBet = 100;



void RenderThreadFunction() {
    while (true) {


        auto startTime = std::chrono::high_resolution_clock::now();
        std::string text;

        if (isRunning == false)
            break;


        rendererMutex.lock();
        SDL_RenderClear(renderer);
        rendererMutex.unlock();

      
        //player score

        if (player.ShowScore().second == 21)
            text = std::to_string(player.ShowScore().second);
        text = std::to_string(player.ShowScore().first);

        SDL_Rect HandScore{ destinationRectForPlayer.x + 100 + 40 * playerCardsPtr->size(), destinationRectForPlayer.y, 100, 100 };


        ShowText(text, HandScore, { 0,0,0 }, "Assets/lazy.ttf");

        //dealer score
        if (lastDealersCardMustBeShown) {
            if (dealer.ShowScore().second == 21)
                text = std::to_string(dealer.ShowScore().second);
            text = std::to_string(dealer.ShowScore().first);

            SDL_Rect HandScoreDealer{ destinationRectForDealer.x + 100 + 40 * playerCardsPtr->size(), destinationRectForDealer.y, 100, 100 };

            ShowText(text, HandScoreDealer, { 0,0,0 }, "Assets/lazy.ttf");
        }


        //total bank money
        SDL_Texture* totalBankMoneyTexture = nullptr;
        text = "Bank:";
        text.append(std::to_string(Bank::GetInstance().GetTotalMoney()).c_str());


        ShowText(text, totalBankMoneyRect, { 0,0,0 }, "Assets/lazy.ttf");

        //Player Money
        text = "Money: ";
        text.append(std::to_string(player.GetMoney()));

        SDL_Rect PlayerMoney{ destinationRectForPlayer.x - 150, destinationRectForPlayer.y, 150, 60 };

        ShowText(text, PlayerMoney, { 0,0,0 }, "Assets/lazy.ttf");

        

        

        if (betStage) {


   
            ShowText(std::to_string(moneyToBet), moneyToBetRect, { 0,0,0 }, "Assets/lazy.ttf");
           
            smartPtrShowImg ptrShowImg {"button.jpg",buttonRect};

            ptrShowImg.showImg();
           
        }

        if (hitOrStandStage) {
            smartPtrShowImg ptrShowImg{ "button.jpg",hitButtonRect };

            ptrShowImg.showImg();

            smartPtrShowImg ptrShowImg2{ "button.jpg",stayButtonRect };

            ptrShowImg2.showImg();

        }

        if (waitToEndCardsMoving) {
            smartPtrShowImg ptrShowImg{ "button.jpg",GoToNextDealRect };

            ptrShowImg.showImg();

           
        }

        int time = 0;
        for (auto& it : Textures) {
          
            if (it == dealerCardsPtr->at(dealerCardsPtr->size() - 1).GetTexture()) {

                if (lastDealersCardMustBeShown) {
                    rendererMutex.lock();
                    SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);
                    rendererMutex.unlock();
                }
                else {
                    rendererMutex.lock();
                    SDL_RenderCopy(renderer, Textures[time].second, nullptr, &Rects[time].first);
                    rendererMutex.unlock();
                }
            }
            else {
                rendererMutex.lock();
                SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);
                rendererMutex.unlock();
            }
            

            if (time == indexTexture)
                break;
            time++;
        }
        time = 0;


        // Оновлення відображення
        rendererMutex.lock();
        SDL_RenderPresent(renderer);
        rendererMutex.unlock();


      

     



    }
}
void RenderCardsThreadFunction() {
    while (true) {

        if (isRunning == false)
            break;

      
        if (Rects.size() != 0) {
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
                    if (Rects.size() - 1 != indexTexture) {

                        Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/cardPlacedSound.mp3");
                        Mix_PlayChannel(-1, dealCardEffect, 0);
                        Mix_Volume(-1, MIX_MAX_VOLUME / 8);
                        Mix_FreeChunk(dealCardEffect);
                    }

                    isMoving = false;
                    x = 1, y = 1;

                }
            }
            if (isMoving == false) {

                if (indexTexture < Rects.size() - 1) {

                    indexTexture++;
                    isMoving = true;
                }


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

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1)
    {
        std::cout << "SDL could not initialize! SDL Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());

    }



    SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    Deck& deck = Deck::GetInstance();
    rendererMutex.lock();
    deck.SetRenderer(renderer);
    rendererMutex.unlock();



    playerCardsPtr = player.GetCardInstance();
    dealerCardsPtr = dealer.GetCardInstance();

    SDL_Event event;

    dispenser disp;


    SDL_Rect StartRect{ 500, 200, 150, 200 };


    std::thread renderThread(RenderThreadFunction);
    std::thread renderCardsThread(RenderCardsThreadFunction);



    rendererMutex.lock();
    SDL_SetRenderDrawColor(renderer, 0, 183, 0, 255);
    rendererMutex.unlock();



    bool takeCard = false;

    gMusic = Mix_LoadMUS("Assets/music.mp3");

    Mix_PlayMusic(gMusic, -1);
    Mix_VolumeMusic(20);
    while (isRunning) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                renderThread.join();
                renderCardsThread.join();
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (PointInRect(mouseX, mouseY, SwitchCardsTexturesRect)) {
                    Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/buttonPressed.mp3");
                    Mix_PlayChannel(-1, dealCardEffect, 0);
                    Mix_Volume(-1, MIX_MAX_VOLUME / 4);
                    Mix_FreeChunk(dealCardEffect);
                    // Deck::GetInstance().SwitchToAnotherTextureCards(rendererMutex);
                }
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

                    Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/chips.mp3");
                    Mix_PlayChannel(-1, dealCardEffect, 0);
                    Mix_Volume(-1, MIX_MAX_VOLUME / 4);
                    Mix_FreeChunk(dealCardEffect);

                    if (player.GetMoney() != moneyToBet)
                        moneyToBet += 100;
                    std::cout << "Money to bet:" << moneyToBet << std::endl;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {

                    Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/chips.mp3");
                    Mix_PlayChannel(-1, dealCardEffect, 0);
                    Mix_Volume(-1, MIX_MAX_VOLUME / 4);

                    Mix_FreeChunk(dealCardEffect);
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

                        Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/buttonPressed.mp3");
                        Mix_PlayChannel(-1, dealCardEffect, 0);
                        Mix_Volume(-1, MIX_MAX_VOLUME / 4);

                        std::cout << "Hit card" << std::endl;

                        takeCard = true;

                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (PointInRect(mouseX, mouseY, stayButtonRect)) {

                        Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/buttonPressed.mp3");
                        Mix_PlayChannel(-1, dealCardEffect, 0);
                        Mix_Volume(-1, MIX_MAX_VOLUME / 4);

                        Mix_FreeChunk(dealCardEffect);
                        player.SetBusted();
                        hitOrStandStage = false;
                        dealerLogicStage = true;

                    }
                }


            }

            if (waitToEndCardsMoving == true) {

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (PointInRect(mouseX, mouseY, GoToNextDealRect)) {

                       

                        GoToNextDeal = true;

                    }
                }


            }


        }


        if (betStage) {
         

            if (player.GetMoney() == 0) {
                std::cout << "Player dont have money";
                player.SetLost(true);
                betStage = false;
            }

        }

        if (startCardsStage) {

            std::cout << "DealCards";

            disp.TakeCard(dealer, 2);
            disp.TakeCard(player, 2);

            startCardsStage = false;
            hitOrStandStage = true;

        }

        if (hitOrStandStage) {

            if (player.GetStanding())
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
                Bank::GetInstance().GiveMoney(moneyToBet / 2, player);
            }



            if (playerScore > dealerScore && playerScore < 22)
            {

                Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/winChisp.mp3");
                Mix_PlayChannel(-1, dealCardEffect, 0);
                Mix_Volume(-1, MIX_MAX_VOLUME / 8);
                Mix_FreeChunk(dealCardEffect);



                Bank::GetInstance().GiveMoney(moneyToBet, player);
                std::cout << "player won (playerScore > dealerScore) ";
                std::cout << "Player money:" << player.GetMoney() << std::endl;
            }
            if (playerScore < 22 && dealerScore > 21)
            {
                std::cout << "dealer lost (dealer > 21)" << std::endl;
                Bank::GetInstance().GiveMoney(moneyToBet, player);


                Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/winChisp.mp3");
                Mix_PlayChannel(-1, dealCardEffect, 0);
                Mix_Volume(-1, MIX_MAX_VOLUME / 8);
                Mix_FreeChunk(dealCardEffect);
            }
            if (playerScore > 21) {
                Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/winChisp.mp3");
                Mix_PlayChannel(-1, dealCardEffect, 0);
                Mix_Volume(-1, MIX_MAX_VOLUME / 8);
                Mix_FreeChunk(dealCardEffect);
            }



            winDefindingStage = false;
            waitToEndCardsMoving = true;

        }

        if (waitToEndCardsMoving) {

            if (indexTexture + 1 == (PlayerCardIndex + DealerCardIndex)) {

                if (isMoving == false && GoToNextDeal == true) {
                   
                    winDefindingStage = false;
                    betStage = true;
                    Rects.clear();

                  //  for (auto text : Textures) {
                  //      SDL_DestroyTexture(text.first);
                   //     SDL_DestroyTexture(text.second);
                    }
                    Textures.clear();

                    PlayerCardIndex = 0;
                    DealerCardIndex = 0;
                    moneyToBet = 100;
                   
                   

                    player.Reload();
                    dealer.Reload();
                    Bank::GetInstance().ReloadBank();
                    Deck::GetInstance().ReloadDeck();

                  

                    waitToEndCardsMoving = false;
                    lastDealersCardMustBeShown = false;
                    isMoving = true;
                    GoToNextDeal = false;
                    indexTexture = 0;
                }
            }
        }
    }







    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}





void MoveCard()  {
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
            if (Rects.size() - 1 != indexTexture) {

                Mix_Chunk* dealCardEffect = Mix_LoadWAV("Assets/cardPlacedSound.mp3");
                Mix_PlayChannel(-1, dealCardEffect, 0);
                Mix_Volume(-1, MIX_MAX_VOLUME / 8);
                Mix_FreeChunk(dealCardEffect);
            }

            isMoving = false;
            x = 1, y = 1;

        }
    }
    if (isMoving == false) {

        if (indexTexture < Rects.size() - 1) {

            indexTexture++;
            isMoving = true;
        }


    }
}
