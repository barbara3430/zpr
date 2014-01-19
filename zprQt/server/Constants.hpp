#pragma once

const unsigned POKER_PORT = 61298;

// KARTY

const unsigned DECK_SIZE = 52;
const unsigned CARDS_IN_COLOR = 13;

const unsigned COLOR_CLUBS = 0;
const unsigned COLOR_DIAMONDS = 1;
const unsigned COLOR_HEARTS = 2;
const unsigned COLOR_SPADES = 3;

const unsigned CARD_2 = 0;
const unsigned CARD_3 = 1;
const unsigned CARD_4 = 2;
const unsigned CARD_5 = 3;
const unsigned CARD_6 = 4;
const unsigned CARD_7 = 5;
const unsigned CARD_8 = 6;
const unsigned CARD_9 = 7;
const unsigned CARD_10 = 8;
const unsigned CARD_J = 9;
const unsigned CARD_Q = 10;
const unsigned CARD_K = 11;
const unsigned CARD_A = 12;

const unsigned HAND_HIGH = 0;
const unsigned HAND_PAIR = 1;
const unsigned HAND_TWOPAIR = 2;
const unsigned HAND_THREE = 3;
const unsigned HAND_STRAIGHT = 4;
const unsigned HAND_FLUSH = 5;
const unsigned HAND_FULL = 6;
const unsigned HAND_FOUR = 7;
const unsigned HAND_STRFLUSH = 8;
const unsigned HAND_ROYAL = 9;

// rzeczy dla stolu
const unsigned MAX_SEATS = 8;

// rzeczy dla reki

const unsigned PRIV_HAND_SIZE = 2;
const unsigned HAND_SIZE = 5;
const unsigned FULL_HAND_SIZE = 7;
const unsigned CRITERIA = 5;

// podstawowa odpowiedz
const unsigned RETURN_OK = 10;
const unsigned RETURN_FAIL = 11;

// ?
const unsigned ACCOUNT_LOGIN = 40;
const unsigned ACCOUNT_LOGOUT = 41;

// wiadomosc zwrotna na listing
const unsigned LIST_TABLES_RETURNMSG = 50; // OUT: uint1 tableid uint2 buyin uint3 smallblind
const unsigned LIST_PLAYERS_RETURNMSG = 51; // OUT: str1 nick, uint1 kasa do uzycia, uint2 kasa w grze, uint3 kasa postawiona w tej turze

// prosby gracza o listing
const unsigned LIST_TABLES = 52;
const unsigned LIST_PLAYERS = 53;

// prosby do gracza, zaby wyslal swoj request z listingiem
const unsigned REFRESH_LOBBY = 60;
const unsigned REFRESH_TABLE = 61;

// wbudowany typ wiadomosci, nie-opcode
const unsigned MSG_COMMON = 80;
const unsigned MSG_TABLE = 81;

// pojedyncze requesty do stolu
const unsigned TABLE_JOIN = 100;
const unsigned TABLE_LEAVE = 101;
const unsigned TABLE_BUYIN = 102;
const unsigned TABLE_PAUSE = 103;
const unsigned TABLE_UNPAUSE = 104;
const unsigned TABLE_CHECK_NEW_GAME = 105;
const unsigned TABLE_NEW_GAME = 106;

const unsigned TABLE_RAISE = 110; // IN: uint1 do ilu podbija OUT: uint1 do ilu podbija str1 kto podbija
const unsigned TABLE_CHECK = 111; // OUT: str1 kto checkuje
const unsigned TABLE_FOLD = 112; // OUT: str1 kto folduje
const unsigned TABLE_ALLIN = 113; // OUT: str1 kto allinuje
const unsigned TABLE_REVEAL_PUBLIC_CARD = 114; // OUT: uint1 nowa karta publiczna
const unsigned TABLE_REVEAL_PRIVATE_CARD = 115; // OUT: uint1 nowa karta prywatna str1 czyja
const unsigned TABLE_REVEAL_WINNER = 116; // OUT: uint1 nowa karta prywatna str1 czyja
const unsigned TABLE_REVEAL_ALLIN = 117; // OUT: uint1 nowa karta prywatna str1 czyja

const unsigned TABLE_SET_ACTIVE = 120; // OUT: str1 kto aktywny


/*

	poza tym wszystkie wiadomosci wejsciowe zawieraja account_name, account_password, id (id wiadomosci);
	wiadomosci wejsciowe dla stolu zawieraja takze dest_table - stol docelowy
	wiadomosci wyjsciowe zawieraja dodatkowo conn - informacje do kogo wyslac i jesli jest to odpowiedz na cos od klienta - id wiadomosci na ktora odpowiadaja

*/
