#include <iostream>
#include <random>

#include<vector>
#include<string>
#include "UriBehaviorTree.h"


//Exercise 1 Action Nodes
class MoveAway : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Move Away" << std::endl;
		return NodeStatus::Success;
	}
};

class TripleStab : public BehaviorTreeNode
{
	NodeStatus Run() override
	{
		std::cout << "Triple Stab" << std::endl;
		return NodeStatus::Success;
	}
};

class NormalAttack : public BehaviorTreeNode
{
	NodeStatus Run() override
	{
		std::cout << "Normal Attack" << std::endl;
		return NodeStatus::Success;
	}
};

class GetCloser : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "GetCloser" << std::endl;
		return NodeStatus::Success;
	}
};

//Exercise 2 Action Nodes
class SwingSword : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Swing sword" << std::endl;
		return NodeStatus::Success;
	}
};

class Taunt : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Taunt the player" << std::endl;
		return NodeStatus::Success;
	}
};

class Wander : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Wander" << std::endl;
		return NodeStatus::Success;
	}
};


class FindAid : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Find aid" << std::endl;
		return NodeStatus::Success;
	}
};

class Evade : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Evade" << std::endl;
		return NodeStatus::Success;
	}
};

int main()
{



	std::cout << "----------------------" << std::endl;
	std::cout << "Exercise 1: " << std::endl;
	//Exercise 1

	//To Do 2: Identify the nodes from the diagram: 
		//4 Action Nodes: 
			//- Triple Stab
			//- Normal Attack
			//- Get Closer 
			//- Move Away
		//4 Control Flow Nodes: 
			// - 25% Change Triple Stab Ability (WeightedRandomDistribution)
			// - Ability Up? (SwitchConditionNode)
			// - Is the opponent inside my range attack? (SwitchConditionNode)
			// - Am I low of health? (SwitchConditionNode) /Root/

	//To Do 3: Action Nodes
	auto tripleStab = std::make_shared<TripleStab>();
	auto normalAttack = std::make_shared<NormalAttack>();
	auto getCloser = std::make_shared<GetCloser>();
	auto moveAway = std::make_shared<MoveAway>();

	//To Do 4: Control Flow Nodes
	auto stabAbilityWeighted = std::make_shared<WeightedRandomDistribution>();
	stabAbilityWeighted->AddChild(tripleStab, 0.10);
	stabAbilityWeighted->AddChild(normalAttack, 0.90);
	stabAbilityWeighted->SetNodeName("stabAbilityWeighted");

	auto abilityUp = std::make_shared<SwitchConditionNode>(stabAbilityWeighted, normalAttack);
	abilityUp->SetCondition(true);
	abilityUp->SetNodeName("abilityUp");

	auto isInsideRange = std::make_shared<SwitchConditionNode>(abilityUp, getCloser);
	isInsideRange->SetCondition(true);
	isInsideRange->SetNodeName("isInsideRange");

	auto checkHealth = std::make_shared<SwitchConditionNode>(moveAway, isInsideRange);
	checkHealth->SetNodeName("checkHealth");

	//To Do 5: Creating Behavior Tree Object with his root. And enabling Debug Mode.
	BehaviorTree tree;
	tree.SetDebugEnabled(true);
	tree.SetRoot(checkHealth);

	//To Do 6: Running our Behavior tree
	tree.Run();
	std::cout << "----------------------" << std::endl;

	//------------------------------------------------------------------------------------------//

	std::cout << "Exercise 2: " << std::endl;
	//Exercise 1
	//To Do 2: Identify the nodes from the diagram: 
		//5 Action Nodes: 
			//- Swing sword (15)
			//- Taunt the player (13)
			//- Wander (11)
			//- Find aid (7)
			//- Evade (4)

		//10 Control Flow Nodes: 
			// - Player in front? (14) (Condition Node) 
			// - Attack player with sword (12) (Sequence Node)
			// - Attack (10) (Selector Node)
			// - Player is in line of sight? (9) (Condition Node)
			// - (8) (Sequence Node)
			// - Has low HP? (6) (condition Node)
			// - (5) (Sequence Node)
			// - Player is attacking? (3) (Condition Node)
			// - (2) (Sequence Node)
			// - (1) (Selector Node) /Root/

	//To Do 3: Action Nodes
	auto swingSword = std::make_shared<SwingSword>();
	auto tauntThePlayer = std::make_shared<Taunt>();
	auto wander = std::make_shared<Wander>();
	auto findAid = std::make_shared<FindAid>();
	auto evade = std::make_shared<Evade>();

	//To Do 4: Control Flow Nodes
	auto playerInFront = std::make_shared<ConditionNode>();
	playerInFront->SetTest(true);
	playerInFront->SetNodeName("playerInFront");

	auto sequenceAttackWithSword = std::make_shared<SequenceNode>();
	sequenceAttackWithSword->AddChild(playerInFront);
	sequenceAttackWithSword->AddChild(swingSword);
	sequenceAttackWithSword->SetNodeName("sequenceAttackWithSword");

	auto selectorAttack = std::make_shared<SelectorNode>();
	selectorAttack->AddChild(sequenceAttackWithSword);
	selectorAttack->AddChild(tauntThePlayer);
	selectorAttack->SetNodeName("selectorAttack");

	auto playerIsInSight = std::make_shared<ConditionNode>();
	playerIsInSight->SetTest(true);
	playerIsInSight->SetNodeName("playerIsInSight");

	auto sequenceNode8 = std::make_shared<SequenceNode>();
	sequenceNode8->AddChild(playerIsInSight);
	sequenceNode8->AddChild(selectorAttack);
	sequenceNode8->SetNodeName("sequenceNode8");

	auto hasLowHP = std::make_shared<ConditionNode>();
	hasLowHP->SetTest(false);
	hasLowHP->SetNodeName("hasLowHP");

	auto sequenceNode5 = std::make_shared<SequenceNode>();
	sequenceNode5->AddChild(hasLowHP);
	sequenceNode5->AddChild(findAid);
	sequenceNode5->SetNodeName("sequenceNode5");
	
	auto playerIsAttacking = std::make_shared<ConditionNode>();
	playerIsAttacking->SetTest(false);
	playerIsAttacking->SetNodeName("playerIsAttacking");

	auto sequenceNode2 = std::make_shared<SequenceNode>();
	sequenceNode2->AddChild(playerIsAttacking);
	sequenceNode2->AddChild(evade);
	sequenceNode2->SetNodeName("sequenceNode2");

	auto rootSelectorNode = std::make_shared<SelectorNode>();
	rootSelectorNode->AddChild(sequenceNode2);
	rootSelectorNode->AddChild(sequenceNode5);
	rootSelectorNode->AddChild(sequenceNode8);
	rootSelectorNode->AddChild(wander);
	rootSelectorNode->SetNodeName("rootSelectorNode");

	//To Do 5: Creating Behavior Tree Object with his root. And enabling Debug Mode.
	BehaviorTree treeExercise2;
	treeExercise2.SetDebugEnabled(true);
	treeExercise2.SetRoot(rootSelectorNode);

	//To Do 6: Running our Behavior tree
	treeExercise2.Run();


	std::cout << "----------------------" << std::endl;



	//auto heal = std::make_shared<Action>();
	//auto attack = std::make_shared<ActionDos>();
	//auto cond = std::make_shared<ConditionNode>();
	//auto seq = std::make_shared<SequenceNode>();
	//auto uniform = std::make_shared<RandomUniformDistribution>(2);

	//auto weighted = std::make_shared<WeightedRandomDistribution>(a);

	//auto bernoulliWrap = std::make_shared<RandomBernoulliDistribution>(0.1);
	//bernoulliWrap->SetChild(heal);
	
	//auto inverter = std::make_shared<Inverter>();
	//inverter->SetChild(heal);

	//auto succeder = std::make_shared<Succeeder>();
	//succeder->SetChild(heal);

	//auto failer = std::make_shared<Failer>();
	//failer->SetChild(heal);

	//auto repeater = std::make_shared<Repeater>(5);
	//repeater->SetChild(heal);
	
	//seq->AddChild(succeder);
	//seq->AddChild(inverter);
	//seq->AddChild(failer);
	//seq->AddChild(repeater);
	//seq->AddChild(bernoulliWrap);
	//seq->AddChild(heal);
	//seq->AddChild(cond);
	//seq->AddChild(attack);
	//tree.SetRoot(seq);


	//uniform->AddChild(heal);
	//uniform->AddChild(attack);
	//tree.SetRoot(uniform);

	//weighted->AddChild(heal, 0.10);
	//weighted->AddChild(attack, 1.50);
	//tree.SetRoot(weighted);


















	return 0;
}