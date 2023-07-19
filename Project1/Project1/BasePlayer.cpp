#include "BasePlayer.h"

std::vector<PlayingCard>* BasePlayer::GetCardInstance()
{
	return hand->GetCardInstance();
}

std::pair<int, int> BasePlayer::ShowScore() const
{
	return hand->GetScore();
}

void BasePlayer::ShowHand() const
{
	hand->ShowHand();
}

void BasePlayer::TakeCardToHand()
{
	hand->TakeCard();
}

void BasePlayer::ClearHand()
{
	hand->ClearHand();
}

void BasePlayer::SetBusted()
{
	isBusted = true;
}

bool BasePlayer::GetBusted() const
{
	return isBusted;
}