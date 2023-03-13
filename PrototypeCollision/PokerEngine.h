#pragma once
#include "StaticObject.h"
#include "Text.h"
#include <vector>
#include <algorithm>
//KARTA
class Karta {
	static std::vector<Karta> komplet;
	static void stworz_komplet(std::vector<Karta>&);
	static int rewers;//{icefire = 1,science=2,fish=4,nuts=5,rose=10 }
	static int rewers_id;
	enum kolor { wino, czerwo, zoladz, dzwonek } k;
	enum numer { as = 1, dwa, trzy, cztery, piec, szesc, siedem, osiem, dziewiec, dziesiec, walet, dama, krol } n;
	static void set_rewers(int);
	Karta(kolor, numer);
public:
	int get_kolor();
	int get_numer();
	static const std::vector<Karta>& get_komplet();
	static void KartaInit();
	static int get_rewers();
	static void next_rewers();
	static void prev_rewers();

};
//GRACZ
class Gracz {
	int credits;
public:
	bool allin;
	bool checks;
	int gave_to_pool;
	std::vector<Karta> reka;
	static std::vector<Gracz> graczList;
	void add_credits(int);
	int get_credits(void);
	void wez_karty_z_talii();
	void raise(int);
	void call(int);
	void fold(int);
	void random_action();
	void give_to_pool(int);

};
//BOT
class Bot : public Gracz {

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
	static bool enemy_turn;
	static int checked_cards;
	static int whowins; //0=player 1=enemy 2=none
	static std::string enemy_desc;
	static void clear_pool(void);
	static int get_pool(void);
	static std::vector<Karta> talia;
	static std::vector<Karta> stol;
	static void stworz_talie(std::vector<Karta>&);
	static void stworz_stol();
	static void blinds(int);
};