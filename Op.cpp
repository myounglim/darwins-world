#include "Op.h"
#include "Machine.h"
#include <iostream>
#include "World.h"

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam << std::endl;
}

//rotate operation
void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	//std::cout << "Performing rotate operation: \n";
	/*std::cout << "Now facing ";
	switch (state.mFacing)
	{
	case (MachineState::UP):
		std::cout << "UP";
		break;
	case (MachineState::RIGHT):
		std::cout << "RIGHT";
		break;
	case (MachineState::DOWN):
		std::cout << "DOWN";
		break;
	case (MachineState::LEFT):
		std::cout << "LEFT";
		break;
	}
	std::cout << std::endl;*/
	state.mProgramCounter++;
	state.mActionsTaken++;
}

//go to operation 
void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter = mParam;
	//std::cout << "Performing GoTo operation line number now is: " << state.mProgramCounter << "\n";
}

//test wall: test for wall on the borders of the grid 
//program counter incremented
//does NOT consume an action 
void OpTestWall::Execute(MachineState& state)
{
	DebugOutput(state);

	//top side
	if (state.mYPosition == 0 && state.mFacing == MachineState::UP) {
		state.mTest = true;
	}
	//right side
	else if (state.mXPosition == 19 && state.mFacing == MachineState::RIGHT) {
		state.mTest = true;
	}
	//bottom side
	else if (state.mYPosition == 19 && state.mFacing == MachineState::DOWN) {
		state.mTest = true;
	}
	//left side
	else if (state.mXPosition == 0 && state.mFacing == MachineState::LEFT) {
		state.mTest = true;
	}
	else {
		state.mTest = false; 
	}
	state.mProgramCounter++;		//increment the program counter 
	//std::cout << "Performing testWall operation: " << state.mTest << "\n";
}

//JE jump line if flag true 
//else simply go to the next line 
void OpJE::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.mTest) {
		state.mProgramCounter = mParam;
	}
	else
		state.mProgramCounter++; 
	//std::cout << "Performing JE operation, line number now: " << state.mProgramCounter << "\n";
}

//JNE jump line if flag false (opposite of JE) 
void OpJNE::Execute(MachineState& state)
{
	DebugOutput(state);
	if (!state.mTest)
		state.mProgramCounter = mParam;
	else
		state.mProgramCounter++; 
	//std::cout << "Performing JNE operation, line number now: " << state.mProgramCounter << "\n";
}

//Move Forward, if possible 
//Consumes an action point 
void OpForward::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
	
	bool ifPossible = true; 
	switch (state.mFacing)
	{
	case (MachineState::UP):
		//check for presence of wall 
		if (state.mYPosition == 0)
			ifPossible = false;
		//check for zombie presence
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mXPosition == (*it)->mXPosition && state.mYPosition - 1 == (*it)->mYPosition) {
				ifPossible = false;
				break;
				
			}
		}
		//check for human presence 
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mXPosition == it->mXPosition && state.mYPosition - 1 == it->mYPosition) {
				ifPossible = false;
				break;
			}
		}

		if (ifPossible) {
			state.mYPosition--; 
		}
		break;
	case (MachineState::RIGHT):
		if (state.mXPosition == 19)
			ifPossible = false; 
		// check for zombie presence
			for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
				if (state.mYPosition == (*it)->mYPosition && state.mXPosition + 1 == (*it)->mXPosition) {
					ifPossible = false;
					break;
				}
			}
		//check for human presence 
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mYPosition == it->mYPosition && state.mXPosition + 1 == it->mXPosition) {
				ifPossible = false;
				break;
			}
		}

		if (ifPossible) {
			state.mXPosition++;
		}
		break;
	case (MachineState::DOWN):
		if (state.mYPosition == 19)
			ifPossible = false; 
		// check for zombie presence
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mXPosition == (*it)->mXPosition && state.mYPosition + 1 == (*it)->mYPosition) {
				ifPossible = false;
				break;
			}
		}
		//check for human presence 
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mXPosition == it->mXPosition && state.mYPosition + 1 == it->mYPosition) {
				ifPossible = false;
				break;
			}
		}

		if (ifPossible) {
			state.mYPosition++;
		}
		break;
	case (MachineState::LEFT):
		if (state.mXPosition == 0)
			ifPossible = false; 
		// check for zombie presence
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mYPosition == (*it)->mYPosition && state.mXPosition - 1 == (*it)->mXPosition) {
				ifPossible = false;
				break;
			}
		}
		//check for human presence 
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mYPosition == it->mYPosition && state.mXPosition - 1 == it->mXPosition) {
				ifPossible = false;
				break;
			}
		}

		if (ifPossible) {
			state.mXPosition--;
		}
		break;
	}	//end switch

	//std::cout << "Performing forward operation...XPos: " << state.mXPosition << " YPos: " << state.mYPosition << "\n";
}

