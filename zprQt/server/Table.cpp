#include "Table.hpp"

#include "AccountBase.hpp"
#include "Lobby.hpp"
#include "MessageQueue.hpp"
#include "Utility.hpp"

#include <boost/thread/locks.hpp>

Table::Table(unsigned id,unsigned enter,unsigned small)
{
#if DEBUGLEV >= 1
	std::cout << (Sync() << "* Table " << id << " (" << enter << ", " << small << ") running...\n");
#endif
	table_desc.table_id = id;
	table_desc.buy_in = enter;
	table_desc.small_blind = small;
	table_desc.game_nr = 0;
	game_data.dealer = 0;
	game_data.in_game = false;
	running = true;
	main_loop = boost::thread(boost::bind(&Table::prv_mainLoop, this));
}
Table::~Table()
{
	stopTable();
	main_loop.join();
}
void Table::stopTable()
{
	if(!running) return;

	running = false;
	game_data.in_game = false;

	while(players.size()) prv_leaveTable(players.begin()->first);

	main_loop.interrupt();

#if DEBUGLEV >= 1
	std::cout << (Sync() << "* Table " << table_desc.table_id << " stopped...\n");
#endif
}
void Table::prv_mainLoop()
{
	boost::shared_ptr<TableMessage> task;
	while(running) {
		boost::unique_lock<boost::mutex> lock(queue_mutex);
		while(!task_queue.size()) {
			queue_empty.wait(lock);
			if(!running) return;
		}
		task = task_queue.front();
		task_queue.pop();
		lock.unlock();
		prv_processOperation(task);
	}
}
void Table::addTask(boost::shared_ptr<TableMessage> message)
{
	boost::unique_lock<boost::mutex> lock(queue_mutex);
	if(!running) {
		message->return_msg(false);
		return;
	}
	task_queue.push(message);
	queue_empty.notify_one();
}
void Table::prv_processOperation(boost::shared_ptr<TableMessage> message)
{
	boost::unique_lock<boost::shared_mutex> lock(data_mutex);
	// operacje, ktore moga byc wykonywane zawsze
	switch(message->operation) {
		case TABLE_JOIN:
			message->return_msg(prv_joinTable(message->account_name));
			return;
		case TABLE_LEAVE:
			message->return_msg(prv_leaveTable(message->account_name));
			return;
		case TABLE_BUYIN:
			message->return_msg(prv_playerBuyIn(message->account_name));
			return;
		case TABLE_PAUSE:
			message->return_msg(prv_playerSetPause(message->account_name,true));
			return;
		case TABLE_UNPAUSE:
			message->return_msg(prv_playerSetPause(message->account_name,false));
			return;
		case TABLE_CHECK_NEW_GAME: // wiadomosc uzywana przez serwer
			message->return_msg(true);
			prv_checkForNewGame();
			return;
		case LIST_PLAYERS:
			for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
				prv_sendToOne(message->account_name,LIST_PLAYERS_RETURNMSG,it->first,message->id,it->second.cash_available,it->second.cash_in_game,it->second.bet_this_turn);
			}
			message->return_msg(true);
			return;
	}

	if(message->account_name != prv_activeNowName()) {
		message->return_msg(false);
		return;
	}
	// operacje, ktore moga byc wykonywane tylko w swojej kolejce
	switch(message->operation) {
		case TABLE_RAISE:
			message->return_msg(prv_playerRaise(message->account_name,message->uint1));
			return;
		case TABLE_CHECK:
			message->return_msg(prv_playerCheck(message->account_name));
			return;
		case TABLE_FOLD:
			message->return_msg(prv_playerFold(message->account_name));
			return;
		case TABLE_ALLIN:
			message->return_msg(prv_playerAllIn(message->account_name));
			return;
		default:
			message->return_msg(false);
			break;
	}
}
std::string Table::prv_activeNowName()
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.is_active) {
			return it->first;
		}
	}
	return "";
}

unsigned Table::prv_activeNowSeat()
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.is_active) {
			return it->second.seat;
		}
	}
	return 0;
}

