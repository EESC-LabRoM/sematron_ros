#include <stdio.h>


/** include ros libraries**********************/
#include <std_msgs/String.h>
#include <ros/ros.h>



int main(int argc, char **argv)
{

//***  First: Initialize ROS giving it argc, argv and a Name ***
    ros::init(argc, argv, "publisher");

//*** Second: Create a Node  ***
    ros::NodeHandle n;

//*** Third: Create a Publisher inside that Node, the publisher is defined by a type, a name and the size of the buffer
    ros::Publisher pub = n.advertise< std_msgs::String >( "chatter", 1000);

    ros::Rate loop_rate(10);

    int count = 0;

    while( ros::ok() ){

        //*** Forth: Create a message and put in it some content ***
        std_msgs::String msg;

        std::stringstream ss;
        ss << "Hello World - " << count ;

        msg.data = ss.str();

        ROS_INFO( "%s", msg.data.c_str());

        //*** Fifth: publish the message
        pub.publish(msg);

        ros::spinOnce();

        loop_rate.sleep();

        ++count;
    }

}
