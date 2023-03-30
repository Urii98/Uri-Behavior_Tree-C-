#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <cassert>
#include <numeric>

//BehaviorTree
//Action/Base  : Funciona
//Condition  :Funciona
//Control Flow:
    //Selector : Funciona
    //Sequence : Funciona
    //RandomUniformDistribution : Funciona
    //WeightedRandomDistribution : Funciona
//Decorator:
    //Inverter : Funciona
    //Succeeder : Funciona
    //Failer : Funciona
    //Repeater : Funciona
    //RandomBernoulliDistribution : Funciona


//DUDAS
/*

    ¿¿¿ child->TickNode() o child.Get()->TickNode() ???

    -probar que pasa si le pasas a uno de bernoulli una probablity fuera del rango de 0 a 1
*/

enum class NodeStatus
{
	Default,
	Success,
	Failure,
	Running
};

//BehaviorTreeNode class is the base class for all behavior tree nodes.
//It defines a basic interface for running a behavior tree nodeand getting its current status.
//The class is abstract, which means that it cannot be instantiated directlyand must be subclassed to create specific types of nodes.
class BehaviorTreeNode 
{
public:

	virtual ~BehaviorTreeNode() {};
	virtual NodeStatus Run() = 0;
	virtual void ResetNode() {}

    NodeStatus TickNode()
    {
        if (currentStatus != NodeStatus::Running) {
            ResetNode();
        }

        currentStatus = Run();

        return currentStatus;
    }
    
    NodeStatus GetStatus() const { return currentStatus; }
    
protected:
    NodeStatus currentStatus = NodeStatus::Default;

};


//Each node in the tree is connected to other nodesand evaluated sequentially during tree execution, 
//from the root to the leaves.The BehaviorTree class implements this structureand provides a method 
//to run the treeand change the root of the tree as needed.
class BehaviorTree : public BehaviorTreeNode
{
public:
    BehaviorTree() {}
    BehaviorTree(const std::shared_ptr<BehaviorTreeNode> &rootNode) : BehaviorTree() { root = rootNode; }
    
    NodeStatus Run() { return root->TickNode(); }

    void SetRoot(const std::shared_ptr<BehaviorTreeNode> node) { root = node; }

private:
    std::shared_ptr<BehaviorTreeNode> root;
};


//The Condition Node represents a Boolean test that can evaluate to either true or false
//If the test evaluates to true, the node returns a SUCCESS status code, and if it evaluates 
//to false, it returns a FAILURE code.
class ConditionNode : public BehaviorTreeNode
{
public:

    virtual ~ConditionNode() {}

    NodeStatus Run() override
    {
        if (test)
        {
            currentStatus = NodeStatus::Success;
        }
        else
        {
            currentStatus = NodeStatus::Failure;
        }

        return NodeStatus::Success;

    }


public:
    bool test = false;
    //std::shared_ptr<BehaviorTreeNode> leftChildren; //trueChildren
    //std::shared_ptr<BehaviorTreeNode> rightChildren; //falseChildren
};


//A Selector Node examines its child components in sequence from left to right, 
//returning a SUCCESS code immediately when one of its children returns SUCCESS.
//If a child returns a FAILURE code, the selector continues examining its remaining children.
//If all of the selector's children return a FAILURE code, the selector returns a FAILURE code itself. 
class SelectorNode : public BehaviorTreeNode
{
public:

    virtual ~SelectorNode() {};

    void AddChild(std::shared_ptr<BehaviorTreeNode> child)
    {
        children.push_back(child);
    }

    bool IsEmpty() const { return children.empty(); }

    NodeStatus Run() override
    {
        for (auto& child : children) {
           
            NodeStatus status = child->TickNode(); 
            
            if (status == NodeStatus::Success)
            {
                return status;
            }
        }

        return NodeStatus::Failure;
    }

private:
    
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;

  
};


//The Sequence Node executes its child nodes sequentially, one after the other.
//If one of the nodes fails, the Sequence Node returns a failure stateand does not execute the remaining nodes.
//If a node succeeds, the Sequence Node continues to call the remaining nodes in sequence.
//If all nodes succeed, the Sequence Node returns a success state.
class SequenceNode : public BehaviorTreeNode
{
public:
    virtual ~SequenceNode() {};

    void AddChild(std::shared_ptr<BehaviorTreeNode> child)
    {
        children.push_back(child);
    }

    bool IsEmpty() const { return children.empty(); }

    NodeStatus Run() override
    {
        for (auto& child : children) {

            NodeStatus status = child->TickNode();

            if (status == NodeStatus::Failure)
            {
                return status;
            }
        }

        return NodeStatus::Success;
    }

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
};


//RandomUniformDistribution randomly selects one of its children to execute,
//with each child having an equal probability of being chosen.The selection
//is made using a uniform distribution.
class RandomUniformDistribution : public BehaviorTreeNode
{
public:
    RandomUniformDistribution(int numChildren) : m_distribution(0, numChildren - 1)
    {
        m_eng = std::default_random_engine(std::time(0));
    }

    virtual ~RandomUniformDistribution() {};

    void AddChild(std::shared_ptr<BehaviorTreeNode> child)
    {
        children.push_back(child);
    }

    bool IsEmpty() const { return children.empty(); }

