#include "tableinfo.h"

Tableinfo::Tableinfo()
{
}

Tableinfo::Tableinfo(int id, int buyin, int smallblind)
	: table_id(id), buy_in(buyin), small_blind(smallblind)
{
}

void Tableinfo::setLastAction(int action)
{
	last_action = action;
}
