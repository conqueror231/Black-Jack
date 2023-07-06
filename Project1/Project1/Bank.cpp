#include "Bank.h"


void Bank::ShowTotalMoney() const {
	std::cout << "Bank: " << totalMoney;
}

void Bank::GiveMoney(int money, Player& player) {
	totalMoney -= money;
	player.TakeMoney(money * 2);
}

