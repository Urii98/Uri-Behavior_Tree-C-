#include <iostream>
#include <random>

#include<vector>
#include<string>
#include "UriBehaviorTree.h"

class Action : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Me tengo que curar!" << std::endl;
		return NodeStatus::Success;
	}
};

class ActionDos : public BehaviorTreeNode
{
public:
	NodeStatus Run() override
	{
		std::cout << "Tengo que atacarle!" << std::endl;
		return NodeStatus::Success;
	}
};

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

bool giveMeTrue()
{
	return true;
}
int main()
{

	auto getCloser = std::make_shared<GetCloser>();
	auto moveAway = std::make_shared<MoveAway>(); 
	auto normalAttack = std::make_shared<NormalAttack>();
	auto tripleStab = std::make_shared<TripleStab>();

	auto stabAbilityWeighted = std::make_shared<WeightedRandomDistribution>();
	stabAbilityWeighted->AddChild(tripleStab, 0.10);
	stabAbilityWeighted->AddChild(normalAttack, 0.90);
	stabAbilityWeighted->nodeName = "stabAbilityWeighted";

	auto abilityUp = std::make_shared<SwitchConditionNode>(stabAbilityWeighted, normalAttack);
	abilityUp->SetCondition(true);
	abilityUp->nodeName = "abilityUp";

	auto isInsideRange = std::make_shared<SwitchConditionNode>(abilityUp, getCloser);
	isInsideRange->SetCondition(true);
	isInsideRange->nodeName = "isInsideRange";

	auto cheackHealth = std::make_shared<SwitchConditionNode>(moveAway, isInsideRange);
	cheackHealth->nodeName = "cheackHealth";
	

	BehaviorTree tree; 

	tree.SetDebugEnabled(true);
	tree.SetRoot(cheackHealth);
	tree.Run();

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



	
	//tree.SetRoot(weighted);
	//tree.Run();

	//std::default_random_engine e(time(0));
	//std::uniform_real_distribution<float> u(0,1);
	//std::uniform_int_distribution<int> a(0,5);
	//std::normal_distribution<> n(4, 1.5); 
	//std::bernoulli_distribution b(.75); //probabilidad de true, por defecto está en 50%

	//for (int i = 0; i < 10; i++)
	//{
	//	//std::cout << e() << std::endl;
	//	std::cout << u(e) << std::endl;
	//	//std::cout << n(e) << std::endl;
	// 
	//}

	//bool test = b(e);
	//std::cout << (test ? "Test 1" : "test 2") << std::endl;

	////printar la distribución
	//std::vector<unsigned> vals(9);
	//for(int i = 0; i != 200; ++i)
	//{
	//	unsigned v = lround(n(e));
	//	if (v < vals.size())
	//		++vals[v];
	//}

	//for (int i = 0; i != vals.size(); ++i)
	//{
	//	std::cout << i << ": " << std::string(vals[i], '*') << std::endl;
	//}





	return 0;
}