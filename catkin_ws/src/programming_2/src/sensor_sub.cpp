
/** include ros libraries**********************/
#include "ros/ros.h"
#include "std_msgs/Float32.h"

//*** Callback function to print the distance
void chatterCallback(const std_msgs::Float32 msg)
{
  ROS_INFO("Distance: [%f]", msg.data);
}

int main(int argc, char **argv)
{

// Initialize ros and node
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

// Subscribing the Sensor topic
  ros::Subscriber sub = n.subscribe("/vrep/vehicle/frontSonar", 1000, chatterCallback);

//*** Forth: remember to spin the code
  ros::spin();

  return 0;
}
