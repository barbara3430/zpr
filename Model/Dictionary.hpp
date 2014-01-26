#ifndef Dictionary_hpp
#define Dictionary_hpp

const unsigned DECK_SIZE = 52;
const unsigned COLOR_LEN = 13;

const unsigned TREFL = 0;
const unsigned KARO = 1;
const unsigned KIER = 2;
const unsigned PIK = 3;

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

const unsigned WYSOKA_KARTA = 0;
const unsigned PARA = 1;
const unsigned DWIE_PARY = 2;
const unsigned TROJKA = 3;
const unsigned STRIT = 4;
const unsigned KOLOR = 5;
const unsigned FULL = 6;
const unsigned KARETA = 7;
const unsigned POKER = 8;
const unsigned ROYAL_POKER = 9;

const unsigned HAND_SIZE = 5;

const unsigned MAX_SEATS = 2;
const unsigned ANTE = 20;
const unsigned START_BUDGET = 1000;

const unsigned PLAYER_RED = 0;
const unsigned PLAYER_BLUE = 1;

//stany gry
const unsigned START = 1;
const unsigned I_BIDDING = 2;
const unsigned CHANGE = 3;
const unsigned II_BIDDING = 4;
const unsigned END = 5;
#endif