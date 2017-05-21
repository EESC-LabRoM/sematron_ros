
/** include ros libraries**********************/
#include "ros/ros.h"
#include "std_msgs/String.h"

//*** Create one callback function with ROS_INFO
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  //***  First: Initialize ROS giving it argc, argv and a Name ***
  ros::init(argc, argv, "listener");

  //*** Second: Create a Node  ***
  ros::NodeHandle n;

  //*** Third: create a Subscriber inside that Node, it is defined by a topic name, the size of the buffer  and the CallBack function
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  //*** Fourth: remember to spin the code
  ros::spin();

  return 0;
}
