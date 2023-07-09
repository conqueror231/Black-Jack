#pragma once
#include "Player.h"
#include<iostream>

class Bank
{
private:
	int totalMoney = 0;
public:
	void ShowTotalMoney() const;
	static Bank& GetInstance()
		{

			static Bank instance;
			return instance;
		}

	int GetTotalMoney();

	void SendMoney(int money, Player& player);

	
	void GiveMoney(int money, Player& player);
	
	void ReloadBank() {
		totalMoney = 0;
	}

	};

	
