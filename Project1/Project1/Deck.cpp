#include "Deck.h"


void Deck::SwitchToAnotherTextureCards(std::mutex& mutex) {

    index++;
    if (index > PathsToCards->size() - 1)
        index = 0;

    std::string backPath = PathsToCards[index] + "card back red.png";

    for (auto card : cards) {

        std::string frontPath = PathsToCards[index] + std::to_string(card.GetScore()) + "_of_" + suits[FromSuitToNumber(card.GetSuit())] + ".png";


        if (index == 1)
            std::string frontPath = PathsToCards[index] + "alternative.png";
        SDL_Surface* surfFront = IMG_Load(frontPath.c_str());
        SDL_Surface* surfBack = IMG_Load(backPath.c_str());


        card.ChangeTextures(SDL_CreateTextureFromSurface(renderer, surfFront), SDL_CreateTextureFromSurface(renderer, surfBack));




    }

}

void  Deck::AddCard(std::mutex& mutex)
{

    mutex.lock();

    PlayingCard  cardToAdd;
    SDL_Surface* surfF, * surfB;

    std::uniform_int_distribution<size_t> cardScore(2, 14);
    std::uniform_int_distribution<size_t> cardSuit(0, 3);
    int cardScoreNum, cardSuitNum;
    while (true) {

        cardScoreNum = cardScore(gen);
        cardSuitNum = cardSuit(gen);

        std::string f = PathsToCards[index] + std::to_string(cardScoreNum) + "_of_" + suits[cardSuitNum] + ".png";
        std::string b = PathsToCards[index] + "card back red.png";
        if (index == 1) {
            f = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/alternative.png";
            b = "E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/alternative.png";
        }



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


    mutex.unlock();


}

void  Deck::ReloadDeck() {

    cards.clear();
}

int  Deck::FromSuitToNumber(Suit suit_) {
    switch (suit_)
    {
    case Suit::Clubs: {
        return 1;
    }
    case Suit::Diamonds: {
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
Suit  Deck::FromNumberToSuit(int num) {

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


void  Deck::SetRenderer(SDL_Renderer* renderer_) {
    renderer = renderer_;
}

Deck::Deck()
{

    std::random_device rd;
    gen = std::mt19937(rd());
}

void  Deck::ShowDeck() const
{
    for (const auto& card : cards) {
        card.Display();
    }
}

void  Deck::ShuffleDeck()
{
    std::shuffle(cards.begin(), cards.end(), gen);
}

PlayingCard  Deck::pickCard()
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
