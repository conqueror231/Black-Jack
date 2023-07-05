#include "Bank.h"


void Bank::ShowTotalMoney() const {
	std::cout << "Bank: " << totalMoney;
}

void Bank::GiveMoney(int money, Player& player) {
	player.TakeMoney(money * 2);
}