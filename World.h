#pragma once

#include "Singleton.h"
#include "Machine.h"
#include "Traits.h"
#include <vector>

/*
* The current status of the "world" 
* Keeps track of the state of each zombie and the state of each human which are stored in two separate vectors. 
*/

class World : public Singleton<World>
{
	DECLARE_SINGLETON(World)
public:
	World();
	~World();

	//state of each zombie 
	std::vector<std::shared_ptr<MachineState>> mZombieState; 

	Machine<ZombieTraits> mZombieMachine;

	//state of each human 
	std::vector<MachineState> mHumanState;

	Machine<HumanTraits> mHumanMachine; 
};

