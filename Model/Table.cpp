#include "Table.hpp"

#include <iostream>
#include <stdlib.h>  
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"
#include <boost/limits.hpp>

//#include <boost/thread/locks.hpp>

Table::Table()
{
        game_data.in_game = false;
	//game_data.last_raise = 0;
	game_data.turn_min_bet = ANTE;
        //main_loop = boost::thread(boost::bind(&Table::prv_mainLoop, this));
}
Table::~Table()
{
        stopTable();
        //main_loop.join();
}

void Table::stopTable()
{
        //if(!running) return;
        //running = false;
        game_data.in_game = false;
	players.clear();
        //main_loop.interrupt();
}

Table::GameData Table::getGameData()
{
        //boost::shared_lock<boost::shared_mutex> lock(data_mutex);
        return game_data;
}

unsigned Table::pickCard()
{
	boost::mt19937 randGen(static_cast<unsigned>(std::time(0)));
	boost::uniform_int<> intDist(0, std::numeric_limits<int>::max());
	boost::variate_generator< boost::mt19937&, boost::uniform_int<> > GetRand(randGen, intDist);
	
	//losuj indeks karty do zwrocenia
	unsigned card = static_cast<unsigned>(GetRand()) % cards_left;
	//pobierz karte z wektora
        unsigned ret = deck[card];
	//przenies ostatnia karte na miejsce zwracanej
        deck[card] = deck[--cards_left];
        return ret;
}

void Table::initDeck()
{
        cards_left = DECK_SIZE;
        deck.clear();
        for(unsigned i = 0; i < DECK_SIZE; ++i) {
                deck.push_back(i);
        }
}

void Table::newGame()
{
	game_data.in_game = true;
	game_data.turn_min_bet = ANTE;
	//game_data.last_raise = 0;
	game_data.start_player = abs(game_data.start_player-1); //teraz zaczyna inny gracz
	game_data.player_turn = game_data.start_player;
	//wyczysc tabele wygranych
	winners.clear();

	initDeck();

	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->is_active = false;
		it->all_in = false;
		it->bet_this_turn = 0;
	}

	activate_player(game_data.player_turn);

	//pobierz ante
	for(unsigned i=0; i<MAX_SEATS; ++i){
		if(players[i].cash_available >= ANTE)
		  playerRaise(players[i].seat, ANTE);
		else
		  playerAllIn(i);	    
	}

	game_data.state = START; //poczatek gry - po ante

	//rozdaj nowe karty
	std::vector<unsigned> v;
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		v.clear();
		if(it->is_playing) {
		  for(int i=0; i<5; ++i) {
		    v.push_back(pickCard());
		  }
		  it->hand.set(v);
		}
	}
}

void Table::activate_player(unsigned s)
{
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
	    if(it->seat == s)
	      it->is_active = true;
	    else
	      it->is_active = false;
	}
}

bool Table::playerRaise(unsigned s,unsigned raise) //przebicie
{
	unsigned ind = 2;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == s) {
	       ind = i;
	       break;
	    }
	}
	if(ind == 2){
	  return false;
	}
	if(raise > game_data.turn_min_bet && players[ind].bet_this_turn + players[ind].cash_available > raise) {
	    game_data.turn_min_bet = raise;
	    players[ind].bet(raise - players[ind].bet_this_turn, players[ind].bet_this_turn);
	    //game_data.last_raise = players[ind].seat;
	    //zaznacz, że wykonal ruch, drugiemu ustaw na false
	    resetActions(ind);
	}
	else {
		return false;
	}
	//teraz ruch drugiego gracza
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	checkNextState();
	return true;
}

bool Table::playerCheck(unsigned s) //sprawdzenie
{
	unsigned ind = 2;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == s) {
	       ind = i;
	       break;
	    }
	}
	if(ind == 2){
	  return false;
	}
	unsigned bet_now = 0;
	//jesli starcza mu na sprawdzenie
	if(players[ind].cash_available + players[ind].bet_this_turn >= game_data.turn_min_bet) {
		bet_now = game_data.turn_min_bet - players[ind].bet_this_turn;
		players[ind].bet(bet_now, players[ind].bet_this_turn);
		players[ind].took_action = true;
	}
	else {
		return false;
	}
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	checkNextState();
	return true;
}

bool Table::playerFold(unsigned s)
{
	unsigned ind = 2;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == s) {
	       ind = i;
	       break;
	    }
	}
	if(ind == 2){
	  return false;
	}
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	players[ind].is_playing = false;
	//jesli pasujacy, jako ostatni przebijal, ustaw drugiego jako ostatnio przebijajacego
	/*if(players[ind].seat == game_data.last_raise) {
		game_data.last_raise = abs(game_data.last_raise -1);
	}*/
	checkNextState();
	return true;
}

bool Table::playerAllIn(unsigned s)
{
	unsigned ind = 2;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == s) {
	       ind = i;
	       break;
	    }
	}
	if(ind == 2){
	  return false;
	}
	players[ind].all_in = true;
	//jesli gracz wszystko stawia, ale wcale nie musi
	if(players[ind].cash_available + players[ind].bet_this_turn > game_data.turn_min_bet) {
		//game_data.last_raise = abs(game_data.player_turn-1);
		resetActions(ind);
	}
	players[ind].bet(players[ind].cash_available, game_data.turn_min_bet);
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	checkNextState();
	return true;
}

