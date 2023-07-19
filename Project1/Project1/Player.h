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

	void SetLost(bool lost);
	bool GetLost();
	void Reload();
	int GetMoney() const;
	void TakeMoney(int money_);
	bool GetStanding() const;
	void SetStanding();
	void BetMoney(int money_);

};