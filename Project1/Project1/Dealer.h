#pragma once
#include "BasePlayer.h"
#include "Hand.h"
#include "Deck.h"
#include "Player.h"

class Dealer : public BasePlayer
{
public:
	void Reload();
	void DealCards(Player& player, int numCards);
	void DealCards(Dealer& dealer, int numCards);
	void CollectCards(Player& player);
	void SetBusted();
};