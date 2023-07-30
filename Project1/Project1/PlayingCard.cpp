#include "PlayingCard.h"

void PlayingCard::ChangeTextures(SDL_Texture* frontText, SDL_Texture* backText) {

	textureFront = frontText;
	textureBack = backText;
}

void PlayingCard::DeleteTextures() {

	if(textureFront!= nullptr)
	SDL_DestroyTexture(textureFront);
	if (textureBack != nullptr)
	SDL_DestroyTexture(textureBack);
}

std::pair<SDL_Texture*, SDL_Texture* > PlayingCard::GetTexture() {

	return std::make_pair(textureFront, textureBack);
}


PlayingCard::PlayingCard()
	: cardScore(0)
{}


PlayingCard::PlayingCard(short int cardScore_, Suit suit_, SDL_Texture* textureFront_, SDL_Texture* textureBack_)
	: cardScore(cardScore_), suit(suit_), textureFront(textureFront_), textureBack(textureBack_)
{}


bool PlayingCard::getIsUsing() const {
	return isUsing;
}


void PlayingCard::setIsUsing(bool value) {
	isUsing = value;
}

void PlayingCard::SetSuit(Suit suit_) {
	suit = suit_;
}

Suit PlayingCard::GetSuit() {
	return suit;
}

void PlayingCard::Display() const {
	std::cout << "Card: " << cardScore;
	/*
	switch (suit) {
	case Suit::Spades:
		std::cout << "Spades" << std::endl;
		break;
	case Suit::Hearts:
		std::cout << "Hearts" << std::endl;
		break;
	case Suit::Diamonds:
		std::cout << "Diamonds" << std::endl;
		break;
	case Suit::Clubs:
		std::cout << "Clubs" << std::endl;
		break;
	}
	*/
	std::cout << std::endl;

};
int PlayingCard::GetScore() const { return cardScore; };
