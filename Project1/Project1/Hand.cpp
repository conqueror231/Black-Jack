#include "Hand.h"

void Hand::Reload() {
	score = 0;
	secondScore = 0;

	for (auto card : cards) {
		card.DeleteTextures();
	}

	cards.clear();


}

void Hand::ShowHand() {

	std::cout << "Cards in hand:" << std::endl;
	for (const auto& card : cards) {
		card.Display();
	}

}

PlayingCard Hand::TakeCard() {
	PlayingCard pickedCard = Deck::GetInstance().pickCard();



	cards.push_back(pickedCard);

	score += pickedCard.GetScore();
	secondScore += pickedCard.GetScore();

	if (pickedCard.GetScore() == 11)
		secondScore += 1;

	return pickedCard;
}

void Hand::ClearHand() {
	while (!cards.empty()) {
		cards.front().setIsUsing(false);
		cards.pop_back();
	}
}

std::pair<int, int> Hand::GetScore() const {
	std::pair<int, int> temp(score, secondScore);
	return temp;
}

std::vector<PlayingCard>* Hand::GetCardInstance() {
	return &cards;
}