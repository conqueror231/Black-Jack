#pragma once
#include"Hand.h"


class Player
{
private:
	Hand *hand = new Hand;
	int money;
	bool isStanding = false;
	bool isBusted = false;
	bool Lost = false;
public:
	bool GetLost() {
		return Lost;
	}
	bool SetLost() {
		return Lost;
	}
	void Reload() {
		hand->Reload();
		isStanding = false;
		isBusted = false;

	}

	std::vector<PlayingCard>* GetCardInstance() {
		return hand->GetCardInstance();

	}

	int GetMoney() {
		return money;
	}
	void TakeMoney(int money_) {
		money += money_;
	}


	Player(int money_)
		: money(money_)
	{

	}

	bool getStanding() {
		return isStanding;
	}

	void setStanding() { isStanding = true; };


	void BetMoney(int money_) {

		money -= money_;

	}

	void TakeCardToHand() {
		hand->TakeCard();
	}
	void ClearHand() {
		hand->ClearHand();
	}

	void ShowHand() const {
		hand->ShowHand();
	}
	void SetBusted() {
		isBusted = true;
	}
	bool GetBusted() {
		return isBusted;
	}
	std::pair<int,int> ShowScore() const {
		return hand->GetScore();
	}


};