void Table::resetActions(unsigned new_act) {
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->took_action = (new_act == it->seat);
	}
}

void Table::checkNextState()
{
	//nic nie zmieniaj, jesli ktorys z graczy się nie ruszył (może się nie ruszać tylko, jeśli wcześniej wszedl za wszystko
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->is_playing && !it->all_in && !it->took_action) {
			return;
		}
	}
	//reset minimalnej stawki zakladu
	game_data.turn_min_bet = 0;
	//reset zakladu kazdego z graczy
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->bet_this_turn = 0;
	}
	
	/*
	//aktywuj gracza, ktory ostatni przebijal
	activate_player(game_data.last_raise);
	//ruch gracza, który ostatnio przebijal
	player_turn = game_data.last_raise;
	resetActions(game_data.last_raise);*/
	
	//resetuj akcje graczy - czy wykonali ruch, obaj nie wykonali ruchu
	resetActions(2);
	//przejdz do nastepnego stanu
	game_data.state++;
	if(game_data.state == END) {
	    endGame();
	}

	//ilu graczy dalej gra
	unsigned players_betting = 0;
	  for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		players_betting += (it->is_playing && !it->all_in);
	  }
	  //jeśli został tylko jeden gracz lub zaden, to koniec
	  if(players_betting <= 1) {
		game_data.state++;
		if(game_data.state == END) {
		    endGame();
		}
	  }
}

void Table::endGame()
{
	
	std::vector<unsigned> leavers; //wektor z numerami przegranych graczy

	//std::cout<< "* Ending game" << ", winners:\n";

	//dobyte nagrody
	unsigned prize;
	//moze byc wektor jedno lub dwu-elementowy
	
	getWinners();
	prize = 0;
	//pobierz pule ze stolu (wklad kazdego z graczy)
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {

		//std::cout<< "* "<< it->cash_in_game << " own cash in game\n";
		prize += it->cash_in_game;
		it->cash_in_game = 0;
	}

	//jeden wygrany zbiera cala pule, drugi moze odpada
	if(winners.size() == 1)
	{
	   unsigned ind;
	   for(unsigned i=0; i<MAX_SEATS; ++i){
	   if(players[i].seat == winners[0]) {
	       ind = i;
	       break;
	      }
	  }
	  players[ind].cash_available += prize;
	  //jesli drugi gracz zbankrutowal, usun z gry
	  for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {

		if(it->cash_available == 0)
		//std::cout<< "* usuniety z gry:"<< it->name << "\n";
		players.erase(it);
	  }
	}
	//pula dzielona na pol, jesli dwoch wygralo lub dwoch spasowalo(mozliwe?)
	else if(winners.size() == 2 || winners.empty())
	{
	  players[winners[0]].cash_available += prize / 2;
	  players[winners[1]].cash_available += prize / 2;
	}
	else //nie powinno sie nigdy zdarzyc!
	{
	  players.clear();
	}
	
	game_data.in_game = false;
	winners.clear();
	
	//moze sie zaczac nowa gra, ew. mozna dodać gracza, jesli zostal tylko jeden
}

void Table::getWinners()
{
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->is_playing) {
			winners.push_back(it->seat);
		}
	}
	if(winners.empty())
	  return;
	//jesli dwoch graczy zostalo, to porownujemy uklady
	if(winners.size() > 1) {
	  if(players[0].hand == players[1].hand) //remis
	  {
	    	winners.clear();
		winners.push_back(0);
		winners.push_back(1);
	  }
	  else if(players[0].hand < players[1].hand) {
		winners.clear();
		winners.push_back(1);
	  }
	  else{
	    	winners.clear();
		winners.push_back(0);
	  }
	}
}

bool Table::addPlayer()
{
    if(players.size() < MAX_SEATS) {    
      //pobierz nr siedzenia (i tym samym nazwe)
      unsigned s = abs(players[0].seat - 1);
      players.push_back(Player(s));
      return true;
    }
    return false;
}

bool Table::playerChange(unsigned seat, std::vector<unsigned> cards){
  	unsigned ind = 2;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == seat) {
	       ind = i;
	       break;
	    }
	}
	if(ind == 2 || cards.size() > 5){
	  return false;
	}
	
	//pobierz aktualna reke
	std::vector<unsigned> v = players[ind].hand.getCards();
	
	//wymien odpowiednie
	for(unsigned i=0; i<cards.size(); ++i) 
	{
	    for(unsigned i=0; i<v.size(); ++i)
	    {
	      if(cards[i]==v[i]) //zamien
	      {
		v[i] = pickCard();
		deck.push_back(cards[i]);
		++cards_left;
		break;
	      }
	    }
	}
	//ustaw nowa reke
	players[ind].hand.set(v);
	players[ind].took_action = true;
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	return true;
}