bool Table::prv_isAtTable(std::string player)
{
	return AccountBase::getInstance().getAccount(player).getTable() == table_desc.table_id;
}
bool Table::prv_playerBuyIn(std::string name)
{
	if(!prv_isAtTable(name)) return false;
	if(players[name].cash_available != 0 || players[name].cash_in_game != 0) return false;
	AccountBase::getInstance().getAccount(name).addCash(0 - table_desc.buy_in);
	players[name].cash_available = table_desc.buy_in;
	prv_sendToAll(REFRESH_TABLE,"",0,0,0,0);
	return true;
}
unsigned Table::prv_newSeat()
{
	bool taken[MAX_SEATS];
	for(unsigned i = 0; i < MAX_SEATS; ++i) {
		taken[i] = false;
	}
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		taken[it->second.seat] = true;
	}
	for(unsigned i = 0; i < MAX_SEATS; ++i) {
		if(!taken[i]) return i;
	}
	throw;
}
bool Table::prv_joinTable(std::string name)
{
	if(AccountBase::getInstance().getAccount(name).getTable()) {
		return false;
	}
	else if(players.size() > MAX_SEATS) {
		return false;
	}
	else if(AccountBase::getInstance().getAccount(name).getCash() < table_desc.buy_in) {
		return false;
	}
	players.insert(std::make_pair(name,Player(prv_newSeat(),name)));
	AccountBase::getInstance().getAccount(name).setTable(table_desc.table_id);
	prv_playerBuyIn(name);
	//prv_sendToAll(REFRESH_TABLE,"",0,0,0,0); - nie robimy bo przy buyinie juz jest refresh
	return true;
}
Table::GameData Table::getGameData()
{
	boost::shared_lock<boost::shared_mutex> lock(data_mutex);
	return game_data;
}
Table::TableDesc Table::getTableDesc()
{
	boost::shared_lock<boost::shared_mutex> lock(data_mutex);
	return table_desc;
}
bool Table::prv_leaveTable(std::string name)
{
	if(!prv_isAtTable(name)) {
		return false;
	}
	AccountBase::getInstance().getAccount(name).addCash(players[name].cash_available);
	AccountBase::getInstance().getAccount(name).setTable(0);
	players.find(name)->second.cash_available = 0;
	if(game_data.in_game) {
		players.find(name)->second.left = true;
		if(players.find(name)->second.is_active) {
			prv_playerFold(name);
		}
		else {
			players.find(name)->second.is_playing = false;
		}
	}
	else {
		players.erase(players.find(name));
	}
	prv_sendToAll(REFRESH_TABLE,"",0,0,0,0);
	return true;
}

std::map<std::string,Player>::iterator Table::prv_seatToIterator(unsigned seat)
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.seat == seat) {
			return it;
		}
	}
	return players.end();
}

std::map<std::string,Player>::iterator Table::prv_getNextPlayer(unsigned seat)
{
	for(unsigned i = (seat + 1) % MAX_SEATS;i != seat; i = (i + 1) % MAX_SEATS) {
		std::map<std::string,Player>::iterator it = prv_seatToIterator(i);
		if(it != players.end() && it->second.is_playing && !it->second.all_in) {
			return it;
		}
	}
	return prv_seatToIterator(seat);
}

