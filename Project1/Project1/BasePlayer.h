#pragma once
#include "Hand.h"

class BasePlayer
{
protected:
	Hand* hand = new Hand;
	bool isBusted = false;

public:
	std::vector<PlayingCard>* GetCardInstance();
	std::pair<int, int> ShowScore() const;
	void ShowHand() const;
	void TakeCardToHand();
	void ClearHand();
	void SetBusted();
	bool GetBusted() const;
};