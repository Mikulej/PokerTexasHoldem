#pragma once
#include "PokerEngine.h"
#include <vector>
#include <algorithm>

struct SimData {
	bool isDealer;
	int next_move; int raise_by;//do next move 0-raise 1-call 2-fold
	int bot_credits; int bot_gave;
	int enemy_credits; int enemy_gave;
	int pool; int checked_cards;
	bool enemy_checks;
};
/// <summary>
///    O <- Point
///  / | \
/// O  O  O <-(Child)Nodes
/// </summary>
/// 
///gra konczy sie kiedy komus skoncza sie kredyty
class Point {
	Point* node_raise, * node_call, * node_fold;
	Point* prev;
	int simulations;
	int wins;

	static std::vector<Karta> hand_cards;
	static std::vector<Karta> desk_cards;
	SimData sd;
	//MCTS core functions
	Point* selection(void);//wybiera node ktory bedziemy sprawdzac "co dalej?" - wybieramy najbardziej obiecujacy
	void expansion(void);//tworzy 3 node'y reprezentujace ruchy ktore mozna wykonac
	bool simulation(void);// zwraca czy bool czy bot wygral - takze uaktualnie SimData sd do stanu PO wykonaniu zaplanowego z gory ruchu
	void update(bool _won);
	inline double calc_uct(Point* head);

	Point(Point* _prev, SimData _sd);
public:
	int get_next_move(void);
	int get_raise_by(void);
	Point(const std::vector<Karta>& _hand_cards, const std::vector<Karta>& _desk_cards, bool _isDealer, int _bot_credits, int _bot_gave, int _enemy_credits, int _enemy_gave, bool _enemy_checks, int _pool, int _checked_cards);
	~Point();
};

