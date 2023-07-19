#pragma once
#include"PlayingCard.h"
#include"deck.h"
#include<vector>
class Hand
{
private:
	int score;
	int secondScore;
	std::vector <PlayingCard> cards;

public:
	void Reload();
	void ShowHand();
	PlayingCard TakeCard();
	void ClearHand();
	std::pair<int, int> GetScore() const;
	std::vector<PlayingCard>* GetCardInstance();



	
	

};
