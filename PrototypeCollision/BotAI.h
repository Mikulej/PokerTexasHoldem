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
	int next_move;
	//MCTS core functions
	
	void selection(void);
	void expansion(void);
	
	void update(void);
public:
	void init_point(const std::vector<Karta>& hand_cards, bool _isDealer);//do testow
	void simulation(int bot_credits,int enemy_credits,int _pool,int first_move);//first_move 0-raise by random num 1-call 2- fold//do testow
	Point();
	~Point() = default;
};
