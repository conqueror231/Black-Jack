#pragma once
#include <vector>
#include "PlayingCard.h"
#include <random>
#include <algorithm>
#include "Suit.h"
#include<string>
#include<SDL.h>
#include<SDL_image.h>


class Deck
{

private:
    SDL_Renderer* renderer = nullptr;

    std::vector<PlayingCard> cards;
    std::mt19937 gen;

    std::string suits[4]{ {"clubs"}, {"diamonds"}, {"hearts"}, {"spades" } };
    std::string fronts[3]{ {"king"}, {"jack"}, {"queen"}};
   

public:
    void InitDeck()
    {
        std::string frontImagePath;
        std::string backImagePath = "E:/02 c++/01 myProjects/04 Black Jack/Project1/Project1/PNG-cards-1.3/card back red.png";
        SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
        SDL_Surface* surface2 = IMG_Load(backImagePath.c_str());
        for (int x = 0; x < 4; x++) {
            for (int i = 2; i < 11; i++)
            {
                frontImagePath = "E:/02 c++/01 myProjects/04 Black Jack/Project1/Project1/PNG-cards-1.3/" +
                std::to_string(i) + "_of_" + suits[x] + ".png";

                surface = IMG_Load(frontImagePath.c_str());


                cards.emplace_back(i, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));
            }
            for (int i = 0; i < 3; i++)
            {
                frontImagePath = "E:/02 c++/01 myProjects/04 Black Jack/Project1/Project1/PNG-cards-1.3/" +  fronts[i] + "_of_" + suits[x] + "2.png";
                surface = IMG_Load(frontImagePath.c_str());
                cards.emplace_back(10, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));
            }
            frontImagePath = "E:/02 c++/01 myProjects/04 Black Jack/Project1/Project1/PNG-cards-1.3/ace_of_" + suits[x] + ".png";
            cards.emplace_back(11, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));
        }

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
            throw std::runtime_error("Колода пуста!");
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

    Suit GetSuitByNumber(int randNum)
    {
        switch (randNum)
        {
        case 1:
            return Suit::Spades;
        case 2:
            return Suit::Hearts;
        case 3:
            return Suit::Diamonds;
        case 4:
            return Suit::Clubs;
        default:
            throw std::invalid_argument("Недопустимый номер для масти.");
        }
    }
};
