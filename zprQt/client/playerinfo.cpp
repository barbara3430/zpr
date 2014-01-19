#include "playerinfo.h"
#include <QString>

Playerinfo::Playerinfo(QString name, unsigned allCash, unsigned tableCash, unsigned turn)
    : name(name), cash_available(allCash), cash_in_game(tableCash), last_bet(turn)
{
}

Playerinfo::Playerinfo()
{
}

