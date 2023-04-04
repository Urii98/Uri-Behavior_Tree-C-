# Urii-Behavior_Tree

The UriBehaviorTree class is an implementation of a Behavior Tree in C++. A Behavior Tree is a data structure that is used to model the behavior of an agent in a game, robot, or any autonomous system.

##Installation

To use the UriBehaviorTree class, simply include the "UriBehaviorTree.h" and "UriBehaviorTree.cpp" files in your C++ project, and make sure that C++11 or higher is being used.

##Conventions for using the behavior tree class

1.Create the behavior tree diagram.

2.Identify all node types, and keep in mind which ones are action nodes versus control flow nodes and condition nodes.

3.Declare all action nodes, starting from the deepest and ending with the most superficial. If there are any decorators, define them as well in the same order.

4.Declare all control flow and condition nodes, starting from the deepest and ending with the most superficial. This declaration will include adding any child nodes that they may have and giving each node a name that is related to what it does and allows it to be quickly identified with the node in the diagram that we created earlier. Additionally, if it is a conditional or switch node, assign a value to the bool that it incorporates.

5.Once we have absolutely all the nodes, we create our BehaviorTree object, set the root of this tree (which will be the most superficial node from which the rest of the nodes hang), and if we want to activate debugging, we do so at this step.

6.Run the tree to obtain the outcome of our behavior tree.

##Contribution

Feel free to send pull requests or report issues in the repository's issues section. You can also contact me via email if you have any questions or suggestions. 

oriol.martin.corella@estudiantat.upc.edu