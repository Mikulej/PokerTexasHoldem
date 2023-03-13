#include "PokerEngine.h"
std::string enemy_desc;
//KARTA
std::vector<Karta> Karta::komplet;
int Karta::rewers_id = 1;
int Karta::rewers = 0;
void Karta::next_rewers() {
	if (rewers_id < 5) { rewers_id++; }
	else { rewers_id = 1; }
	set_rewers(rewers_id);
}
void Karta::prev_rewers() {
	if (rewers_id > 1) { rewers_id--; }
	else { rewers_id = 5; }
	set_rewers(rewers_id);
}
void Karta::set_rewers(int _r) {
	switch (_r) {
	case 1:
		rewers = 0;
		break;
	case 2:
		rewers = 1;
		break;
	case 3:
		rewers = 3;
		break;
	case 4:
		rewers = 4;
		break;
	case 5:
		rewers = 9;
		break;
	}

}
int Karta::get_rewers() {
	return rewers;
}
void Karta::stworz_komplet(std::vector<Karta>& komplet) {
	int k = 0, n = 1;
	while (komplet.size() < 52) {
		komplet.push_back(Karta((kolor)k, (numer)n));
		n++;
		if (n == 14) { k++; n = 1; }
	}
}
Karta::Karta(kolor _k, numer _n) : k(_k), n(_n) {

}
void Karta::KartaInit() {
	stworz_komplet(komplet);
}
int Karta::get_kolor() {
	return (int)k;
}
int Karta::get_numer() {
	return (int)n;
}
const std::vector<Karta>& Karta::get_komplet() {
	return komplet;
}

//GRACZ
std::vector<Gracz> Gracz::graczList;
int starting_credits = 1000;
void Gracz::add_credits(int _credits) {
	credits += _credits;
}
int Gracz::get_credits(void) {
	return credits;
}
void Gracz::wez_karty_z_talii() {
	reka.push_back(Game::talia.back()); Game::talia.pop_back();
	reka.push_back(Game::talia.back()); Game::talia.pop_back();
}
void Gracz::raise(int _v) {
	give_to_pool(_v);
	for (auto g : graczList) g.checks = false;	
}
void Gracz::call(int _caller) {
	int enemy = 1 - _caller;
	//jesli caller dal mniej niz drugi to musi doplacic
	if (Gracz::graczList[_caller].gave_to_pool < Gracz::graczList[enemy].gave_to_pool) {
		give_to_pool(Gracz::graczList[enemy].gave_to_pool - Gracz::graczList[_caller].gave_to_pool);
		if(_caller){ Game::enemy_desc = "Enemy calls."; }
	}
	else if (_caller) { Game::enemy_desc = "Enemy checks."; }
	checks = true;

}
void Gracz::fold(int _folder) {
	int enemy = 1 - _folder;
	Gracz::graczList[enemy].add_credits(Game::get_pool());
	Game::clear_pool();
	if (_folder) { Game::enemy_desc = "Enemy folds."; }
	Game::game_started = false;
	Game::whowins = enemy;

}
void Gracz::random_action() {
	//if (Gracz::graczList[0].allin) {//poszedl all-in nie podbijaj stawki
	//	
	//	if (gave_to_pool >= Gracz::graczList[0].gave_to_pool) {
	//		//check
	//	}
	//	else {
	//		//call-wyrownaj stawke
	//	}
	//}
	//else {
	//	int r = (rand() % 9) + 1;
	//	raise((r / 2) * 100);
	//	Game::enemy_desc = "Enemy raised by " + std::to_string((r / 2) * 100) + ".";
	//}
	call(1);
	//fold(1);

}

void Gracz::give_to_pool(int _v) {
	Game::pool += _v;
	gave_to_pool += _v;
	add_credits(-_v);
}
//BOT

//GAME
int Game::pool = 0;
int Game::dealer_option = 1;
int Game::whos_dealer = 0;
int Game::starting_credits = 5000;
int Game::minimal_raise = 100;
int Game::raise = 100;
bool Game::game_started = false;
bool Game::enemy_turn = false;

int Game::checked_cards = 0;
int Game::whowins = 2; //0=player 1=enemy 2=none
std::string Game::enemy_desc = "New cards dealt.";

//std::string Game::enemy_desc = "Cards dealt.";
//std::string Game::button1_desc = "Raise";
//std::string Game::button2_desc = "Call";
std::vector<Karta> Game::talia;
std::vector<Karta> Game::stol;
int rng(int i) { return std::rand() % i; }
void Game::stworz_talie(std::vector<Karta>& _talia) {
	_talia = Karta::get_komplet();
	std::random_shuffle(_talia.begin(), _talia.end(), rng);
}
void Game::stworz_stol() {
	for (int i = 0; i < 5; i++) {
		stol.push_back(talia.back());
		talia.pop_back();
	}
}
void Game::clear_pool() {
	pool = 0;
	Gracz::graczList[0].gave_to_pool = 0;
	Gracz::graczList[1].gave_to_pool = 0;
}


int Game::get_pool() {
	return pool;
}
void Game::blinds(int _d) {//przydziel stawki startowe
	if (_d == 0) {//dealer to gracz otrzymuje small blind
		Gracz::graczList[0].give_to_pool(200);
		Gracz::graczList[1].give_to_pool(100);
	}
	else {
		Gracz::graczList[0].give_to_pool(100);
		Gracz::graczList[1].give_to_pool(200);
	}
}

//void Game::add_pool_from(int _v,Gracz& _g) {
//	pool += _v;
//	_g.gave_to_pool += _v;
//}

