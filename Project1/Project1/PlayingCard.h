
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
	SDL_Surface* textureFront = nullptr;
	SDL_Surface* textureBack = nullptr;
	
public:


	
	std::pair<SDL_Surface*, SDL_Surface* > GetSurfaces(){
		
		return std::make_pair(textureFront, textureBack);
	}
	

	PlayingCard()
		: cardScore(0)
	{}


	PlayingCard(short int cardScore_, SDL_Surface* textureFront_, SDL_Surface* textureBack_)
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

	void Display() const { std::cout << "Card: " << cardScore;
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
			// вернуть текстуру єтой масти
		}
		if (suit == Suit::Diamonds) {
			// вернуть текстуру єтой масти
		}
		if (suit == Suit::Hearts) {
			// вернуть текстуру єтой масти
		}
		if (suit == Suit::Clubs) {
			// вернуть текстуру єтой масти
		}
	}
	*/

};

