#include "Hand.hpp"

#include <algorithm>

unsigned Card::c() const
{
	return card / 13; // 0 trefl 1 karo 2 kier 3 pik
}
unsigned Card::v() const
{
	return card % 13; // 0 2, 1 3, ..., 8 10, 9 J, 10 Q, 11 K, 12 A
}
std::string Card::print() const
{
	std::string ret;
	return ret + "234567890JQKA"[v()] + ' ' + "CDHS"[c()];
}
Card::Card(unsigned c)
{
	card = c;
}
bool Card::operator < (const Card& crd) const
{
	if(v() != crd.v()) {
		return v() > crd.v();
	}
	return c() > crd.c();
}
Hand::Hand()
{
	type = 0;
	for(unsigned i = 0; i < CRITERIA; ++i) {
		cr[i] = 0;
	}
}
void Hand::set(const std::list<unsigned>& priv,const std::list<unsigned>& pub)
{
	card.clear();
	for(std::list<unsigned>::const_iterator it = priv.begin(); it != priv.end(); ++it) {
		card.push_back(*it);
	}
	for(std::list<unsigned>::const_iterator it = pub.begin(); it != pub.end(); ++it) {
		card.push_back(*it);
	}
	calculateHand();
}
bool Hand::operator < (const Hand& h) const
{
	if(type != h.type) return type < h.type;
	for(unsigned i = 0; i < CRITERIA; ++i) {
		if(cr[i] != h.cr[i]) return cr[i] < h.cr[i];
	}
	return false;
}
bool Hand::operator == (const Hand& h) const
{
	if(type != h.type) return false;
	for(unsigned i = 0; i < CRITERIA; ++i) {
		if(cr[i] != h.cr[i]) return false;
	}
	return true;
}
void Hand::calculateHand()
{
	std::vector<Card> h;
	sort(card.begin(),card.end());
	for(unsigned i = 0; i < FULL_HAND_SIZE - 1; ++i) {
		for(unsigned j = 1; j < FULL_HAND_SIZE; ++j) {
			if(i == j) continue;
			h.clear();
			for(unsigned k = 0; h.size() < 5; ++k) {
				if(k != i && k != j) {
					h.push_back(card[k]);
				}
			}

			// tu zaczyna sie faktyczne liczenie pieciokartowej reki
			checkHand(h,HAND_ROYAL,0,checkRoyalFlush);
			checkHand(h,HAND_STRFLUSH,1,checkStraightFlush);
			checkHand(h,HAND_FOUR,2,checkFour);
			checkHand(h,HAND_FULL,2,checkFull);
			checkHand(h,HAND_FLUSH,1,checkFlush);
			checkHand(h,HAND_STRAIGHT,1,checkStraight);
			checkHand(h,HAND_THREE,3,checkThree);
			checkHand(h,HAND_TWOPAIR,3,checkTwoPair);
			checkHand(h,HAND_PAIR,4,checkPair);
			checkHand(h,HAND_HIGH,5,checkHighCard);

		}
	}
}

void Hand::checkHand(const std::vector<Card>& h, unsigned hand_type, unsigned criteria, unsigned critFcn(const std::vector<Card>&,unsigned))
{
	bool upd;
	unsigned tmp1;
	if(type <= hand_type) {
		upd = false;
		if(critFcn(h,0)) {
			if(type != hand_type){
				upd = true;
				for(unsigned k = 0; k < CRITERIA; ++k) {
					cr[k] = 0;
				}
			}
			type = hand_type;
			for(unsigned k = 0; k < criteria; ++k) {
				if(cr[k] > (tmp1 = critFcn(h,k+1)) && !upd) {
					break;
				}
				if(cr[k] < tmp1 || upd) {
					upd = true;
					cr[k] = tmp1;
				}
			}
		}
	}
}

