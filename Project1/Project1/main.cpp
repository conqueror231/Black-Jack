#include <SDL.h>
#include <SDL_image.h>
#include<iostream>
#include<list>
#include"PlayingCard.h"
#include"Deck.h"
#include"Bank.h"
#include<queue>
#include"Player.h"
#include"Dealer.h"
#include<thread>
#include"SDL_ttf.h"
#include<Windows.h>
#include<fstream>
SDL_Texture* ShowText(SDL_Color color, const char * text);

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int CARD_WIDTH = 100;
const int CARD_HEIGHT = 150;

SDL_Renderer* renderer = nullptr;
//Distanation For Dealer {}
SDL_Rect destinationRectForPlayer{ 200, 400, 150, 200 };
SDL_Rect destinationRectForDealer{ 200, 100, 150, 200 };

std::vector<std::pair<SDL_Rect,SDL_Rect>> Rects;
std::vector<std::pair<SDL_Texture*, SDL_Texture*>> Textures;


Dealer dealer;
Player player(500);
std::vector<PlayingCard>* dealerCardsPtr;
std::vector<PlayingCard>* playerCardsPtr;


int PlayerCardIndex = 0;
int DealerCardIndex = 0;
class dispenser {
public:
	void TakeCard(Player player_, int countOfCards_) {
		dealer.DealCards(player_, countOfCards_);

		for (int i = 0; i < countOfCards_; i++) {
			std::pair< SDL_Rect, SDL_Rect> temp({ 500, 200, 150, 200 }, { destinationRectForPlayer.x + 30 * PlayerCardIndex , destinationRectForPlayer.y,150,200});
			
			Rects.push_back(temp);
			Textures.push_back(playerCardsPtr->at(PlayerCardIndex).GetTexture());
			PlayerCardIndex++;
		}

	};
	void TakeCard(Dealer dealer_, int countOfCards_) {
		dealer.DealCards(dealer_, countOfCards_);
	
		for (int i = 0; i < countOfCards_; i++) {
			std::pair< SDL_Rect, SDL_Rect> temp({ 500, 200, 150, 200 }, { destinationRectForDealer.x + 30 * DealerCardIndex, destinationRectForDealer.y, 150,200 });
			Rects.push_back(temp);
			Textures.push_back(dealerCardsPtr->at(DealerCardIndex).GetTexture());
			DealerCardIndex++;
		}

	};


};

double x = 1, y = 1;

void MoveCard();

bool isMoving = true;
int indexTexture = 0;
bool isRunning = true;

	

void RenderThreadFunction(SDL_Renderer* renderer) {

	while (true) {

		if (isRunning == false)
			break;
		SDL_RenderClear(renderer);


		MoveCard();


		TTF_Font* font = TTF_OpenFont("E:/02 c++/01 myProjects/Black-Jack/Project1/Project1/PNG-cards-1.3/lazy.ttf", 24);
		SDL_Color textColor = { 255, 255, 255, 255 }; // Белый цвет текста

		SDL_Surface* textSurface = TTF_RenderText_Solid(font, "100", textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		SDL_Rect textRect;
		textRect.x = 100;
		textRect.y = 100;
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;
		
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);


		SDL_Texture* textScorePlayer = ShowText({ 0,0,225 }, {(char*)player.ShowScore().first});

		SDL_Texture* textScoreDealer = ShowText({ 0,0,225 }, { (char*)dealer.ShowScore().first });

		int time = 0;
		for (auto it : Textures) {
			if (it == dealerCardsPtr->at(dealerCardsPtr->size() - 1).GetTexture()) {
				SDL_RenderCopy(renderer, Textures[time].second, nullptr, &Rects[time].first);
				SDL_RenderCopy(renderer, textScoreDealer, nullptr, &Rects[time].first);
			}
			else
				SDL_RenderCopy(renderer, Textures[time].first, nullptr, &Rects[time].first);

			if (it == playerCardsPtr->at(playerCardsPtr->size() - 1).GetTexture()) {
				
				SDL_Rect rect{ 100,100,100,100 };
				SDL_RenderCopy(renderer, textScorePlayer, nullptr, &rect);
			}

			if (time == indexTexture)
				break;
			time++;
		}
		time = 0;

		
		// Оновлення відображення
		SDL_RenderPresent(renderer);
		if (isMoving == false) {
			if (indexTexture < Rects.size() - 1) {

				indexTexture++;
				isMoving = true;
			}


		}
		
	}
}





int main(int argc, char* argv[])