//Test Random: randomly set the test flag true or false 
//Move to the next line but this does NOT consume an action 
void OpTestRandom::Execute(MachineState& state)
{
	DebugOutput(state);
	if (rand() % 2 == 0) {
		state.mTest = true;
	}
	else
		state.mTest = false; 
	state.mProgramCounter++;
	//std::cout << "Performing TestRandom operation, flag is: " << state.mTest << "\n";
}

//Test Passable: set flag to true if facing an open tile 
//does NOT consume an action point 
void OpTestPassable::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mTest = true;
	state.mProgramCounter++;

	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (state.mYPosition == 0)
			state.mTest = false;
		else {
			//check for zombie presence
			for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
				if (state.mXPosition == (*it)->mXPosition && state.mYPosition - 1 == (*it)->mYPosition) {
					state.mTest = false;
					break;
				}
			}
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
				if (state.mXPosition == it->mXPosition && state.mYPosition - 1 == it->mYPosition) {
					state.mTest = false;
					break;
				}
			}
		}
		break;
	case (MachineState::RIGHT):
		if (state.mXPosition == 19)
			state.mTest = false; 
		else {
			//check for zombie presence
			for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
				if (state.mYPosition == (*it)->mYPosition && state.mXPosition + 1 == (*it)->mXPosition) {
					state.mTest = false;
					break;
				}
			}
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
				if (state.mYPosition == it->mYPosition && state.mXPosition + 1 == it->mXPosition) {
					state.mTest = false;
					break;
				}
			}
		}
		break;
	case (MachineState::DOWN):
		if (state.mYPosition == 19)
			state.mTest = false;
		else {
			//check for zombie presence
			for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
				if (state.mXPosition == (*it)->mXPosition && state.mYPosition + 1 == (*it)->mYPosition) {
					state.mTest = false;
					break;
				}
			}
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
				if (state.mXPosition == it->mXPosition && state.mXPosition + 1 == it->mXPosition) {
					state.mTest = false;
					break;
				}
			}
		}
		break;
	case (MachineState::LEFT): 
		if (state.mXPosition == 0)
			state.mTest = false; 
		else {
			//check for zombie presence
			for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
				if (state.mYPosition == (*it)->mYPosition && state.mXPosition - 1 == (*it)->mXPosition) {
					state.mTest = false;
					break;
				}
			}
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
				if (state.mYPosition == it->mYPosition && state.mXPosition - 1 == it->mXPosition) {
					state.mTest = false;
					break;
				}
			}
		}
		break;
	}	//end switch 
	//std::cout << "Performing passable operation: " << state.mTest << "\n";
}

//Test Zombie
//depending on the orientation, iterate through the zombie vector and check 
void OpTestZombie::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mTest = false;
	switch (state.mFacing)
	{
	case (MachineState::UP):
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mXPosition == (*it)->mXPosition &&  state.mYPosition - mParam == (*it)->mYPosition) {
				state.mTest = true;
				break; 
			}
		}
		break;

	case (MachineState::RIGHT):
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mYPosition == (*it)->mYPosition &&  state.mXPosition + mParam == (*it)->mXPosition) {
				state.mTest = true;
				break;
			}
		}
		break;

	case (MachineState::DOWN):
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mXPosition == (*it)->mXPosition &&  state.mYPosition + mParam == (*it)->mYPosition) {
				state.mTest = true;
				break;
			}
		}
		break;

	case (MachineState::LEFT): 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it) {
			if (state.mYPosition == (*it)->mYPosition &&  state.mXPosition - mParam == (*it)->mXPosition) {
				state.mTest = true;
				break;
			}
		}
		break;
	}	//end switch

	//std::cout << "Performing test zombie operation: " << state.mTest << " Parameter: " << mParam << "\n";
}