unsigned Hand::checkRoyalFlush(const std::vector<Card>& c,unsigned cr)
{
	if(cr == 0) {
		c[0].c() == c[1].c() &&
		c[1].c() == c[2].c() &&
		c[2].c() == c[3].c() &&
		c[3].c() == c[4].c() &&
		c[0].v() == CARD_A &&
		c[1].v() == CARD_K &&
		c[2].v() == CARD_Q &&
		c[3].v() == CARD_J &&
		c[4].v() == CARD_10;
	}
	return 0;
}
unsigned Hand::checkStraightFlush(const std::vector<Card>& c,unsigned cr)
{
	if(cr == 0) {
		if(checkRoyalFlush(c,0)) return 0;
		return
		c[0].c() == c[1].c() &&
		c[1].c() == c[2].c() &&
		c[2].c() == c[3].c() &&
		c[3].c() == c[4].c() &&
		((c[1].v() - 1 == c[2].v() && c[2].v() - 1 == c[3].v() && c[3].v() - 1 == c[4].v())
		&&
		(c[0].v() - 1 == c[1].v() || (c[0].v() == CARD_A && c[4].v() == CARD_2)));
	}
	else if(cr == 1) {
		return checkStraight(c,1);
	}
	return 0;
}
unsigned Hand::checkFour(const std::vector<Card>& c,unsigned cr)
{
	if(cr == 0) {
		return
		c[1].v() == c[2].v() &&
		c[2].v() == c[3].v() &&
		(c[0].v() == c[1].v() || c[3].v() == c[4].v());
	}
	else if(cr == 1) {
		return (c[0].v() == c[1].v()) ? c[0].v() : c[4].v();
	}
	else if(cr == 2) {
		return (c[0].v() == c[1].v()) ? c[4].v() : c[0].v();
	}
	return 0;
}
unsigned Hand::checkFull(const std::vector<Card>& c,unsigned cr)
{
	if(cr == 0) {
		return
		c[0].v() == c[1].v() &&
		c[3].v() == c[4].v() &&
		(c[1].v() == c[2].v() || c[2].v() == c[3].v());
	}
	else if(cr == 1) {
		return (c[1].v() == c[2].v()) ? c[0].v() : c[4].v();
	}
	else if(cr == 2) {
		return (c[1].v() == c[2].v()) ? c[4].v() : c[0].v();
	}
	return 0;
}
unsigned Hand::checkFlush(const std::vector<Card>& c,unsigned cr)
{
	if(cr == 0) {
		if(checkStraightFlush(c,0)) return 0;
		if(checkRoyalFlush(c,0)) return 0;
		return
		c[0].c() == c[1].c() &&
		c[1].c() == c[2].c() &&
		c[2].c() == c[3].c() &&
		c[3].c() == c[4].c();
	}
	else if(cr == 1) {
		return c[0].v();
	}
	return 0;
}
unsigned Hand::checkStraight(const std::vector<Card>& c,unsigned cr)
{
	if(cr == 0) {
		if(checkStraightFlush(c,0)) return 0;
		if(checkRoyalFlush(c,0)) return 0;
		return
		(c[1].v() - 1 == c[2].v() &&
		c[2].v() - 1 == c[3].v() &&
		c[3].v() - 1 == c[4].v())
		&&
		(c[0].v() - 1 == c[1].v() || (c[0].v() == CARD_A && c[4].v() == CARD_2));
	}
	if(cr == 1) {
		return (c[4].v() == CARD_2) ? c[1].v() : c[0].v();
	}
	return 0;
}
unsigned Hand::checkThree(const std::vector<Card>& c,unsigned cr)
{
	std::vector<unsigned> val(CARDS_IN_COLOR,0);
	for(unsigned i = 0; i < c.size(); ++i) {
		++val[c[i].v()];
	}
	if(cr == 0) {
		if(checkFull(c,0)) return 0;
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 3) {
				return true;
			}
		}
		return 0;
	}
	else if(cr == 1) {
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 3) {
				return i;
			}
		}
	}
	else if(cr == 2) {
		for(unsigned i = CARD_A; i >= CARD_2; --i) {
			if(val[i] == 1) {
				return i;
			}
		}
	}
	else if(cr == 3) {
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 1) {
				return i;
			}
		}
	}
	return 0;
}
unsigned Hand::checkTwoPair(const std::vector<Card>& c,unsigned cr)
{
	std::vector<unsigned> val(CARDS_IN_COLOR,0);
	unsigned pair_ct;
	for(unsigned i = 0; i < c.size(); ++i) {
		++val[c[i].v()];
	}
	if(cr == 0) {
		pair_ct = 0;
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 2) {
				++pair_ct;
			}
		}
		return pair_ct == 2;
	}
	else if(cr == 1) {
		for(unsigned i = CARD_A; i >= CARD_2; --i) {
			if(val[i] == 2) {
				return i;
			}
		}
	}
	else if(cr == 2) {
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 2) {
				return i;
			}
		}
	}
	else if(cr == 3) {
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 1) {
				return i;
			}
		}
	}
	return 0;
}
unsigned Hand::checkPair(const std::vector<Card>& c,unsigned cr)
{
	std::vector<unsigned> val(CARDS_IN_COLOR,0);
	unsigned ct = 0;
	for(unsigned i = 0; i < c.size(); ++i) {
		++val[c[i].v()];
	}
	if(cr == 0) {
		if(checkTwoPair(c,0)) return 0;
		if(checkFull(c,0)) return 0;
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 2) {
				return 1;
			}
		}
		return 0;
	}
	else if(cr == 1) {
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] == 2) {
				return i;
			}
		}
	}
	else if(cr >= 2 && cr <= 4) {
		for(unsigned i = CARD_A; i >= CARD_2; --i) {
			if(val[i] == 1) {
				if((++ct) == cr - 1) return i;
			}
		}
	}
	return 0;
}
unsigned Hand::checkHighCard(const std::vector<Card>& c,unsigned cr)
{
	std::vector<unsigned> val(CARDS_IN_COLOR,0);
	unsigned ct = 0;
	for(unsigned i = 0; i < c.size(); ++i) {
		++val[c[i].v()];
	}
	if(cr == 0) {
		if(checkStraight(c,0)) return 0;
		if(checkFlush(c,0)) return 0;
		if(checkStraightFlush(c,0)) return 0;
		if(checkRoyalFlush(c,0)) return 0;
		for(unsigned i = 0; i < CARDS_IN_COLOR; ++i) {
			if(val[i] > 1) return 0;
		}
		return 1;
	}
	else if(cr >= 1 && cr <= 5) {
		for(unsigned i = CARD_A; i >= CARD_2; --i) {
			if(val[i] == 1) {
				if((++ct) == cr) return i;
			}
		}
	}
	return 0;
}

unsigned Hand::getType()
{
	return type;
}
unsigned Hand::getCriteria(unsigned x)
{
	if(x > CRITERIA) {
		return 0;
	}
	return cr[x];
}
