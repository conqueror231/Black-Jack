#pragma once
#include<iostream>
class Bank
{
private: 
	int totalMoney = 0;
public: 
	void ShowTotalMoney() const {
		std::cout << "Bank: " << totalMoney;
	};
	void SendMoney(int money) {
		totalMoney += money;
	}

};

