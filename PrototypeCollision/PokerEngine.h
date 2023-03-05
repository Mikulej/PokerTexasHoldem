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
	//int k;
	//int n;
	static void set_rewers(int);
	//Karta(int, int);
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
	std::vector<Karta> reka;
	static std::vector<Gracz> graczList;
	void add_credits(int);
	int get_credits(void);
	void wez_karty_z_talii();
	void raise(int);
	void random_action();

};
//BOT
class Bot : public Gracz {

};
//GAME czy ta klasa wgl jest potrzebna?
class Game {
	static int pool;

public:
	static int dealer_option;//wybor
	static int whos_dealer;//0=player 1=enemy
	static int starting_credits;
	static int minimal_raise;
	static int raise;
	static bool game_started;
	static bool enemy_turn;
	static std::string enemy_desc;
	static void clear_pool(void);
	static void add_pool(int);
	static int get_pool(void);
	static std::vector<Karta> talia;
	static std::vector<Karta> stol;
	static void stworz_talie(std::vector<Karta>&);
};