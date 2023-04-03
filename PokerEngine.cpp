#include "PokerEngine.h"
extern int ending_game;
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
Karta::Karta(int _k, int _n) :k((kolor)_k),n((numer)_n){

}
void Karta::KartaInit() {
	stworz_komplet(komplet);
}
int Karta::get_kolor() const{
	return (int)k;
}
int Karta::get_numer() const{
	return (int)n;
}
const std::vector<Karta>& Karta::get_komplet() {
	return komplet;
}

//GRACZ
std::vector<Gracz> Gracz::graczList;
int starting_credits = 1000;
int Gracz::next_move = 0;
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
		int align_amount = Gracz::graczList[enemy].gave_to_pool - Gracz::graczList[_caller].gave_to_pool;
		if (align_amount < Gracz::graczList[_caller].credits) {//ma wystarczajaco kredytow by wyrownac stawke
			give_to_pool(Gracz::graczList[enemy].gave_to_pool - Gracz::graczList[_caller].gave_to_pool);
		}
		else {//nie ma
			give_to_pool(Gracz::graczList[_caller].credits);
		}
		if(_caller){ Game::enemy_desc = "Enemy calls."; }
	}
	else if (_caller) { Game::enemy_desc = "Enemy checks."; }
	checks = true;

}
void Gracz::fold(int _folder) {
	int enemy = 1 - _folder;
	if (_folder) { Game::enemy_desc = "Enemy folds."; }
	Game::end_round(enemy);
}
void Gracz::bot_action() {
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
	if (next_move == 0) {
		next_move = (rand() % 3) + 1;
	}
	int pom = (credits / 100) / 2;
	if (pom == 0) { pom = 1; }
	int rv = rand() % pom;
	switch (next_move) {
	case 0:
		break;
	case 1:
		raise(rv * 100);
		Game::enemy_desc = "Enemy raised by " + std::to_string(rv*100) + ".";
		break;
	case 2:
		call(1);
		break;	
	case 3:
		fold(1);
		break;
	}



}

