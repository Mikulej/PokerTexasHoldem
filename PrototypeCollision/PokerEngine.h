#pragma once
#include "StaticObject.h"
#include "Text.h"
#include <vector>
#include <algorithm>
//KARTA
class Karta {
	friend class Gracz;//tylko do testow przy tworzeniu sztucznej talii kart! get_power()
	static std::vector<Karta> komplet;
	
	static int rewers;//{icefire = 1,science=2,fish=4,nuts=5,rose=10 }
	static int rewers_id;
	enum kolor { wino, czerwo, zoladz, dzwonek } k;
	enum numer { as = 1, dwa, trzy, cztery, piec, szesc, siedem, osiem, dziewiec, dziesiec, walet, dama, krol } n;
	static void set_rewers(int);
	Karta(kolor, numer);
	Karta(int, int);
public:
	bool operator==(const Karta& right);
	static void stworz_komplet(std::vector<Karta>&);
	int get_kolor() const;
	int get_numer() const;
	static const std::vector<Karta>& get_komplet();
	static void KartaInit();
	static int get_rewers();
	static void next_rewers();
	static void prev_rewers();

};
//GRACZ
class Gracz {
	int credits;
	int power;
	int sub_power;
public:
	
	static int next_move;
	bool allin;
	bool checks;
	bool folds;
	int gave_to_pool;
	std::vector<Karta> reka;
	static std::vector<Gracz> graczList;
	void add_credits(int);
	int get_credits(void);
	void wez_karty_z_talii();
	void raise(int);
	void call(int);
	void fold(int);
	void bot_action(int fin_min_raise);
	void give_to_pool(int);
	void calculate_power();
	int get_power(void);
	int get_sub_power(void);
	//do symulacji
	void random_action(void);
	void sim_give_to_pool(int _val,int &_pool);
};
//GAME czy ta klasa wgl jest potrzebna?
class Game {
public:
	static int pool;
	static int dealer_option;//wybor
	static int whos_dealer;//0=player 1=enemy
	static int starting_credits;
	static int minimal_raise;
	static int raise;
	static bool game_started;
	static bool first_round;
	static bool enemy_turn;
	static int checked_cards;
	static int whowins; //0=player 1=enemy 2=none
	static int won_prize; //wartosc pool na koncu poprzedniej rundy
	static bool missing_blind;//sprawdza czy wszystkie blindy zostala poprawnie przydzielone
	static std::string enemy_desc;
	static void clear_pool(void);
	static int get_pool(void);
	static std::vector<Karta> talia;
	static std::vector<Karta> stol;
	static void stworz_talie(std::vector<Karta>&);
	static void stworz_stol();
	static void blinds(int);
	static void end_round(int);
	static void end_game(int);
};