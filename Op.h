// Defines all the classes for different operations
// which can be performed.
#pragma once

#include "Exceptions.h"

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const char* OpName, int parameter)
		: mOpName(OpName)
		, mParam(parameter)
	{ }

	void DebugOutput(MachineState& state);

	virtual void Execute(MachineState& state) = 0;

	MachineState* GetTarget(MachineState& state, int n = 1);
	
	virtual ~Op() { }
protected:
	int mParam;
	const char* mOpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op("OpRotate", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op("OpGoto", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the TestWall operation
struct OpTestWall : Op
{
	OpTestWall(int parameter)
		: Op("OpTestWall", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the JE operation
struct OpJE : Op
{
	OpJE(int parameter)
		: Op("OpJE", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the JNE if false flag operation
struct OpJNE : Op
{
	OpJNE(int parameter)
		: Op("OpJNE", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Forward operation 
struct OpForward : Op
{
	OpForward(int parameter)
		: Op("OpForward", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Test Random operation 
struct OpTestRandom : Op
{
	OpTestRandom(int parameter)
		: Op("OpTestRandom", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Test Passable operation 
struct OpTestPassable : Op
{
	OpTestPassable(int parameter)
		: Op("OpTestPassable", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Test Zombie operation 
struct OpTestZombie : Op
{
	OpTestZombie(int parameter)
		: Op("OpTestZombie", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Test Human operation 
struct OpTestHuman : Op
{
	OpTestHuman(int parameter)
		: Op("OpTestHuman", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Attack operation 
struct OpAttack : Op
{
	OpAttack(int parameter)
		: Op("OpAttack", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Ranged attack operation 
struct OpRangedAttack : Op, InvalidOp
{
	OpRangedAttack(int parameter)
		: Op("OpRangedAttack", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the Ranged attack operation 
struct OpEndTurn : Op
{
	OpEndTurn(int parameter)
		: Op("OpEndTurn", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};


