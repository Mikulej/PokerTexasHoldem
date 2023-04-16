#include "BotAI.h"
std::vector<Karta> Point::hand_cards;
#define SIM_NUM 5000
Point::Point(const std::vector<Karta>& _hand_cards, bool _isDealer, int _bot_credits,int _bot_gave, int _enemy_credits, int _enemy_gave, bool _enemy_checks,  int _pool,int _checked_cards) {//head constructor
	hand_cards = _hand_cards;
	sd.isDealer = _isDealer;
	sd.bot_credits = _bot_credits;
	sd.bot_gave = _bot_gave;
	sd.enemy_credits = _enemy_credits;
	sd.enemy_gave = _enemy_gave;
	sd.pool = _pool;
	sd.checked_cards = _checked_cards;
	sd.enemy_checks = _enemy_checks;
	prev = nullptr;

	Point* node_current = this;//zacznij od punktu startowego(glowy)
	while (simulations < SIM_NUM) {
		//std::cout << simulations << std::endl;
		node_current->expansion();//dodaj 3 rozgalezienia
		bool won_sim; //wykonaj ich symulacje
		won_sim = node_current->node_raise->simulation();
		node_current->node_raise->update(won_sim);
		won_sim = node_current->node_call->simulation();
		node_current->node_call->update(won_sim);
		won_sim = node_current->node_fold->simulation();
		node_current->node_fold->update(won_sim);
		node_current = selection();//wybierz nowe rozgalezienie do obslugi
		
	}
	//wybierz najlepszy ruch (ten z najwieksza iloscia symulacji)
	//zaktualizuj SimData dla korzenia bedzie to ruch ktory jako nastepny wykona bot
	int max_sim = node_raise->simulations; sd = node_raise->sd;
	if (node_call->simulations > max_sim) { max_sim = node_call->simulations; sd = node_call->sd;}
	if (node_fold->simulations > max_sim) { max_sim = node_fold->simulations; sd = node_fold->sd; }
}
Point::Point(Point* _prev,SimData _sd):prev(_prev),sd(_sd) {//node constructor

}
Point::~Point() {
	delete(node_raise);
	delete(node_call);
	delete(node_fold);
}
#define PARAM_C sqrt(2)
inline double Point::calc_uct(Point* root) {
	 return (double)((double)wins / (double)simulations) + PARAM_C * sqrt(log(prev->simulations) / (double)simulations);
}
Point* Point::selection(void) {	
	//sprawdz najlepsze z 3 dostepnych i wejdz glebiej
	Point* node_parent = this, *node_explore = nullptr;
	double max, temp;
	while (node_parent->node_raise != nullptr) {
		max = node_parent->node_raise->calc_uct(this); node_explore = node_parent->node_raise;
		temp = node_parent->node_call->calc_uct(this); if (temp > max) { max = temp; node_explore = node_parent->node_call;}
		temp = node_parent->node_fold->calc_uct(this); if (temp > max) { max = temp; node_explore = node_parent->node_fold; }
		node_parent = node_explore;
	}
	return node_parent;
}
bool Point::simulation(void) {
	int first_move = sd.next_move;
	std::vector<Gracz> players;
	players.push_back(Gracz()); players[0].add_credits(sd.enemy_credits); players[0].gave_to_pool = sd.enemy_gave; players[0].checks = sd.enemy_checks;
	players.push_back(Gracz()); players[1].add_credits(sd.bot_credits); players[1].gave_to_pool = sd.bot_gave; 
	std::vector<Karta> all_cards; Karta::stworz_komplet(all_cards);
	//std::cout << "Przed: " << all_cards.size() << std::endl;
	//wyjmij karty z all_cards ktore ma bot
	for (auto start = all_cards.begin(), finish = all_cards.end(); start != finish; ) {
		if ((*start) == hand_cards[0] || (*start) == hand_cards[1]) {
			start = all_cards.erase(start); finish = all_cards.end();
		}
		else {	
			start++;
		}
	}
	//std::cout << "Po: " << all_cards.size() << std::endl;
	std::random_shuffle(all_cards.begin(), all_cards.end());
	std::vector<Karta> hole_cards;//karty na stole
	for (int i = 0; i < 5; i++) hole_cards.push_back(all_cards.back()); all_cards.pop_back();
	for (int i = 0; i < 2; i++)  players[0].reka.push_back(all_cards.back()); all_cards.pop_back(); //karty gracza
	players[1].reka = hand_cards;//karty bota
	
	bool updated_sim_data = false;//sluzy takze jako flaga do wykonywania pierwszego ruchu
	bool round_start = false;//symulacja zaczyna sie od trwajacej rundy
	bool bot_turn = true;
	int sim_pool = sd.pool;
	int checked_cards = sd.checked_cards;
	int missing_blind = false;
	bool isDealer = sd.isDealer;
	while (players[0].get_credits()!=0 || players[1].get_credits() !=0) {//bot wykonuje pierwszy ruch
		std::vector<Gracz>::iterator player = players.begin(); if (bot_turn) { player++; }
		std::vector<Gracz>::iterator enemy = players.begin(); if (!bot_turn) { enemy++; }
		//if (bot_turn) { std::cout << "Bot's turn" << std::endl; }
		//else{ std::cout << "Player's turn" << std::endl; }
		//std::cout << "Bot credits=" << players[1].get_credits() << " Player credits=" << players[0].get_credits() << " sim_pool="<<sim_pool<< " all=" << players[1].get_credits()+ players[0].get_credits()+sim_pool << std::endl;
		//blinds
		if (round_start) {		
			//std::cout << "New round " << std::endl;
			if (!isDealer) {//dealer dostaje big blind
				if (players[0].get_credits() >= 200) { players[0].sim_give_to_pool(200, sim_pool); }
				else { missing_blind = true; }
				if (players[1].get_credits() >= 100) { players[1].sim_give_to_pool(100, sim_pool); }
				else{ missing_blind = true; }
				//std::cout << "Player is dealer " << std::endl;
			}
			else {
				if (players[0].get_credits() >= 100) { players[0].sim_give_to_pool(100, sim_pool); }
				else{ missing_blind = true; }
				if(players[1].get_credits() >= 200){ players[1].sim_give_to_pool(200, sim_pool); }
				else{ missing_blind = true; }

				//std::cout << "Bot is dealer " << std::endl;
			}
			round_start = false;
		}
		if (missing_blind) { break; }
		//std::cout << "Bot credits=" << players[1].get_credits() << " Player credits=" << players[0].get_credits() << " sim_pool=" << sim_pool << " all=" << players[1].get_credits() + players[0].get_credits() + sim_pool << std::endl;
		if ((*player).get_credits() == 0 || (*enemy).get_credits() == 0) {//forced check
			checked_cards = 4;
		}
		else {
			//perform random action
			int option;
			if (!updated_sim_data) { option = first_move;  } //wykonaj pierwszy ruch zaplanowany w klasie Point
			else{ option = rand() % 3; }//moze ustawic jakies lepsze wagi? ze np fold ma 65% szans			
			int raise_amount; int h_max; int h_min; int align_amount; int fin_min_raise = 0; int h;
			switch (option) {
			case 0://raise - done
				if (!updated_sim_data) {
					raise_amount = sd.raise_by;
				}
				else{
					fin_min_raise = (*enemy).gave_to_pool - (*player).gave_to_pool;
					if (fin_min_raise < 0) { fin_min_raise = 0; }
					fin_min_raise += Game::minimal_raise;

					h_min = fin_min_raise;
					h_max = (*player).get_credits();
					h = h_max - h_min;
					if (h <= 0) { raise_amount = h_max; }
					else {
						raise_amount = (rand() % h / 100) * 100 + h_min;
					}
				}
				

				(*player).sim_give_to_pool(raise_amount, sim_pool);
				(*player).checks = false; (*enemy).checks = false;
				//std::cout << bot_turn << " raised by " << raise_amount << std::endl;
				break;
			case 1://call - done		
				//jesli caller dal mniej niz drugi to musi doplacic
				if ((*player).gave_to_pool < (*enemy).gave_to_pool) {
					align_amount = (*enemy).gave_to_pool - (*player).gave_to_pool;
					if (align_amount < (*player).get_credits()) {//ma wystarczajaco kredytow by wyrownac stawke
						(*player).sim_give_to_pool((*enemy).gave_to_pool - (*player).gave_to_pool, sim_pool);
					}
					else {//nie ma
						(*player).sim_give_to_pool((*player).get_credits(), sim_pool);
					}
				}
				(*player).checks = true;
				//std::cout << bot_turn << " calls " << std::endl;
				break;
			case 2://fold
				(*enemy).add_credits(sim_pool);
				sim_pool = 0; (*player).gave_to_pool = 0; (*enemy).gave_to_pool = 0;
				(*player).checks = false; (*enemy).checks = false;
				//std::cout << bot_turn << " folds " << std::endl;
				bot_turn = !bot_turn;
				round_start = true;
				isDealer = !isDealer;
				if (!updated_sim_data) { //zwroc SimulationData po wykonaniu pierwszego ruchu do Point
					sd.enemy_credits = players[0].get_credits();
					sd.bot_credits = players[1].get_credits();
					sd.enemy_gave = players[0].gave_to_pool;
					sd.bot_gave = players[1].gave_to_pool;
					sd.pool = sim_pool;
					sd.checked_cards = checked_cards;
					sd.isDealer = isDealer;
					updated_sim_data = true;
				}
				continue;//dla optymalizacji ale jesli psuje switcha to mozna wywalic i te bot_turn = !bot_turn; tez
				break;
			}
			if ((*player).checks && (*enemy).checks) { checked_cards++; }
		}
		
		if (checked_cards == 4) {
			players[0].calculate_power(); players[1].calculate_power();
			//porownaj i wybierz zwyciesce
			int power0 = players[0].get_power();
			int power1 = players[1].get_power();
			int sub_power0 = players[0].get_sub_power();
			int sub_power1 = players[1].get_sub_power();
			if ((power0 > power1) || (power0 == power1 && sub_power0 > sub_power1)) {
				players[0].add_credits(sim_pool);
				sim_pool = 0;
				//std::cout <<"Player wins" << std::endl;
			}
			else if ((power1 > power0) || (power0 == power1 && sub_power1 > sub_power0)) {
				players[1].add_credits(sim_pool);
				sim_pool = 0;
				//std::cout << "Bot wins" << std::endl;
			}
			else {//remis
				players[0].add_credits(players[0].gave_to_pool);
				players[1].add_credits(players[1].gave_to_pool);
				sim_pool = 0;
				//std::cout << "Draw" << std::endl;
			}
			round_start = true;
			isDealer = !isDealer;
		}
		if(!updated_sim_data){ //zwroc SimulationData po wykonaniu pierwszego ruchu do Point
			sd.enemy_credits = players[0].get_credits();
			sd.bot_credits = players[1].get_credits();
			sd.enemy_gave = players[0].gave_to_pool;
			sd.bot_gave = players[1].gave_to_pool;
			sd.pool = sim_pool;
			sd.checked_cards = checked_cards;
			sd.isDealer = isDealer;
			updated_sim_data = true;
		}
		bot_turn = !bot_turn;		
	}
	//std::cout << "Game finished " << std::endl;
	if (players[1].get_credits() > players[0].get_credits()) {
		//std::cout << "BOT WON" << std::endl;
		return true; 
	}
	else { 
		//std::cout << "BOT LOST" << std::endl; 
		return false;
	}

}
void Point::expansion(void) {
	SimData nsd = sd;
	//ustal o ile bedziemy podbijac
	int fin_min_raise,h_min,h_max,h;
	fin_min_raise = sd.enemy_gave - sd.bot_gave;
	if (fin_min_raise < 0) { fin_min_raise = 0; }
	fin_min_raise += Game::minimal_raise;
	h_min = fin_min_raise;
	h_max = sd.bot_credits;
	h = h_max - h_min;
	if (h <= 0) { nsd.raise_by = h_max; }
	else {nsd.raise_by= (rand() % h / 100) * 100 + h_min;}
	//ta czesc mozna zoptymalizowac bardziej ^
	nsd.next_move = 0; node_raise = new Point(this, nsd); 
	nsd.raise_by = 0;
	nsd.next_move = 1; node_call = new Point(this,nsd);
	nsd.next_move = 2; node_fold = new Point(this,nsd);
}
void Point::update(bool _won) {//zrob update do punktu startowego(glowy)
	Point* temp = this;
	if (_won) { 		
		while (temp != nullptr) {
			temp->simulations++; temp->wins++;
			temp = temp->prev;
		}
	}
	else {
		while (temp != nullptr) {
			temp->simulations++;
			temp = temp->prev;
		}
	}	
}
int Point::get_next_move(void) {
	return sd.next_move;
}
int Point::get_raise_by(void) {
	return sd.raise_by;
}