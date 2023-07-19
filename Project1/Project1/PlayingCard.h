
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
	void ChangeTextures(SDL_Texture* frontText, SDL_Texture* backText);
	void DeleteTextures();
	std::pair<SDL_Texture*, SDL_Texture* > GetTexture();
	PlayingCard();
	PlayingCard(short int cardScore_, Suit suit_, SDL_Texture* textureFront_, SDL_Texture* textureBack_);
	bool getIsUsing() const;
	void setIsUsing(bool value);
	void SetSuit(Suit suit_);
	Suit GetSuit();
	void Display() const;
	int GetScore() const;


	
	

};
