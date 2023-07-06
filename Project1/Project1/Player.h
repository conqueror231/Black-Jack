#pragma once
#include"BasePlayer.h"

class Player : public BasePlayer
{
private:
	int money;
	bool isStanding = false;
	bool Lost = false;

public:
	Player(int money_) : money(money_) {}

	void SetLost(bool lost) {
		Lost = lost;
		
	}

	bool GetLost()
	{
		return Lost;
	}

	void Reload()
	{
		hand->Reload();
		isStanding = false;
		isBusted = false;
		Lost = false;
	}

	int GetMoney() const
	{
		return money;
	}

	void TakeMoney(int money_)
	{
		money += money_;
	}

	

	bool GetStanding() const
	{
		return isStanding;
	}

	void SetStanding()
	{
		isStanding = true;
	}

	void BetMoney(int money_)
	{
		money -= money_;
	}

};