//Test Human
//depending on the orientation, iterate through human vector and check 
void OpTestHuman::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mTest = false;
	switch (state.mFacing)
	{
	case (MachineState::UP):
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mXPosition == it->mXPosition && state.mYPosition - mParam == it->mYPosition) {
				state.mTest = true;
				break;
			}
		}
		break;

	case (MachineState::RIGHT):
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mYPosition == it->mYPosition &&  state.mXPosition + mParam == it->mXPosition) {
				state.mTest = true;
				break;
			}
		}
		break;

	case (MachineState::DOWN):
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mXPosition == it->mXPosition &&  state.mYPosition + mParam == it->mYPosition) {
				state.mTest = true;
				break;
			}
		}
		break;

	case (MachineState::LEFT): 
		for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it) {
			if (state.mYPosition == it->mYPosition &&  state.mXPosition - mParam == it->mXPosition) {
				state.mTest = true;
				break;
			}
		}
		break;
	}	//end switch
	//std::cout << "Performing test human operation: " << state.mTest << " Parameter: " << mParam << "\n";
}

//Attack
//depending on the orientation, check for zombie or human presence.
//if zombie attacks zombie, nothing happens
//if zombie attacks human, kill and infect the human
//human attacking zombie, zombie dies....human attacking human, human dies 
void OpAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;

	//determine if the current is a zombie 
	bool isZombie = state.GetInfect(); 
	bool hitSomeone = false; 

	int index = 0; 
	int zombieSize = World::get().mZombieState.size(); 
	switch (state.mFacing)
	{
	case (MachineState::UP):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mXPosition == (*it)->mXPosition && state.mYPosition - 1 == (*it)->mYPosition) {
				hitSomeone = true; 
				if (isZombie) {
					//zombie attacking zombie does nothing 
				}
				else {
					// is a human so kill the zombie: delete zombie from zombie vector 
					World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				}
				break;
			}
		}
		if (!hitSomeone) 
		{
			index = 0; 
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mXPosition == it->mXPosition && state.mYPosition - 1 == it->mYPosition) {
					if (isZombie) {
						//INFECT...zombie attacking human: delete human from human vector and add to zombie vector then bind
						World::get().mZombieState.push_back(std::make_shared<MachineState>());
						World::get().mZombieState[zombieSize]->mXPosition = it->mXPosition;
						World::get().mZombieState[zombieSize]->mYPosition = it->mYPosition;
						World::get().mZombieMachine.BindState(*World::get().mZombieState[zombieSize]);
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					else {
						//human attacking human: delete human from human vector 
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					break;
				}
			}
		}
		break;

	case (MachineState::RIGHT):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mYPosition == (*it)->mYPosition && state.mXPosition + 1 == (*it)->mXPosition) {
				hitSomeone = true;
				if (isZombie) {
					//zombie attacking zombie does nothing 
				}
				else {
					// is a human so kill the zombie: delete zombie from zombie vector 
					World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				}
				break;
			}
		}
		if (!hitSomeone) 
		{
			index = 0; 
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mYPosition == it->mYPosition && state.mXPosition + 1 == it->mXPosition) {
					if (isZombie) {
						//INFECT...zombie attacking human: delete human from human vector and add to zombie vector then bind
						World::get().mZombieState.push_back(std::make_shared<MachineState>());
						World::get().mZombieState[zombieSize]->mXPosition = it->mXPosition;
						World::get().mZombieState[zombieSize]->mYPosition = it->mYPosition;
						World::get().mZombieMachine.BindState(*World::get().mZombieState[zombieSize]);
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					else {
						//human attacking human: delete human from human vector 
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					break;
				}
			}
		}
		break;

	case (MachineState::DOWN):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mXPosition == (*it)->mXPosition && state.mYPosition + 1 == (*it)->mYPosition) {
				hitSomeone = true;
				if (isZombie) {
					//zombie attacking zombie does nothing 
				}
				else {
					// is a human so kill the zombie: delete zombie from zombie vector 
					World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				}
				break;
			}
		}
		if (!hitSomeone) 
		{
			index = 0; 
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mXPosition == it->mXPosition && state.mYPosition + 1 == it->mYPosition) {
					if (isZombie) {
						//INFECT...zombie attacking human: delete human from human vector and add to zombie vector then bind
						World::get().mZombieState.push_back(std::make_shared<MachineState>());
						World::get().mZombieState[zombieSize]->mXPosition = it->mXPosition;
						World::get().mZombieState[zombieSize]->mYPosition = it->mYPosition;
						World::get().mZombieMachine.BindState(*World::get().mZombieState[zombieSize]);
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					else {
						//human attacking human: delete human from human vector 
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					break;
				}
			}
		}
		break;

	case (MachineState::LEFT):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mYPosition == (*it)->mYPosition && state.mXPosition - 1 == (*it)->mXPosition) {
				hitSomeone = true;
				if (isZombie) {
					//zombie attacking zombie does nothing 
				}
				else {
					// is a human so kill the zombie: delete zombie from zombie vector 
					World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				}
				break;
			}
		}

		if (!hitSomeone) 
		{
			index = 0; 
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mYPosition == it->mYPosition && state.mXPosition - 1 == it->mXPosition) {
					if (isZombie) {
						//INFECT...zombie attacking human: delete human from human vector and add to zombie vector then bind
						World::get().mZombieState.push_back(std::make_shared<MachineState>());
						World::get().mZombieState[zombieSize]->mXPosition = it->mXPosition;
						World::get().mZombieState[zombieSize]->mYPosition = it->mYPosition;
						World::get().mZombieMachine.BindState(*World::get().mZombieState[zombieSize]);
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					else {
						//human attacking human: delete human from human vector 
						World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					}
					break;
				}
			}
		}
		break;
	}	//end switch
}


