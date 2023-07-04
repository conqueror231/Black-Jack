#pragma once
#include<iostream>
class Bank
{
private: 
	int totalMoney = 0;
public: 
	static Bank& GetInstance()
	{

		static Bank instance;
		return instance;
	}

	int GetTotalMoney() {
		return totalMoney;
	};
	void SendMoney(int money) {
		totalMoney += money;
	}

};

