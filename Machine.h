#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include "Op.h"
#include "Exceptions.h"

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
	{
		this->mXPosition = 0;
		this->mYPosition = 0; 
	}

	~MachineState()
	{
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;

	//current x and y position of the zombie or human
	int mXPosition;
	int mYPosition;  

	int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
	bool GetInfect() const noexcept { return mInfectOnAttack; }
private:
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	//save the loaded file to display on the panel 
	std::string loadedFile;

	//the number of turns passed 
	int numTurns; 
	// Destructor
	~Machine();
private:
	std::vector<std::shared_ptr<Op>> mOps;
};

/*
* Read in the file and store in the mOps vector 
*/
template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	std::cout << "In load machine" << "\n";
	mOps.clear();
	
	// Requires <fstream>
	std::ifstream::pos_type size;

	// Open the file for input, in default text mode, and start beg (at the beginning)
	std::ifstream file(filename, std::ios::in | std::ios::beg);
	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line)) 
		{
			std::string parameter;
			if (line.find("test_wall") != std::string::npos) {
				mOps.push_back(std::make_shared<OpTestWall>(0));
				//std::cout << "Read test_wall\n";
			}
			else if (line.find("forward") != std::string::npos) {
				mOps.push_back(std::make_shared<OpForward>(0));
				//std::cout << "Read forward\n";
			}
			else if (line.find("goto") != std::string::npos) {
				std::size_t pos = line.find(",");
				parameter = line.substr(pos + 1, 1);
				mOps.push_back(std::make_shared<OpGoto>(std::stoi(parameter)));
				//std::cout << "Read goto\n";
			}
			else if (line.find("test_random") != std::string::npos) {
				mOps.push_back(std::make_shared<OpTestRandom>(0));
				//std::cout << "Read test_random\n";
			}
			else if (line.find("rotate") != std::string::npos) {
				std::size_t pos = line.find(",");
				parameter = line.substr(pos + 1, 1);
				mOps.push_back(std::make_shared<OpRotate>(std::stoi(parameter)));
				//std::cout << "Read rotate\n";
			}
			else if (line.find("attack") != std::string::npos) {
				mOps.push_back(std::make_shared<OpAttack>(0));
				//std::cout << "Read attack\n";
			}
			else if (line.find("ranged_attack") != std::string::npos) {
				mOps.push_back(std::make_shared<OpRangedAttack>(0));
				//std::cout << "Read ranged attack\n";
			}
			else if (line.find("test_human") != std::string::npos) {
				std::size_t pos = line.find(",");
				parameter = line.substr(pos + 1, 1);
				mOps.push_back(std::make_shared<OpTestHuman>(std::stoi(parameter)));
				//std::cout << "Read test_human\n";
			}
			else if (line.find("test_zombie") != std::string::npos) {
				std::size_t pos = line.find(",");
				parameter = line.substr(pos + 1, 1);
				mOps.push_back(std::make_shared<OpTestZombie>(std::stoi(parameter)));
				//std::cout << "Read test_zombie\n";
			}
			else if (line.find("test_passable") != std::string::npos) {
				mOps.push_back(std::make_shared<OpTestPassable>(0));
				//std::cout << "Read test_passable\n";
			}
			else if (line.find("endturn") != std::string::npos) {
				mOps.push_back(std::make_shared<OpEndTurn>(0));
				//std::cout << "Read endturn\n";
			}
			else if (line.find("jne") != std::string::npos) {
				std::size_t pos = line.find(",");
				parameter = line.substr(pos + 1, 2);
				mOps.push_back(std::make_shared<OpJNE>(std::stoi(parameter)));
				//std::cout << "Read JNE\n";
			}
			else if (line.find("je") != std::string::npos) {
				std::size_t pos = line.find(",");
				parameter = line.substr(pos + 1, 2);
				mOps.push_back(std::make_shared<OpJE>(std::stoi(parameter)));
				//std::cout << "Read JE\n";
			}
			std::cout << "Line: " << line;
			std::cout << "\tParameter: " << parameter << "\n"; 
		}
		file.close();
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	//std::cout << "TAKING TURN" << std::endl;
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	mOps.clear();
}
