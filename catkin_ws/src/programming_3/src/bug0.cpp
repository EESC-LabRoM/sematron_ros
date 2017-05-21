#include <programming_3/bug0.h>
#include <programming_3/math.h>

namespace bug
{
/**
    *  Empty constructor
    */
Bug0::Bug0()
{
    // Reserve memory space for 3 sonars.
    for (int i = 0; i < 2; i++)
    {
        sonarArray.push_back(0);
    }
}

/**
    *  Empty destructor
    */
Bug0::~Bug0() { ; }

/**
    * Go to Point: Computes desired twist that takes the robot towards the goal point. 
    */
void Bug0::goToPoint(void)
{
    // Go to point. Given the point specified in the this->goal, compute the necessary twist to reach it. HEre, you must use this->odometry.

    //....
}

/**
    * Wall Following function: Computes desired twist that allows robot circum-navagiating a wall/obstacle.
    */
void Bug0::wallFollower(void)
{
    // Wall follower routine. Computes this->twist that enables the robot following an obstacle continuously. Try to use only local measurements, e.g. sonar sensors. No need to use odometry.

    //....
}

/**
    * Bug Manager: Decides which sub-routine shall be called.
    */
void Bug0::bugManager(void)
{
    // Static variables hold values
    static int state = 0;
    static double shortestDistanceToGoal = 0;

    // Compute current distance in respect to the final goal
    double distanceToGoal = math::distance2D(this->odometry.pose.pose.position, this->goal);
    // Compute current distance in respect to last hit point
    double distanceToH_in = math::distance2D(this->odometry.pose.pose.position, this->h_in);
    // Compute current distance in respect to last leaving point
    double distanceToH_out = math::distance2D(this->odometry.pose.pose.position, this->h_out);

    std::cout << "state: " << state << std::endl;
    std::cout << "distance to goal: " << distanceToGoal << std::endl;
    std::cout << "distance to hin: " << distanceToH_in << std::endl;
    std::cout << "distance to hout: " << distanceToH_out << std::endl;

    switch (state)
    {
    // State 0: Nothing to do.
    case 0:
        if (distanceToGoal > 0.1)
        { // Robot is far from the goal
            // Change to "Go to point" state.
            state = 1;
        }
        break;

    // State 1: Obstacle free, pursue the goal!
    case 1: // Move toward the goal.
        this->goToPoint();
        // Did the robot reach the goal?
        if (distanceToGoal < 0.05)
        {
            // Change to "resting" state.
            this->twist.linear.x = 0;
            this->twist.angular.z = 0;
            state = 0;
        }
        else
            // Did the robot detected an obstacle in front of it?
            if (this->sonarArray[FRONT_SONAR] > 0 && this->sonarArray[FRONT_SONAR] < 0.5)
        {
            // Save hit IN point.
            h_in = odometry.pose.pose.position;
            // Change to "obstacle detected" state.
            state = 2;
        }
        break;

    // State 2: The robot has just detected an obstacle.
    case 2: // Follow the wall.
        this->wallFollower();
        // Remain in this state until robot is far enough from hit point.
        if (distanceToH_in > 0.5)
        {
            // So far, this is shortest distance to goal.
            shortestDistanceToGoal = distanceToGoal;
            // Change "circum-navigate" state.
            state = 3;
        }
        break;

    // State 3: The robot must circum-navigate the obstacle.
    case 3: // Follow the wall.
        this->wallFollower();
        // Is robot closer to the goal than ever before?
        if (distanceToGoal < shortestDistanceToGoal)
        {
            // Yes! Then save current position.
            this->h_out = this->odometry.pose.pose.position;
            shortestDistanceToGoal = distanceToGoal;
        }
        // Remain in this state until robot is back to initial hit point (hit IN).
        if (distanceToH_in < 0.4)
        {
            // Change "back to closest point" state.
            state = 4;
        }
        break;

    // State 4: Take the robot back to the closest point in respect to final goal
    case 4: // Follow the wall.
        this->wallFollower();
        // Remain in this state until robot is back to leaving point (hit OUT).
        if (distanceToH_out < 0.1)
        {
            // change state.
            state = 0;
        }
        break;
    }
}

} // closing bug namespace
