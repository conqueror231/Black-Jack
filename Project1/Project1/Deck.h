#pragma once
#include <vector>
#include "PlayingCard.h"
#include <random>
#include <algorithm>
#include "Suit.h"
#include<string>
#include<SDL.h>
#include<SDL_image.h>

struct CardInfo
{
    PlayingCard card;
    Suit suit;

    CardInfo(const PlayingCard& card_, const Suit& suit_)
        : card(card_), suit(suit_)
    {
    }
};



class Deck
{

private:
    SDL_Renderer* renderer = nullptr;

    std::vector< CardInfo > cards;
    

    std::mt19937 gen;

    std::string suits[4]{ {"clubs"}, {"diamonds"}, {"hearts"}, {"spades" } };
    std::string fronts[3]{ {"king"}, {"jack"}, {"queen"} };


public:
    void InitDeck()
    {
        
       // std::string frontImagePath;

    //    std::string backImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";
       // std::string backImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";
  
      // 
     // /  SDL_Surface* surface = IMG_Load(backImagePath.c_str());
     //   CardInfo card{ PlayingCard(10, SDL_CreateTextureFromSurface(renderer, surface),SDL_CreateTextureFromSurface(renderer, surface)),Suit::Spades };
    //  /  cards.emplace_back(card);
     //   std::cout << SDL_GetError() << std::endl;
        //SDL_Surface* surface2 = IMG_Load(backImagePath.c_str());
        //for (int x = 0; x < 4; x++) {
        //    for (int i = 2; i < 11; i++)
        //    {
        //       frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/" +
        //   //     frontImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/" +
        //            std::to_string(i) + "_of_" + suits[x] + ".png";

        //        SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
        //        cards.emplace_back(i, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2));
        //        SDL_FreeSurface(surface);
        //        surface = nullptr;
        //       
        //    }
        //    for (int i = 0; i < 3; i++)
        //    {
        //         frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/" +  fronts[i] + "_of_" + suits[x] + "2.png";
        //       // frontImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/" + fronts[i] + "_of_" + suits[x] + "2.png";
        //        SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
        //        
        //        SDL_FreeSurface(surface);
        //        surface = nullptr;
        //        
        //    }
        //    frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/ace_of_" + suits[x] + ".png";
        //   // frontImagePath = "E:/Black-Jack/.git/Black-Jack/Project1/Project1/PNG-cards-1.3/ace_of_" + suits[x] + ".png";
        //    SDL_Surface* surface = IMG_Load(frontImagePath.c_str());
        //    cards.emplace_back(CardInfo(11, SDL_CreateTextureFromSurface(renderer, surface), SDL_CreateTextureFromSurface(renderer, surface2), Spades));
      
        //    SDL_FreeSurface(surface);
        //    surface = nullptr;
        //   
        //}
        //SDL_FreeSurface(surface2);
        //surface2 = nullptr;
        
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
       // for (const auto& card : cards) {
       //     card.first.Display();
       // }
    }

    void ShuffleDeck()
    {
      //  std::shuffle(cards.begin(), cards.end(), gen);
    }

    PlayingCard pickCard()
    {
        PlayingCard pickedCard;
       
       
        std::uniform_int_distribution<size_t> cardValueDist(2, 15);
        std::uniform_int_distribution<size_t> suitDist(0, 3);

        bool alreadyUsed = false;
        while (true) {
        
            size_t score = cardValueDist(gen);
        
            size_t suitIndex = suitDist(gen);
            
           
            for (auto card : cards) {
                if (card.card.GetScore() == score)
                {
                    for (int i = 0; i < 4; i++){
                        if (card.suit == GetSuitByNumber(suitIndex)) 
                        {

                            if (suitIndex != suitDist(gen)) {
                                alreadyUsed = false;
                                break;
                            }
                            else
                                alreadyUsed = true;


                        }
                        
                    }
                }
                if (alreadyUsed == false) {
                    break;
                }


            }

            if (alreadyUsed == false) {
              
                std::string backImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/card back red.png";
               // std::string frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/" + std::to_string(score) + "_of_" + suits[suitIndex] + ".png";
                std::string frontImagePath = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/8_of_clubs.png";
                int actualScore = score;

                if(score > 11 && score < 15)
                     actualScore = 10;

                
                SDL_Surface* frontSurface = IMG_Load("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/8_of_clubs.png");
                
                std::cout << IMG_GetError() <<std::endl;
                SDL_Surface* backSurface = IMG_Load("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/8_of_clubs.png");


                SDL_FreeSurface(frontSurface);
                SDL_FreeSurface(backSurface);

                frontSurface = nullptr;
                backSurface = nullptr;

                PlayingCard tempCard(actualScore, frontSurface, backSurface);;
                   
                pickedCard = tempCard;
                cards.emplace_back(std::make_pair(pickedCard, GetSuitByNumber(suitIndex)));
                cards.emplace_back(CardInfo(PlayingCard(score, frontSurface, backSurface), GetSuitByNumber(suitIndex)));
               

               
                break;
            }

        }
        
        return pickedCard;

        // pickedCard;
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
            throw std::invalid_argument("������������ ����� ��� �����.");
        }
    }
};