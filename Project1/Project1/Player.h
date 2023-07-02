#pragma once
#include"Hand.h"
#include"Bank.h"
class Player
{
private:
	Hand *hand = new Hand;
	int money;
	bool isStanding = false;
	bool isBusted = false;
public:
	std::vector<PlayingCard>* GetCardInstance() {
		return hand->GetCardInstance();

	}


	Player(int money_)
		: money(money_)
	{

	}

	bool getStanding() {
		return isStanding;
	}

	void setStanding() { isStanding = true; };
	void Bet(int money, Bank bank) {

		bank.SendMoney(money);
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