    NodeStatus Run() override
    {

        int index = m_distribution(m_eng);
        std::shared_ptr<BehaviorTreeNode> child = children[index];

        return child->TickNode();
    }


private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    std::uniform_int_distribution<int> m_distribution;
    std::default_random_engine m_eng;

};


//WeightedRandomDistribution selects a child node to execute based on a weighted random distribution.
//The user is required to provide a vector of weights that will be used to initialize the discrete distribution.
//The sum of the weights will be checked to ensure it equals 1.0.Then, when children are added to the distribution, 
//the weights will be added to the m_weights vector, but a new discrete distribution will not be created since the 
//initialization was done in the constructor.
class WeightedRandomDistribution : public BehaviorTreeNode
{
public:
    //Constructor that takes a vector of weights to be used in the distribution.
    //The sum of the weights must be 1.0.
    WeightedRandomDistribution(std::vector<float> weights) :
        m_distribution(weights.begin(), weights.end())
    {
        m_eng = std::default_random_engine(std::time(0));

        // Check that the sum of the weights equals 1.0
        float sum = std::accumulate(weights.begin(), weights.end(), 0.0);
        assert(std::abs(sum - 1.0) < 1e-6 && "Weights do not sum up to 1.0");

    }

    virtual ~WeightedRandomDistribution() {};

    void AddChild(std::shared_ptr<BehaviorTreeNode> child, float weight)
    {
        children.push_back(child);
        m_weights.push_back(weight);
        // Update the distribution with the new weights
        m_distribution = std::discrete_distribution<int>(m_weights.begin(), m_weights.end());
    }

    bool IsEmpty() const { return children.empty(); }

    NodeStatus Run() override
    {
        // Check that the sum of the weights equals 1.0
        float sum = std::accumulate(m_weights.begin(), m_weights.end(), 0.0);
        assert(std::abs(sum - 1.0) < 1e-6 && "Weights do not sum up to 1.0");


        // Select a random child node based on the weights
        int index = m_distribution(m_eng);
        std::shared_ptr<BehaviorTreeNode> child = children[index];

        return child->TickNode();
    }

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    std::vector<float> m_weights;
    std::discrete_distribution<int> m_distribution;
    std::default_random_engine m_eng;
};

//A decorator, as a wrapped component, affects exactly one componentand modifies its processing logic
class DecoratorNode : public BehaviorTreeNode
{
public:

    virtual ~DecoratorNode() {}

    void SetChild(std::shared_ptr<BehaviorTreeNode> node)
    { 
        child = node;
    }

    bool HasChild() const { return child != nullptr; }

protected:
    std::shared_ptr<BehaviorTreeNode> child;
};


//An Inverter decorator reverses the outcome of the behavior tree node it is attached to.
//If the wrapped node returns SUCCESS, the inverter will return FAILURE.
//If the wrapped node returns FAILURE, the inverter will return SUCCESS.
class Inverter : public DecoratorNode
{
public:

    NodeStatus Run() override
    {
        NodeStatus status = child->TickNode();

        if (status == NodeStatus::Success) {
            return NodeStatus::Failure;
        }
        else if (status == NodeStatus::Failure) {
            return NodeStatus::Success;
        }
 
        return status;
    }

};


//The Failer decorator always returns a failure status code, no matter what the child component's result is.
class Failer : public DecoratorNode
{
public:
    NodeStatus Run() override
    {
        child->TickNode();
        return NodeStatus::Failure;
    }
};


//The Succeeder decorator always returns a success status code, regardless of the child component's result.
class Succeeder : public DecoratorNode
{
public:
    NodeStatus Run() override
    {
        child->TickNode();
        return NodeStatus::Success;
    }
};


//The Loop decorator repeatedly executes its child node until the child node returns a SUCCESS state, 
//either infinitely or up to a specified limit.
class Repeater : public DecoratorNode
{
public:
    Repeater(int num_iterations) : num_iterations(num_iterations) {}

    void ResetNode() override
    {
        current_iteration = 0;
    }

    NodeStatus Run() override
    {
        while (current_iteration < num_iterations) {
            child->TickNode();
            current_iteration++;
        }

        return NodeStatus::Success;
    }

protected:
    int num_iterations;
    int current_iteration = 0;
};


//RandomBernoulliDistribution represents a random Bernoulli distribution decorator node for use in behavior trees.
//It checks if it has a child node and randomly executes it based on a given probability.
class RandomBernoulliDistribution : public DecoratorNode
{
public:
    RandomBernoulliDistribution() : m_probability(0.5) 
    {
        m_eng = std::default_random_engine(std::time(0));
    }
    RandomBernoulliDistribution(float probablity) : m_probability(probablity) 
    {
        assert(m_probability >= 0.0 && m_probability <= 1.0 && "Probability must be between 0 and 1");
        m_eng = std::default_random_engine(std::time(0));
    }

    NodeStatus Run() override
    {
        assert(child && "RandomBernoulliDistribution: no child node");
        
        std::bernoulli_distribution bern_dist(m_probability);

        bool output = bern_dist(m_eng);

        if (output)
        {
            child->TickNode();
            return NodeStatus::Success;
        }
        else
        {
            return NodeStatus::Failure;
        }
            

    }

private:
    float m_probability;
    std::default_random_engine m_eng;
};

