#include "Bank.h"

int Bank::GetTotalMoney() {
	return totalMoney;
};

void Bank::SendMoney(int money, Player& player) {
	totalMoney += money;
	player.BetMoney(money);
}

void Bank::ShowTotalMoney() const {
	std::cout << "Bank: " << totalMoney;
}

void Bank::GiveMoney(int money, Player& player) {
	totalMoney -= money;
	player.TakeMoney(money * 2);
}

