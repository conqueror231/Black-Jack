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

    std::string PathsToCards[2] = { {"Assets/"}, {"Assets/"} };
    int index = 0;
public:
    void SwitchToAnotherTextureCards(std::mutex& mutex);
    void AddCard(std::mutex& mutex);
    void ReloadDeck();
    int FromSuitToNumber(Suit suit_);
    Suit FromNumberToSuit(int num);
    void SetRenderer(SDL_Renderer* renderer_);
    Deck();
    void ShowDeck() const;
    void ShuffleDeck();
    PlayingCard pickCard();

    static Deck& GetInstance()
    {

        static Deck instance;
        return instance;
    }

    
   
};