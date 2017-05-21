
/** include ros libraries**********************/
#include "ros/ros.h"
#include "std_msgs/String.h"

//*** Create one callback function
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
}

int main(int argc, char **argv)
{

//***  First: Initialize ROS giving it argc, argv and a Name ***


//*** Second: Create a Node  ***



//*** Third: Create a Subscriber inside that Node, it is defined by a topic name, the size of the buffer  and the CallBack function


//*** Fourth: make a ROS_INFO


//*** Fourth: remember to spin the code


  return 0;
}
