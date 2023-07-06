#pragma once
#include "BasePlayer.h"
#include "Hand.h"
#include "Deck.h"
#include "Player.h"

class Dealer : public BasePlayer
{
public:
	void Reload()
	{
		hand->Reload();
		isBusted = false;
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

	void SetBusted()
	{
		isBusted = true;
	}
};
