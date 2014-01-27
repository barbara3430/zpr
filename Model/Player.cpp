#include "Player.hpp"
#include <iostream>

Player::Player()
{
    // nie powinno sie zdarzyc
}

Player::Player(unsigned s, std::string n)
{
    seat = s;
    name = n;
    cash_in_game = 0;
    cash_available = START_BUDGET;
    bet_this_turn = 0;
    is_playing = false;
    is_active = false;
    all_in = false;
    took_action = false;
}

void Player::bet(unsigned cash, unsigned& update_bet_this_turn)
{
    std::cout<< name << " betting " << cash << "\n";
    cash_available -= cash;
    cash_in_game += cash;
    bet_this_turn += cash;
    //aktualizacja aktualnej stawki zakladu
    update_bet_this_turn = std::max(update_bet_this_turn,bet_this_turn);
}

void Player::printPlayer() const
{
  std::cout << "name: "<<name <<std::endl;
  std::cout << "seat: "<<seat <<std::endl;
  std::cout << "cash_available: "<<cash_available <<std::endl;
  std::cout << "cash in game: "<<cash_in_game <<std::endl;
  std::cout << "bet_this_turn: "<<bet_this_turn <<std::endl;
  std::cout << "is_playing: "<<is_playing <<std::endl;
  std::cout << "is_active: "<<is_active <<std::endl;
  std::cout << "all_in: "<<all_in <<std::endl;
  std::cout << "took_action: "<<took_action <<std::endl;
  hand.printHand();
}