//Ranged Attack
//depending on the orientation, check for zombie or human presence
//if zombie tries to use this move, throw an exception! 
void OpRangedAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;

	//determine if the current is a zombie 
	bool isZombie = state.GetInfect();
	bool hitSomeone = false;

	int index = 0;
	int zombieSize = World::get().mZombieState.size();
	
	//zombies cannot use range attacks 
	if (isZombie)
		throw InvalidOp(); 

	switch (state.mFacing)
	{
	case (MachineState::UP):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mXPosition == (*it)->mXPosition && state.mYPosition - 2 == (*it)->mYPosition) {
				hitSomeone = true;
				// human attacking zombie: delete zombie from zombie vector 
				World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				break;
			}
		}
		if (!hitSomeone)
		{
			index = 0;
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mXPosition == it->mXPosition && state.mYPosition - 2 == it->mYPosition) {
					//human attacking human: delete human from human vector 
					World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					break;
				}
			}
		}
		break;

	case (MachineState::RIGHT):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mYPosition == (*it)->mYPosition && state.mXPosition + 2 == (*it)->mXPosition) {
				hitSomeone = true;
				// human attacking zombie: delete zombie from zombie vector 
				World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				break;
			}
		}
		if (!hitSomeone) {
			index = 0;
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mYPosition == it->mYPosition && state.mXPosition + 2 == it->mXPosition) {
					//human attacking human: delete human from human vector 
					World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					break;
				}
			}
		}
		break;

	case (MachineState::DOWN):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mXPosition == (*it)->mXPosition && state.mYPosition + 2 == (*it)->mYPosition) {
				hitSomeone = true;
				// human attacking zombie: delete zombie from zombie vector 
				World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				break;
			}
		}
		if (!hitSomeone) {
			index = 0;
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mXPosition == it->mXPosition && state.mYPosition + 2 == it->mYPosition) {
					//human attacking human: delete human from human vector 
					World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					break;
				}
			}
		}
		break;

	case (MachineState::LEFT):
		//check for zombie presence
		index = 0; 
		for (auto it = World::get().mZombieState.begin(); it != World::get().mZombieState.end(); ++it, index++) {
			if (state.mYPosition == (*it)->mYPosition && state.mXPosition - 2 == (*it)->mXPosition) {
				hitSomeone = true;
				// human attacking zombie: delete zombie from zombie vector 
				World::get().mZombieState.erase(World::get().mZombieState.begin() + index);
				break;
			}
		}
		if (!hitSomeone) {
			index = 0;
			//check for human presence 
			for (auto it = World::get().mHumanState.begin(); it != World::get().mHumanState.end(); ++it, index++) {
				if (state.mYPosition == it->mYPosition && state.mXPosition - 2 == it->mXPosition) {
					//human attacking human: delete human from human vector 
					World::get().mHumanState.erase(World::get().mHumanState.begin() + index);
					break;
				}
			}
		}
		break;
	}	//end switch
	
}

//Endturn
void OpEndTurn::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}
