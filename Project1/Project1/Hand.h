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
	void Reload() {
		score = 0;
		secondScore = 0;
		cards.clear();
	}

	std::vector<PlayingCard>* GetCardInstance() {
		return &cards;

	}


	void ShowHand() {

		std::cout << "Cards in hand:" << std::endl;
		for (const auto& card : cards) {
			card.Display();
		}

	}

	PlayingCard TakeCard() {
		PlayingCard pickedCard = Deck::GetInstance().pickCard();



		cards.push_back(pickedCard);

		score += pickedCard.GetScore();
		secondScore += pickedCard.GetScore();

		if (pickedCard.GetScore() == 11)
			secondScore += 1;

		return pickedCard;
	}


	void ClearHand() {
		while (!cards.empty()) {
			cards.front().setIsUsing(false);
			cards.pop_back();
		}
	}

	std::pair<int, int> GetScore() const {
		std::pair<int, int> temp(score, secondScore);
		return temp;
	}

};
