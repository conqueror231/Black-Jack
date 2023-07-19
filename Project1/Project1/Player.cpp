#include "Player.h"
#include "Bank.h"


void Player::SetLost(bool lost) {
	Lost = lost;

}

bool Player::GetLost()
{
	return Lost;
}

void Player::Reload()
{
	hand->Reload();
	isStanding = false;
	isBusted = false;
	Lost = false;
}

int Player::GetMoney() const
{
	return money;
}

void Player::TakeMoney(int money_)
{
	money += money_;
}

bool Player::GetStanding() const
{
	return isStanding;
}

void Player::SetStanding()
{
	isStanding = true;
}

void Player::BetMoney(int money_)
{
	money -= money_;
}