void Table::prv_activateNext()
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.is_active) {
			prv_activate(prv_getNextPlayer(it->second.seat)->second.seat);
			return;
		}
	}

}
void Table::prv_activate(unsigned seat)
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.is_active = false;
	}
	std::map<std::string,Player>::iterator act = prv_seatToIterator(seat);
	act->second.is_active = true;
	prv_sendToAll(TABLE_SET_ACTIVE,act->first,0,0,0,0);
}
unsigned Table::prv_pickCard()
{
	unsigned card = Rand() % cards_left;
	unsigned ret = deck[card];
	deck[card] = deck[--cards_left];
	return ret;
}
void Table::prv_initDeck()
{
	cards_left = DECK_SIZE;
	deck.clear();
	for(unsigned i = 0; i < DECK_SIZE; ++i) {
		deck.push_back(i);
	}
}
void Table::prv_newGame()
{
	unsigned big,small;
	table_desc.game_nr++;
#if DEBUGLEV >= 1
	std::cout << (Sync() << "* New game #" << table_desc.game_nr << " on table " << table_desc.table_id << "\n");
#endif
	game_data.in_game = true;
	game_data.turn_min_bet = 0;
	game_data.last_raise = 0;

	prv_initDeck();

	game_data.cards.clear();
	for(unsigned i = 0; i < HAND_SIZE; ++i) {
		game_data.cards.push_back(prv_pickCard());
	}

	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.is_playing = !(it->second.pause);
		it->second.is_active = false;
		it->second.all_in = false;
		it->second.revealed = false;
		it->second.bet_this_turn = 0;
	}

	game_data.dealer = prv_getNextPlayer(game_data.dealer)->second.seat;
	small = prv_getNextPlayer(game_data.dealer)->second.seat;
	big = prv_getNextPlayer(small)->second.seat;

	prv_activate(small);

	std::map<std::string,Player>::iterator big_it = prv_seatToIterator(big), small_it = prv_seatToIterator(small);

	if(small_it->second.cash_available >= table_desc.small_blind) {
		prv_playerRaise(small_it->first, table_desc.small_blind);
	}
	else {
		prv_playerAllIn(small_it->first);
	}

	if(big_it->second.cash_available >= table_desc.small_blind * 2) {
		prv_playerRaise(big_it->first, table_desc.small_blind * 2);
	}
	else {
		prv_playerAllIn(big_it->first);
	}

	game_data.turn_min_bet = table_desc.small_blind * 2;
	game_data.state = 0;

	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.card.clear();
		if(it->second.is_playing) {
			it->second.card.push_back(prv_pickCard());
			it->second.card.push_back(prv_pickCard());
			prv_revealOwnCard(it->first, 0);
			prv_revealOwnCard(it->first, 1);
		}
	}

	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.is_playing) {
			it->second.hand.set(it->second.card,game_data.cards);
		}
	}
	prv_sendToAll(TABLE_NEW_GAME,"",0,0,0,0);
}

