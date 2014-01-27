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
	game_data.turn_min_bet = ANTE;
	game_data.start_player = 0;
	game_data.player_turn = 0;
	game_data.state = 1;
	game_data.all_in_bet = 0;
}
Table::~Table()
{
        stopTable();
}

void Table::stopTable()
{
    game_data.in_game = false;
	players.clear();
	game_data.turn_min_bet = 0;
	game_data.start_player = 0;
	game_data.player_turn = 0;
	game_data.state = 1;
}

Table::GameData Table::getGameData() const
{
        return game_data;
}

const std::vector<Player>& Table::getPlayers() const
{
        return players;
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

bool Table::newGame()
{
	if(players.size() != MAX_SEATS) //za mało graczy
	  return false;
	game_data.state = START;
	game_data.in_game = true;
	game_data.turn_min_bet = ANTE;
	game_data.all_in_bet = 0;
	game_data.start_player = abs(game_data.start_player-1); //teraz zaczyna inny gracz
	game_data.player_turn = game_data.start_player;
	//wyczysc tabele wygranych
	winners.clear();
	//zaden z graczy jeszcze nie wykonal ruchu
	resetActions(2);

	initDeck();

    //std::cout<<"* New Game, STAN: "<<game_data.state<<std::endl;

	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->is_active = false;
		it->all_in = false;
		it->bet_this_turn = 0;
		it->cash_in_game = 0;
		it->is_playing = true;
	}
std::cout << "3" << std::flush;
	activate_player(game_data.player_turn);

	//pobierz ante - musi byc zachowana kolejnosc
	unsigned ind;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == game_data.player_turn) {
	       ind = i;
	       break;
	    }	
	}
	playerCheck(players[ind].seat);
	playerCheck(players[abs(ind-1)].seat);   
	
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

	return true;
}

void Table::activate_player(unsigned s)
{
    std::cout<<"Player activated";
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

    //std::cout<<players[ind].name <<" raise" << " "<< raise<<std::endl;
	
	if(raise > game_data.turn_min_bet && players[ind].bet_this_turn + players[ind].cash_available > raise) {
	    game_data.turn_min_bet = raise;
	    players[ind].bet(raise - players[ind].bet_this_turn, players[ind].bet_this_turn);
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

    //std::cout<<players[ind].name <<" check" <<std::endl;
	
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

   //std::cout<<players[ind].name <<" fold" <<std::endl;
	
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	players[ind].is_playing = false;
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

    //std::cout<<players[ind].name <<" all_in" <<std::endl;
	
	players[ind].all_in = true;
	//jesli gracz wszystko stawia, ale wcale nie musi
	if(players[ind].cash_available + players[ind].bet_this_turn > game_data.turn_min_bet) {
		//game_data.last_raise = abs(game_data.player_turn-1);
		resetActions(ind);
	}
	//zapamiętaj za ile zrobił all-in
	game_data.all_in_bet = players[ind].cash_available;
	
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
	//nie zmieniaj stanu, jesli ktorys z graczy:
	//gra i nie wszedł za wszystko i nie wykonał akcji
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->is_playing && !it->all_in && !it->took_action) {
			
			//std::cout<<"NIE"<<std::endl;
			
			return;
		}
	}
	
	//resetuj akcje graczy - czy wykonali ruch, obaj nie wykonali ruchu w nowej rundzie
	resetActions(2);
	//przejdz do nastepnego stanu
    game_data.state++;
	
	//std::cout<<"NEW STATE: "<<game_data.state <<std::endl;
	
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
		endGame();
	}
}

void Table::endGame()
{
	//std::cout<< "* END GAME" << "\n";
	game_data.state = END;
	//zdobyte nagrody
	unsigned prize;
	
	getWinners();
	prize = 0;
	//pobierz pule ze stolu (wklad kazdego z graczy)
	for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
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
		
	  //jesli wygrany wszedł za wszystko, a drugi nie spasował(został w grze)
	  //daj wygranemu pulę przed all-in + jego all-in; drugo dostaje tyle za ile sprawdził
	  if(players[ind].all_in && players[abs(ind-1)].is_playing)
	  {
		  players[ind].cash_available += prize - game_data.all_in_bet;
		  players[abs(ind-1)].cash_available += game_data.all_in_bet;
	  }
	  else
	  {
	  	players[ind].cash_available += prize;
	  }
	  //jesli drugi gracz zbankrutowal (nie ma kasy na nastepna grę), usun z gry
	  for(std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
	  if(it->cash_available < ANTE + 1)
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

bool Table::addPlayer(std::string n)
{
    if(players.size() < MAX_SEATS) {
      unsigned s;
      if(players.size() == 0){
		s = 0;
	  }
      else {
		s = abs(players[0].seat - 1);
	  }
      players.push_back(Player(s, n));
      return true;
    }
    return false;
}

bool Table::playerChange(unsigned s, std::vector<unsigned> c){
	unsigned ind = 2;
	for(unsigned i=0; i<MAX_SEATS; ++i){
	    if(players[i].seat == s) {
	       ind = i;
	       break;
	    }
	}
	if(ind == 2 || c.size() > 5){
	  return false;
	}

	std::cout<< players[ind].name<< " change "<<c.size()<< " cards\n";
	/*for(unsigned i=0; i<c.size(); ++i){
	 std::cout<<c[i]<<std::endl;
	  }*/
	
	
	//pobierz aktualna reke
	std::vector<unsigned> v = players[ind].hand.getCards();

	/*std::cout<<"\n";
	for(unsigned i=0; i<v.size(); ++i){
	    std::cout<<v[i]<<std::endl;
	}
	std::cout<<"\n";*/
	
	//wymien odpowiednie
	for(unsigned i=0; i<c.size(); ++i) 
	{
		 //std::cout<<"i: "<<i<< " "<<c[i]<<std::endl;
	    for(unsigned j=0; j<v.size(); ++j)
	    {
			//std::cout<<"j: "<< j << " " <<v[j]<<std::endl;
	      if(c[i]==v[j]) //zamien odpowiednia karte
	      {
			  /*std::cout<<"--------------------------------------"<<std::endl;
			  std::cout<<"zamiana"<< " " <<c[i] << " " << v[j] << " na "<<std::endl;*/
			v[j] = pickCard();
			  /*std::cout<<v[j]<<std::endl;
			  std::cout<<"--------------------------------------"<<std::endl;*/
			break;
	      }
	    }
	}
	//zwróć karty do deck'a
	for(unsigned i=0; i<c.size(); ++i) 
	{
		deck.push_back(c[i]);
		++cards_left;
	}
	//ustaw nowa reke
	players[ind].hand.set(v);
	players[ind].took_action = true;
	game_data.player_turn = abs(game_data.player_turn-1);
	activate_player(game_data.player_turn);
	checkNextState();
	return true;
}

