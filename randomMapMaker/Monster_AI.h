#pragma once

enum AI_recog
{
	c_sleep = 0,
	wandering,
	chasing,
};

enum AI_action {
	_c_sleep = 0,
	wandering_act,
	chasing_act,

};



class AI_Pattern {
	int current_state;
	virtual void _check_condition() = 0;
	virtual void _action() = 0;
};

class Monster_AI
{
public:
	std::vector<AI_Pattern> ai_state;
	Monster_AI();
	~Monster_AI();
	//void add_pattern(AI_Pattern pattern);
	//void del_pattern(AI_Pattern pattern);
	void recog_condition(AI_recog recog);
};




class Wandering : public AI_Pattern {
	int current_state;
	void _check_condition();
	void _action();
};

class Chasing : public AI_Pattern {
	int current_state;
	void _check_condition();
	void _action();
	
};