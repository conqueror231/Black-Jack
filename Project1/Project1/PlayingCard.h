
#pragma once

#include <iostream>

#include"Suit.h"
#include<SDL.h>

class PlayingCard
{
private:
	short int cardScore;
	Suit suit;
	bool isUsing = false;
	SDL_Texture* textureFront = nullptr;
	SDL_Texture* textureBack = nullptr;

public:



	std::pair<SDL_Texture*, SDL_Texture* > GetTexture() {

		return std::make_pair(textureFront, textureBack);
	}


	PlayingCard()
		: cardScore(0)
	{}


	PlayingCard(short int cardScore_, SDL_Texture* textureFront_, SDL_Texture* textureBack_)
		: cardScore(cardScore_), textureFront(textureFront_), textureBack(textureBack_)
	{}


	bool getIsUsing() const {
		return isUsing;
	}


	void setIsUsing(bool value) {
		isUsing = value;
	}

	void SetSuit(Suit suit_) {
		suit = suit_;
	}

	void Display() const {
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
	int GetScore() const { return cardScore; };
	/*
	void GetTexture(Suit suit){
		if (suit == Suit::Spades) {
			// ������� �������� ���� �����
		}
		if (suit == Suit::Diamonds) {
			// ������� �������� ���� �����
		}
		if (suit == Suit::Hearts) {
			// ������� �������� ���� �����
		}
		if (suit == Suit::Clubs) {
			// ������� �������� ���� �����
		}
	}
	*/

};