void Table::prv_createWinnerTable()
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.is_playing) {
			winner_table.push_back(it->first);
		}
	}
	std::sort(winner_table.begin(),winner_table.end(), boost::bind(&Table::prv_playerComparator, this, _1, _2));
}
bool Table::prv_playerComparator(std::string a, std::string b)
{
	return players[b].hand < players[a].hand;
}
std::vector<std::string> Table::prv_getWinners()
{
	std::vector<std::string> ret;
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.current_winner = false;
	}
	int ptrn = -1;
	for(unsigned i = 0; i < winner_table.size(); ++i) {
		if(players[winner_table[i]].rewarded) {
			continue;
		}
		if(ptrn == -1) {
			ptrn = i;
		}
		if(players[winner_table[i]].hand == players[winner_table[ptrn]].hand) {
			players[winner_table[i]].rewarded = true;
			players[winner_table[i]].current_winner = true;
			ret.push_back(winner_table[i]);
		}
	}
	// zwraca kazdego, kto ma aktualnie najwyzsza reke i nie ma atrubutu rewarded
	// ustawia tym osobom current_winner i rewarded
	return ret;
}
void Table::prv_endGame()
{
	std::vector<std::string> winners, leavers;
#if DEBUGLEV >= 1
	std::cout<< (Sync() << "* Ending game on table " << table_desc.table_id << ", winners:\n");
#endif
	unsigned loot, max_loot;
	prv_createWinnerTable();
	while(true) {
		winners = prv_getWinners();
		if(winners.size() == 0) {
			break;
		}
		max_loot = loot = 0;
		for(unsigned i = 0; i < winners.size(); ++i) {
#if DEBUGLEV >= 3
			std::cout<< (Sync() << "-> " << winners[i] << ": " << players[winners[i]].cash_in_game << " own cash\n");
#endif
			max_loot += players[winners[i]].cash_in_game;
		}
		for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
			if(!it->second.current_winner) {
				loot += std::min(it->second.cash_in_game,max_loot);
				it->second.cash_in_game -= std::min(it->second.cash_in_game,max_loot);
			}
		}
#if DEBUGLEV >= 3
		std::cout<< (Sync() << "\tplus " << loot << " to share\n");
#endif
		for(unsigned i = 0; i < winners.size(); ++i) {
			if(max_loot) {
				prv_revealPrivateCardToAll(winners[i], 0, TABLE_REVEAL_WINNER);
				prv_revealPrivateCardToAll(winners[i], 1, TABLE_REVEAL_WINNER);
				players[winners[i]].cash_available += loot * players[winners[i]].cash_in_game / max_loot; // kazdy zwyciezca swoja czesc
			}
			players[winners[i]].cash_available += players[winners[i]].cash_in_game; // no i swoje
			players[winners[i]].cash_in_game = 0;
		}
	}
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.rewarded = it->second.current_winner = false;
		if(it->second.cash_available == 0) {
			it->second.pause = true;
		}
		if(it->second.left == true) {
			leavers.push_back(it->first);
		}
	}
	winner_table.clear();
	for(unsigned i = 0; i < leavers.size(); ++i) {
		players.erase(players.find(leavers[i]));
	}
	game_data.in_game = false;
	prv_sendCheckForNewGame();
}
bool Table::prv_playerSetPause(std::string name,bool pause)
{
	if(!prv_isAtTable(name)) {
		return false;
	}
	if(!pause && players[name].cash_available == 0 && players[name].cash_in_game == 0) {
		return false;
	}
	players[name].pause = pause;
	if(!pause) {
		prv_sendCheckForNewGame();
	}
	return true;
}
void Table::prv_sendCheckForNewGame()
{
	boost::shared_ptr<TableMessage> msg(new TableMessage);
	msg->operation = TABLE_CHECK_NEW_GAME;
	msg->no_return = true;
	addTask(msg);
}
void Table::prv_checkForNewGame()
{
	if(game_data.in_game) return;
	unsigned actives = 0;
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(!it->second.pause) ++actives;
	}
	if(actives <= 1) return;
	prv_newGame();
}

bool Table::prv_playerRaise(std::string name,unsigned raise)
{
	if(raise > game_data.turn_min_bet && players[name].bet_this_turn + players[name].cash_available >= raise) {
		game_data.turn_min_bet = raise;
		players[name].bet(raise - players[name].bet_this_turn, players[name].bet_this_turn);
		game_data.last_raise = players[name].seat;
		prv_resetActions(players[name].seat);
	}
	else {
		return false;
	}
	prv_sendToAll(TABLE_RAISE, name, 0, raise, 0, 0);
	prv_activateNext();
	prv_checkNextTurn();
	return true;
}
bool Table::prv_playerCheck(std::string name)
{
	unsigned bet_now = 0;
	if(players[name].cash_available + players[name].bet_this_turn >= game_data.turn_min_bet) {
		bet_now = game_data.turn_min_bet - players[name].bet_this_turn;
		players[name].bet(bet_now, players[name].bet_this_turn);
		players[name].took_action = true;
	}
	else {
		return false;
	}
	prv_sendToAll(TABLE_CHECK, name, 0, bet_now, 0, 0);
	prv_activateNext();
	prv_checkNextTurn();
	return true;
}
bool Table::prv_playerFold(std::string name)
{
	prv_sendToAll(TABLE_FOLD, name, 0, 0, 0, 0);
	prv_activateNext();
	players[name].is_playing = false;
	if(players[name].seat == game_data.last_raise) {
		game_data.last_raise = prv_getNextPlayer(players[name].seat)->second.seat;
	}
	prv_checkNextTurn();
	return true;
}
bool Table::prv_playerAllIn(std::string name)
{
	players[name].all_in = true;
	if(players[name].cash_available + players[name].bet_this_turn > game_data.turn_min_bet) {
		game_data.last_raise = prv_getNextPlayer(players[name].seat)->second.seat;
		prv_resetActions(players[name].seat);
	}
	players[name].bet(players[name].cash_available, game_data.turn_min_bet);
	prv_sendToAll(TABLE_ALLIN, name, 0, 0, 0, 0);
	prv_activateNext();
	prv_checkNextTurn();
	return true;
}


