#include "Player.hpp"

#include "Utility.hpp"

Player::Player()
{
    // teoretycznie mapa moze stworzyc, ale sie nie ma prawa zdarzyc... moze jakis throw?
}
Player::Player(unsigned s,std::string n)
{
    seat = s;
    cash_in_game = 0;
    cash_available = 0;
    bet_this_turn = 0;
    is_playing = false;
    is_active = false;
    all_in = false;
    pause = true;
    rewarded = false;
    left = false;
    name = n;
}
void Player::bet(unsigned cash, unsigned& update_bet_this_turn)
{
#if DEBUGLEV >= 3
    std::cout << (Sync() << name << " betting " << cash << "\n");
#endif
    cash_available -= cash;
    cash_in_game += cash;
    bet_this_turn += cash;
    update_bet_this_turn = std::max(update_bet_this_turn,bet_this_turn);
}
