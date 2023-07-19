#include "Dealer.h"

void Dealer::Reload()
{
	hand->Reload();
	isBusted = false;
}

void Dealer::DealCards(Player& player, int numCards)
{
	for (int i = 0; i < numCards; i++)
	{
		player.TakeCardToHand();
	}
}

void Dealer::DealCards(Dealer& dealer, int numCards)
{
	for (int i = 0; i < numCards; i++)
	{
		hand->TakeCard();
	}
}

void Dealer::CollectCards(Player& player)
{
	player.ClearHand();
}

void Dealer::SetBusted()
{
	isBusted = true;
}