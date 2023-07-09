#pragma once
#include <vector>
#include "PlayingCard.h"
#include <random>
#include <algorithm>
#include "Suit.h"
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<mutex>
#include<chrono>
class Deck
{

private:
    SDL_Renderer* renderer = nullptr;

    std::vector<PlayingCard> cards;
    std::mt19937 gen;

    std::string suits[4]{ {"clubs"}, {"diamonds"}, {"hearts"}, {"spades" } };
    std::string fronts[3]{ {"king"}, {"jack"}, {"queen"} };


public:
    void AddCard(std::mutex& mutex)
    {
      
        mutex.lock();

    /*    std::cout << "AddCard===================================================================================================================" <<std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();*/
       

        PlayingCard  cardToAdd;
        SDL_Surface* surfF, *surfB;
     
        std::uniform_int_distribution<size_t> cardScore(2, 14);
        std::uniform_int_distribution<size_t> cardSuit(0, 3);
        size_t cardScoreNum, cardSuitNum;
        while (true) {
            
            cardScoreNum = cardScore(gen);
            cardSuitNum = cardSuit(gen);

            std::string f = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/" + std::to_string(cardScoreNum) + "_of_" + suits[cardSuitNum] + ".png";
            std::string b = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";

            surfF = IMG_Load(f.c_str());
            surfB = IMG_Load(b.c_str());

            bool cardUsed = false;
            for (auto card : cards) {
                int num = FromSuitToNumber(card.GetSuit());
                if (card.GetScore() == cardScoreNum && FromSuitToNumber(card.GetSuit()) == cardSuitNum)
                {
                    cardUsed = true;
                    break;
                }

            }
            if (cardUsed == false)
                break;


        }

        if (cardScoreNum > 11 && cardScoreNum != 11)
            cardScoreNum = 10;
        
            cards.emplace_back(cardScoreNum, FromNumberToSuit(cardSuitNum), SDL_CreateTextureFromSurface(renderer, surfF), SDL_CreateTextureFromSurface(renderer, surfB));

        
       /* auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        std::cout << "Time DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDeck: " << duration.count() << " ms" << std::endl;*/



        mutex.unlock();

         
    }

    int FromSuitToNumber(Suit suit_) {
        switch(suit_)
        {
        case Suit::Clubs:{
            return 1;
        }
        case Suit::Diamonds:{
            return 2;
        }
        case Suit::Hearts: {
            return 3;
        }
        case Suit::Spades: {
            return 4;
        }
      } 
    }
    Suit FromNumberToSuit(int num){
    
        switch (num)
        {
        case 1: {
            return Suit::Clubs;
        }
        case 2: {
            return  Suit::Diamonds;
        }
        case 3: {
            return Suit::Hearts;
        }
        case 4: {
            return Suit::Spades;
        }
        }
    
    }


    void InitDeck()
    {/*
        for (int i = 0; i < 10; i++) {
            std::string f = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/2_of_diamonds.png";
            std::string b = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";

            SDL_Surface* surfF = IMG_Load(f.c_str());
            SDL_Surface* surfB = IMG_Load(b.c_str());

            cards.emplace_back(2, SDL_CreateTextureFromSurface(renderer, surfF), SDL_CreateTextureFromSurface(renderer, surfB));

            SDL_FreeSurface(surfB);
            SDL_FreeSurface(surfF);
            */
       // }
        /*
        std::string frontImagePath;

        std::string backImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";
        // std::string backImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";

        SDL_Surface* surface2 = IMG_Load(backImagePath.c_str());
        for (int x = 0; x < 4; x++) {
            for (int i = 2; i < 11; i++)
            {
                frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/" +
                    //     frontImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/" +
                    std::to_string(i) + "_of_" + suits[x] + ".png";

                SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
                cards.emplace_back(i, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));
                SDL_FreeSurface(surface);
                surface = nullptr;

            }
            for (int i = 0; i < 3; i++)
            {
                frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/" + fronts[i] + "_of_" + suits[x] + "2.png";
                // frontImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/" + fronts[i] + "_of_" + suits[x] + "2.png";
                SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
                cards.emplace_back(10, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));
                SDL_FreeSurface(surface);
                surface = nullptr;

            }
            frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/ace_of_" + suits[x] + ".png";
            // frontImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/ace_of_" + suits[x] + ".png";
            SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
            cards.emplace_back(11, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));

            SDL_FreeSurface(surface);
            surface = nullptr;

        }
        SDL_FreeSurface(surface2);
        surface2 = nullptr;
        */
    }

    static Deck& GetInstance()
    {

        static Deck instance;
        return instance;
    }

    void SetRenderer(SDL_Renderer* renderer_) {
        renderer = renderer_;
    }

    Deck()
    {

        std::random_device rd;
        gen = std::mt19937(rd());
    }

    void ShowDeck() const
    {
        for (const auto& card : cards) {
            card.Display();
        }
    }

    void ShuffleDeck()
    {
        std::shuffle(cards.begin(), cards.end(), gen);
    }

    PlayingCard pickCard()
    {
        PlayingCard pickedCard;
        if (cards.empty()) {
           throw std::runtime_error("������ �����!");

        }

        while (true) {
            std::uniform_int_distribution<size_t> dist(0, cards.size() - 1);
            size_t index = dist(gen);

            if (cards[index].getIsUsing() == false) {
                pickedCard = cards[index];
                cards[index].setIsUsing(true);
                break;
            }

        }

        return pickedCard;
    }

    
   
};