void Gracz::give_to_pool(int _v) {
	Game::pool += _v;
	gave_to_pool += _v;
	add_credits(-_v);
}
int Gracz::get_power() {//oblicz najlepsza kombinacje kart
	//high card = 2-14
	//one pair = 2 (ten sam numer)
	//two pairs = 3 (ten sam numer)
	//three of a kind = 4 (ten sam numer)
	//straight = 5 (5 kart ten sam kolor niepokolei)
	//flush = 6 (wszystkie w tym samym kolorze)
	//full house = 7 (3 kart z tym samym numerem A + 2 karty z tym samym numerem B)
	//straight flush = 8 (5 kart ten sam kolor pokolei)
	//royal flush = 9 (5 kart pokolei w tym samym kolorze najwyzsze karty)
	std::vector<Karta> karty,mozliwe,najlepsze;
	int highest_power = -1, highest_sub_power = -1;
	//for (int i = 0; i < 5; i++) {
	//	karty.push_back(Game::stol.at(i));
	//}
	karty.push_back(reka.at(0));
	karty.push_back(reka.at(1));
	//testowe karty
	karty.push_back(Karta(0,4));
	karty.push_back(Karta(3, 4));
	karty.push_back(Karta(2, 3));
	karty.push_back(Karta(2, 4));
	karty.push_back(Karta(1, 3));
	//karty.push_back();
	//5 kart z 7 mozemy wybrac na 21 sposobow
	//wybierz 5 z 7 kart
	for (int i = 0; i < 7;i++) {//wywal karte A o indeksie i
		for (int j = 0; j < 6; j++) {//wywal karte B o indeksie j
			if (j == i) { continue; }
			
			//stworz ulozenie
			for (int k = 0; k < 7; k++) {
				if(k != i && k != j){ mozliwe.push_back(karty.at(k)); }				
			}
			//KROK 1 czy sa pokolei?
			std::sort(mozliwe.begin(), mozliwe.end(), [](const Karta& lewa, const Karta& prawa) ->bool { 
				int num_left = lewa.get_numer();
				if (num_left == 1) { num_left = 14; }
				int num_right = prawa.get_numer();
				if (num_right == 1) { num_right = 14; }
				return num_left < num_right; });
			bool pokolei = true;	
			for (int i = 0; i < 4; i++) {
				int num_left = mozliwe.at(i + 1).get_numer();	//uwzglednij asa (ma moc 1)
				if (num_left == 1) { num_left = 14; }
				int num_right = mozliwe.at(i).get_numer();
				if (num_right == 1) { num_right = 14; }
				if (num_left - num_right != 1) pokolei = false;
			}
			//KROK 2 czy sa w tym samym kolorze?
			bool jedenkolor = true;
			int kolor = mozliwe.at(0).get_kolor();
			for (auto k : mozliwe) {
				if (k.get_kolor() != kolor) { jedenkolor = false; }
			}
			//KROK 3 policz powtorki kart // czy jest czworka?
			//int asy = 0, dwojki = 0, trojki = 0, czworki = 0, piatki = 0, szostki = 0, siodemki = 0, osemki = 0, dziewiatki = 0, dziesiatki = 0, walety = 0, damy = 0, krole = 0;
			int powtorzenia[13] = { 0 };
			for (int i = 0; i < 5; i++) {
				int numer = mozliwe.at(i).get_numer();
				if (numer == 1) { numer = 14; }
				powtorzenia[numer - 2]++;
		
			}
			//znajdz max
			int max1 = 0, max1_id = 0;//ilosc powtorek // id na te miejsce w powtorzenia[]
			for (int i = 0; i < 13; i++) {
				if (powtorzenia[i] > max1 || (powtorzenia[i] >= max1 && i > max1_id)) {//zastap jesli indeks jest wyzszy(para asow jest lepsza niz para dwojek)
					max1 = powtorzenia[i]; max1_id = i;
				}
			}
			//znajdz druga najwieksza wielokrotnosc
			int max2 = 0, max2_id = 0;
			for (int i = 0; i < 13; i++) {
				if (i == max1_id) { continue; }
				if (powtorzenia[i] > max2 || (powtorzenia[i] >= max2 && i > max2_id)) {//zastap jesli indeks jest wyzszy(para asow jest lepsza niz para dwojek)
					max2 = powtorzenia[i]; max2_id = i;
				}
			}
			//Przydziel moc
			int power, sub_power;
			if (pokolei && jedenkolor && mozliwe[4].get_numer() == 14) { power = 9; sub_power = 1; } //royal flush
			else if (pokolei && jedenkolor) { 
				power = 8; sub_power = mozliwe[4].get_numer(); //DODAJ TU WARUNEK Z ASEM
			} //straight flush
			else if (max1 == 4)	{//czworka
			power = 7; 
			int num_big = mozliwe[4].get_numer(); if (num_big == 1) { num_big = 14; }
			int num_small = mozliwe[0].get_numer(); if (num_small == 1) { num_small = 14; }
			sub_power = (num_big * 1000) + num_small; 
			} 
			else if (max1 == 3 && max2 == 2) {//2+3
				power = 6; 
				int num_big = mozliwe[4].get_numer(); if (num_big == 1) { num_big = 14; }
				int num_small = mozliwe[0].get_numer(); if (num_small == 1) { num_small = 14; }
				sub_power = (num_big * 1000) + num_small;
			}
			else if (jedenkolor) { //flush
				power = 5;
				int rzad = 1;
				sub_power = 0;
				for (int i = 0; i < 5; i++) {
					int num = mozliwe[i].get_numer(); if (num == 1) { num = 14; }
					sub_power += num * rzad;
					rzad *= 10;
				}
			}
			else if (pokolei) { power = 4; } //strit sub_power=najwyzsza karta
			else if (max1 == 3) {//trojka
				power = 3;
			}
			else if (max1 == 2 && max2 == 2) { power = 2; }//dwie pary
			else if (max1 == 2) { power = 1; }
			else { power = 0; }//high card sub_power=najwyzsza karta

			if ((power > highest_power)||(power == highest_power && sub_power > highest_sub_power)) {
				highest_power = power;
				highest_sub_power = sub_power;
				najlepsze = mozliwe;
			}
			//zrobic wlasny typ zwracany ktorym mialby int oraz 2 pola reprezentujace 2 najwyzsze karty
			mozliwe.clear();
		}

	}
	return 0;

	
}
//GAME
int Game::pool = 0;
int Game::dealer_option = 1;
int Game::whos_dealer = 0;
int Game::starting_credits = 5000;
int Game::minimal_raise = 100;
int Game::raise = 100;
bool Game::game_started = false;
bool Game::first_round = true;
bool Game::enemy_turn = false;

int Game::checked_cards = 0;
int Game::whowins = 2; //0=player 1=enemy 2=none
int Game::won_prize = 0;
std::string Game::enemy_desc = "New cards dealt.";
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
void Game::end_round(int _winner) {
	int enemy = 1 - _winner;
	won_prize = pool;
	Gracz::graczList[_winner].add_credits(Game::get_pool());
	clear_pool();
	game_started = false;
	whowins = _winner;
	//Game::enemy_desc = "New cards dealt.";
	if (Gracz::graczList[enemy].get_credits() == 0) {//bankrut - koniec gry
		//end_game(_winner);
		ending_game = _winner;
	}
}
void Game::end_game(int _winner) {
	game_started = false;
	first_round = true;
	ending_game = 2;
	whowins = 2;
	Gracz::graczList.clear();
	Gracz::graczList.push_back(Gracz());
	Gracz::graczList.push_back(Gracz());
	if (_winner == 0) { std::cout << "Wygrales!" << std::endl; }
	else { std::cout << "Przegrales!" << std::endl; }
	//game_state = main_menu
}