void Table::prv_checkNextTurn()
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		if(it->second.is_playing && !it->second.all_in && !it->second.took_action) {
			return;
		}
	}
	game_data.turn_min_bet = 0;
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.bet_this_turn = 0;
		if(it->second.all_in && !it->second.revealed) {
			it->second.revealed = true;
			prv_revealPrivateCardToAll(it->first, 0, TABLE_REVEAL_ALLIN);
			prv_revealPrivateCardToAll(it->first, 1, TABLE_REVEAL_ALLIN);
		}
	}
	prv_activate(game_data.last_raise);
	prv_resetActions(game_data.last_raise);
	game_data.state++;
	switch(game_data.state) {
		case 1:
			prv_revealTableCard(0);
			prv_revealTableCard(1);
			prv_revealTableCard(2);
			break;
		case 2:
			prv_revealTableCard(3);
			break;
		case 3:
			prv_revealTableCard(4);
			break;
		case 4:
			prv_endGame();
			return;
	}
	unsigned players_betting = 0;
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		players_betting += (it->second.is_playing && !it->second.all_in);
	}
	while(players_betting <= 1) {
		game_data.state++;
		switch(game_data.state) {
		case 1:
			prv_revealTableCard(0);
			prv_revealTableCard(1);
			prv_revealTableCard(2);
			break;
		case 2:
			prv_revealTableCard(3);
			break;
		case 3:
			prv_revealTableCard(4);
			break;
		case 4:
			prv_endGame();
			return;
		}
	}
}

void Table::prv_resetActions(unsigned new_act) {
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		it->second.took_action = (new_act == it->second.seat);
	}
}

void Table::prv_revealTableCard(unsigned cardnr)
{
	std::list<unsigned>::iterator card = game_data.cards.begin();
	for(unsigned i = 0; i < cardnr; ++i, ++card);
	prv_sendToAll(TABLE_REVEAL_PUBLIC_CARD, "", 0, *card, 0, 0);
}

void Table::prv_revealOwnCard(std::string whose, unsigned cardnr)
{
	std::list<unsigned>::iterator card = players[whose].card.begin();
	for(unsigned i = 0; i < cardnr; ++i, ++card);
	prv_sendToOne(whose,TABLE_REVEAL_PRIVATE_CARD, whose, 0, *card, 0, 0);
}

void Table::prv_revealPrivateCardToAll(std::string whose, unsigned cardnr, unsigned type_of_reveal)
{
	std::list<unsigned>::iterator card = players[whose].card.begin();
	for(unsigned i = 0; i < cardnr; ++i, ++card);
	prv_sendToAll(type_of_reveal, whose, 0, *card, 0, 0);
}

void Table::prv_sendToAll(unsigned op, std::string str1,unsigned msg_id, unsigned uint1, unsigned uint2, unsigned uint3)
{
	for(std::map<std::string,Player>::iterator it = players.begin(); it != players.end(); ++it) {
		boost::shared_ptr<Message> msg(new Message);
		msg->operation = op;
		msg->uint1 = uint1;
		msg->uint2 = uint2;
		msg->uint3 = uint3;
		msg->str1 = str1;
		msg->id = msg_id;
		msg->conn = AccountBase::getInstance().getAccount(it->first).getConnection();
		MessageQueueOut::getInstance().addMessage(msg);
	}
}

void Table::prv_sendToOne(std::string name, unsigned op, std::string str1,unsigned msg_id, unsigned uint1, unsigned uint2, unsigned uint3)
{
	boost::shared_ptr<Message> msg(new Message);
	msg->operation = op;
	msg->uint1 = uint1;
	msg->uint2 = uint2;
	msg->uint3 = uint3;
	msg->str1 = str1;
	msg->id = msg_id;
	msg->conn = AccountBase::getInstance().getAccount(name).getConnection();
	MessageQueueOut::getInstance().addMessage(msg);
}
