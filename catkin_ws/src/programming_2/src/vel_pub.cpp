#include <stdio.h>


/** include ros libraries**********************/
#include <std_msgs/Float64.h>
#include <ros/ros.h>

/**
 *  This Node get two velocities from the user and publish it to the simulator
 */

int main(int argc, char **argv)
{

    ros::init(argc, argv, "vel_publisher");

    ros::NodeHandle n;

    //  One publisher for each motor
    ros::Publisher pub_1 = n.advertise< std_msgs::Float64 >( "/vrep/vehicle/motorLeftSpeed", 1);
    ros::Publisher pub_2 = n.advertise< std_msgs::Float64 >( "/vrep/vehicle/motorRightSpeed", 1);

    ros::Rate loop_rate(10);

    while( ros::ok() ){

        double vel_1,vel_2;
        std_msgs::Float64 msg_1;
        std_msgs::Float64 msg_2;

        // Get both velocities
        ROS_INFO( "Two Velocities: ");
        scanf("%lf %lf", &vel_1, &vel_2);

        // Convert it
        msg_1.data = vel_1;
        msg_2.data = vel_2;

        // Publish
        pub_1.publish(msg_1);
        pub_2.publish(msg_2);

        ros::spinOnce();

        loop_rate.sleep();

        }
}
