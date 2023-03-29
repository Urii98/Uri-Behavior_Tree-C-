#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
                //Reference: Nodo que apunta a otro BehaviorTree y que devuelve el resultado del status de ese arbol
//Action/Base
//Condition  
//Control Flow
    //Selector
    //Sequence
                //Decorator
                    //Inverter
                    //Succeeder
                    //Failer
                    //Repeater
                    //Count.based limit decorator
                    //Time.based limit decorator

//los update deberian estar en privado en vez de en public?

enum class NodeStatus
{
	Default,
	Success,
	Failure,
	Running,
    Failure
};

//Base node and action node
class BehaviorTreeNode 
{
public:

	virtual ~BehaviorTreeNode() {};
	virtual NodeStatus run() = 0;
	//virtual void resetNode() {}

    NodeStatus tickNode()
    {
      /*  if (currentStatus != NodeStatus::Running) {
            resetNode();
        }*/

        currentStatus = run();

        return currentStatus;
    }
    
    NodeStatus GetStatus() const { return currentStatus; }
    
protected:
    NodeStatus currentStatus = NodeStatus::Default;

};

//The Condition Node represents a Boolean test that can evaluate to either true or false
//If the test evaluates to true, the node returns a SUCCESS status code, and if it evaluates 
//to false, it returns a FAILURE code.
class ConditionNode : public BehaviorTreeNode
{
public:

    virtual ~ConditionNode() {}

    NodeStatus run() override
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

    void addChildren(std::shared_ptr<BehaviorTreeNode> child)
    {
        children.push_back(child);
    }

    bool isEmpty() const { return children.empty(); }

    NodeStatus run() override
    {
        for (auto& child : children) {
           
            NodeStatus status = child->tickNode(); 
            
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

    void addChildren(std::shared_ptr<BehaviorTreeNode> child)
    {
        children.push_back(child);
    }

    bool isEmpty() const { return children.empty(); }

    NodeStatus run() override
    {
        for (auto& child : children) {

            NodeStatus status = child->tickNode();

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