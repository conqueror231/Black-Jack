#pragma once
#include "Hand.h"

class BasePlayer
{
protected:
	Hand* hand = new Hand;
	bool isBusted = false;

public:
	std::vector<PlayingCard>* GetCardInstance()
	{
		return hand->GetCardInstance();
	}

	std::pair<int, int> ShowScore() const
	{
		return hand->GetScore();
	}

	void ShowHand() const
	{
		hand->ShowHand();
	}

	void TakeCardToHand()
	{
		hand->TakeCard();
	}

	void ClearHand()
	{
		hand->ClearHand();
	}

	void SetBusted()
	{
		isBusted = true;
	}

	bool GetBusted() const
	{
		return isBusted;
	}
};