//int SDL_main(int argc, char* argv[]) 
{
	
	SDL_Init(SDL_INIT_VIDEO);
	if (TTF_Init() == -1)
	{
		// Помилка при ініціалізації SDL_ttf
		std::cout<<"Помилка при ініціалізації SDL_ttf: " << TTF_GetError() <<std::endl;
		return -1; // або обробіть помилку за вашими потребами
	}

	

	
	

	// Создание окна
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	

	Deck& deck = Deck::GetInstance();
	deck.SetRenderer(renderer);
	deck.InitDeck();


	
	playerCardsPtr = player.GetCardInstance();
	dealerCardsPtr = dealer.GetCardInstance();
	

	
	Bank bank;

	SDL_Event event;

	dispenser disp;

	disp.TakeCard(dealer, 2);
	disp.TakeCard(player, 2);



	SDL_Rect StartRect{ 500, 200, 150, 200 };

	SDL_SetRenderDrawColor(renderer, 0, 0, 100, 100);

	std::thread renderThread(RenderThreadFunction, renderer);

	std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS

	bool isPlayerCardDealt = false;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
				renderThread.join();
			}

		
		}



		

	}


	


		/*
		Player player(500);
		Bank bank;

		Dealer dealer;


		dealer.DealCards(player, 2);
		dealer.DealCards(dealer, 2);
		player.Bet(200, bank);

		std::cout << "Player Hand:" << std::endl;
		player.ShowHand();

		std::cout << "Dealer Hand:" << std::endl;
		dealer.ShowHand();

		while (!player.getStanding()) {
			std::cout << "Hit or Stand (1 - Hit, 2 - Stand): ";
			int choice;
			std::cin >> choice;

			if (choice == 1) {
				dealer.DealCards(player, 1);
				std::cout << "Player Hand:" << std::endl;
				player.ShowHand();

				if (player.ShowScore().first > 21) {
					player.SetBusted();
					break;
				}
			}
			else if (choice == 2) {
				player.setStanding();
				std::cout << "Player Stands" << std::endl;
			}
		}

		dealer.DealCards(dealer);
		std::cout << "Dealer Hand:" << std::endl;
		dealer.ShowHand();

		std::cout << "Player Hand:" << std::endl;
		player.ShowHand();


		//Win Logic

		if (!player.GetBusted()) {
			if (dealer.GetBusted()) {
				std::cout << "Player Wins!" << std::endl;
			}
			else if (player.ShowScore() > dealer.ShowScore() || player.ShowScore().second == 21) {
				std::cout << "Player Wins!" << std::endl;
			}
			else {
				std::cout << "Dealer Wins!" << std::endl;
			}
		}
		else {
			std::cout << "Player Busted. Dealer Wins!" << std::endl;
		}
		// Ваш код логіки гри/відображення тут
	}
	
	
	*/
	/*
	if (player.GetBusted() == false) {
		if(dealer.GetBusted())
			std::cout << "Player won";
		else {
			if (player.ShowScore() > dealer.ShowScore() || player.ShowScore().first == 21) {
				std::cout << "Player won";
			}
		}
	}
	else {

		std::cout << "Player lost";
	}

	*/
	


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
	
}

SDL_Texture* ShowText(SDL_Color color, const char * text) {

	TTF_Font* font = TTF_OpenFont("E:/02 c+/01 myProjects/04 Black Jack/Project1/Project1/fonts/lazy.ttf", 30);

	SDL_Color textColor = color; 
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	return textTexture;
}



void MoveCard() {
	if (isMoving) {
		if (Rects[indexTexture].first.x < Rects[indexTexture].second.x) {
			x += 0.001;
			Rects[indexTexture].first.x += x;
		}
		if (Rects[indexTexture].first.x > Rects[indexTexture].second.x ) {
			x -= 0.001;
			Rects[indexTexture].first.x += x;
		}
		if (Rects[indexTexture].first.y < Rects[indexTexture].second.y) {
			y += 0.001;
			Rects[indexTexture].first.y += y;
		}
		if (Rects[indexTexture].first.y > Rects[indexTexture].second.y) {
			y -= 0.001;
			Rects[indexTexture].first.y += y;
		}

		if (std::abs(Rects[indexTexture].first.x - Rects[indexTexture].second.x) < 0.1 && std::abs(Rects[indexTexture].first.y - Rects[indexTexture].second.y) < 0.1) {
			isMoving = false;
			x = 1, y = 1;

		}
	}
}
