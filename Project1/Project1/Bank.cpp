#include "Bank.h"

void Bank::ShowTotalMoney() const
{
    std::cout << "Total money in the bank: " << totalMoney << std::endl;
}

int Bank::GetTotalMoney()
{
    return totalMoney;
}

void Bank::SendMoney(int money, Player& player)
{
    if (totalMoney <= money)
    {
        totalMoney -= money;
        player.BetMoney(money);
    }
    else
    {
        std::cout << "Not enough money in the bank to send!" << std::endl;
    }
}

void Bank::GiveMoney(int money, Player& player)
{
    if (player.GetMoney() >= money)
    {
        totalMoney += money;
        player.TakeMoney(money);
    }
    else
    {
        std::cout << "Player doesn't have enough money to give!" << std::endl;
    }
}

void Bank::ReloadBank()
{
    totalMoney = 0;
}