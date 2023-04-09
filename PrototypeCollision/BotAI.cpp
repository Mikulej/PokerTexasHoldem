#include "BotAI.h"
std::vector<Karta> Point::hand_cards;
bool Point::isDealer;
Point::Point() {
	int simulations = 0;
	prev = nullptr;
}
void Point::init_point(const std::vector<Karta>& _hand_cards,bool _isDealer) {
	hand_cards = _hand_cards;
	isDealer = _isDealer;
}
void Point::expansion(void) {
	//nodes.push_back();
}

void Point::simulation(int bot_credits, int enemy_credits, int _pool) {
	simulations++;
	std::vector<Gracz> players;
	players.push_back(Gracz()); players[0].add_credits(enemy_credits);
	players.push_back(Gracz()); players[1].add_credits(bot_credits);
	std::vector<Karta> all_cards; Karta::stworz_komplet(all_cards);
	std::random_shuffle(all_cards.begin(), all_cards.end());
	std::vector<Karta> hole_cards;//karty na stole
	for (int i = 0; i < 5; i++) hole_cards.push_back(all_cards.back()); all_cards.pop_back();
	for (int i = 0; i < 2; i++)  players[0].reka.push_back(all_cards.back()); all_cards.pop_back(); //karty gracza
	players[1].reka = hand_cards;//karty bota
	
	bool round_start = true;
	bool bot_turn = true;
	int pool = _pool;
	int checked_cards = 0;
	while (players[0].get_credits()!=0 || players[1].get_credits() !=0) {//bot wykonuje pierwszy ruch
		std::vector<Gracz>::iterator player = players.begin(); if (bot_turn) { player++; }
		std::vector<Gracz>::iterator enemy = players.begin(); if (!bot_turn) { enemy++; }
		if (bot_turn) { std::cout << "Bot's turn" << std::endl; }
		else{ std::cout << "Player's turn" << std::endl; }
		std::cout << "Bot credits=" << players[1].get_credits() << " Player credits=" << players[0].get_credits() << " pool="<<pool<< std::endl;
		//start bids
		if (round_start) {		
			std::cout << "New round " << std::endl;
			if (!isDealer) {//dealer dostaje big blind
				players[0].give_to_pool(200);
				players[1].give_to_pool(100);
				std::cout << "Player is dealer " << std::endl;
			}
			else {
				players[0].give_to_pool(100);
				players[1].give_to_pool(200);
				std::cout << "Bot is dealer " << std::endl;
			}
			round_start = false;
		}
		std::cout << "Bot credits=" << players[1].get_credits() << " Player credits=" << players[0].get_credits() << " pool=" << pool << std::endl;
		//if (players[0].get_credits() != 0 || players[1].get_credits() != 0) { checked_cards=4; break; }//sprawdz czy ktos zbankrutowal przez danie blinda - napewno?
		//perform random action
		int option = rand() % 3;//moze ustawic jakies lepsze wagi? ze np fold ma 65% szans
		int raise_amount; int h; int align_amount;
		switch (option) {
		case 0://raise
			//if (raise_amount < Game::minimal_raise) {} !!! UWZGLEDNIJ GAME::MINIMAL_RAISE
			h = (*player).get_credits() / 100;
			if (h == 0) {
				(*player).allin = true;
				raise_amount = (*player).get_credits();
			}
			else {
				raise_amount = ((rand() % h)+1) * 100;
			}
			
			
			(*player).sim_give_to_pool(raise_amount,pool);
			(*player).checks = false; (*enemy).checks = false;
			std::cout << bot_turn << " raised by " << raise_amount << std::endl;
			break;
		case 1://call - done		
			//jesli caller dal mniej niz drugi to musi doplacic
			if ((*player).gave_to_pool < (*enemy).gave_to_pool) {
				align_amount = (*enemy).gave_to_pool - (*player).gave_to_pool;
				if (align_amount < (*player).get_credits()) {//ma wystarczajaco kredytow by wyrownac stawke
					(*player).sim_give_to_pool((*enemy).gave_to_pool - (*player).gave_to_pool,pool);
				}
				else {//nie ma
					(*player).sim_give_to_pool((*player).get_credits(),pool);
				}
			}
			(*player).checks = true;
			std::cout << bot_turn << " calls " << std::endl;
			break;
		case 2://fold
			(*enemy).add_credits(pool);
			pool = 0; (*player).gave_to_pool = 0; (*enemy).gave_to_pool = 0;
			(*player).checks = false; (*enemy).checks = false;
			std::cout << bot_turn << " folds " << std::endl;
			bot_turn = !bot_turn;
			round_start = true;
			isDealer = !isDealer;
			
			continue;//dla optymalizacji ale jesli psuje switcha to mozna wywalic i te bot_turn = !bot_turn; tez
			break;
		}
		if ((*player).checks && (*enemy).checks) { checked_cards++; }
		if (checked_cards == 4) {
			players[0].calculate_power(); players[1].calculate_power();
			//porownaj i wybierz zwyciesce
			int power0 = players[0].get_power();
			int power1 = players[1].get_power();
			int sub_power0 = players[0].get_sub_power();
			int sub_power1 = players[1].get_sub_power();
			if ((power0 > power1) || (power0 == power1 && sub_power0 > sub_power1)) {
				players[0].add_credits(pool);
				pool = 0;
				std::cout <<"Player wins" << std::endl;
			}
			else if ((power1 > power0) || (power0 == power1 && sub_power1 > sub_power0)) {
				players[1].add_credits(pool);
				pool = 0;
				std::cout << "Bot wins" << std::endl;
			}
			else {//remis
				players[0].add_credits(players[0].gave_to_pool);
				players[1].add_credits(players[1].gave_to_pool);
				pool = 0;
				std::cout << "Draw" << std::endl;
			}
			round_start = true;
		}
		isDealer = !isDealer;
		bot_turn = !bot_turn;		
	}
	std::cout << "Game finished " << std::endl;
}
