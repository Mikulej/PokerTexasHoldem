#pragma once
#include "PokerEngine.h"
#include <vector>
#include <algorithm>


/// <summary>
///    O <- Point
///  / | \
/// O  O  O <-(Child)Nodes
/// </summary>
/// 
///gra konczy sie kiedy komus skoncza sie kredyty
class Point {
	std::vector<Point> nodes;
	Point* prev;
	int simulations;

	static std::vector<Karta> hand_cards;
	static bool isDealer;

	//MCTS core functions
	void init_point(const std::vector<Karta>& hand_cards, bool _isDealer);
	void selection(void);
	void expansion(void);
	void simulation(void);
	void update(void);
public:
	Point();
	~Point() = default;
};
