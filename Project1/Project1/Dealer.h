#pragma once
#include "Player.h"
#include "Hand.h"
#include "Deck.h"

class Dealer
{
private:
    Hand *hand = new Hand;
    bool isBusted = false;
public:
    bool GetBusted() const {
        return isBusted;
    };

    std::vector<PlayingCard>* GetCardInstance() {
        return hand->GetCardInstance();

    }

    std::pair<int,int> ShowScore() const {
        return hand->GetScore();
    }

    void ShowHand() const {
        hand->ShowHand();
    }


    void DealCards(Player& player, int numCards)
    {
        for (int i = 0; i < numCards; i++)
        {
            player.TakeCardToHand();
        }
    }

    void DealCards(Dealer& dealer, int numCards)
    {
        for (int i = 0; i < numCards; i++)
        {
            hand->TakeCard();
           
        }
    }


    void CollectCards(Player& player)
    {
        player.ClearHand();
    }

    void DealCards(Dealer& dealer){
        while (hand->GetScore().first < 17)
        {
                DealCards(dealer, 1);
                if (hand->GetScore().first > 21 && hand->GetScore().second != 21) {
                    isBusted = true;
                    break;
                }
        }
        
    
    }

};
