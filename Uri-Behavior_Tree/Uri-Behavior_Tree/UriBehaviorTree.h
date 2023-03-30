#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//BehaviorTree
//Action/Base
//Condition  
//Control Flow:
    //Selector
    //Sequence           
//Decorator:
    //Inverter
    //Succeeder
    //Failer
    //Repeater


enum class NodeStatus
{
	Default,
	Success,
	Failure,
	Running
};

//Base node and action node
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

    }


public:
    bool test = false;
    //std::shared_ptr<BehaviorTreeNode> leftChildren; //trueChildren
    //std::shared_ptr<BehaviorTreeNode> rightChildren; //falseChildren
};


//A Selector Noode examines its child components in sequence from left to right, 
//returning a SUCCESS code immediately when one of its children returns SUCCESS.
//If a child returns a FAILURE code, the selector continues examining its remaining children.
//If all of the selector's children return a FAILURE code, the selector returns a FAILURE code itself. 
class SelectorNode : public BehaviorTreeNode
{
public:

    virtual ~SelectorNode() {};

    void AddChildren(std::shared_ptr<BehaviorTreeNode> child)
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
    Repeater(int limit = 0) : limit(limit) {}

    void ResetNode() override
    {
        counter = 0;
    }

    NodeStatus Run() override
    {
        child->TickNode();

        if (limit > 0 && ++counter == limit) {
            return NodeStatus::Success;
        }

        return NodeStatus::Running;
    }

protected:
    int limit;
    int counter = 0;
};

