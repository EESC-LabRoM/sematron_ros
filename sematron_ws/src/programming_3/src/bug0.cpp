#include <programming_3/bug0.h>
#include <programming_3/math.h>

double max_lin_vel = 0.4;
double min_lin_vel = 0.2;

double max_ang_vel = 0.6;
double min_ang_vel = 0.4;

double sonar_max_value = 0.5;
double min_distance_to_goal = 0.1;
double min_distance_to_obstacle = 0.5;

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
    geometry_msgs::Point pos_lin = this->odometry.pose.pose.position;
    geometry_msgs::Quaternion pos_ang = this->odometry.pose.pose.orientation;

    double distanceToGoal = math::distance2D(this->odometry.pose.pose.position, this->goal);

    double yaw = tf::getYaw(pos_ang);
    double theta = std::atan2(goal.y - pos_lin.y, goal.x - pos_lin.x);
    double delta = theta - yaw;

    double gain_ang_vel = 0.2;
    double ang_vel = 0;

    double lin_vel = 0;

    double factor = 0;

    double n_delta = math::normalizeAngle(delta);
    double abs_n_delta = std::fabs(n_delta);
    int mtplr = delta < 0 ? -1 : 1;

    ang_vel = mtplr * std::max(min_ang_vel, std::min(max_ang_vel, gain_ang_vel * abs_n_delta));

    if (abs_n_delta > M_PI / 16)
    {
        // ROS_INFO("fix yaw");
        lin_vel = 0;
    }
    else
    {
        // ROS_INFO("go straight ahead");
        lin_vel = max_lin_vel;
        // Small-factor obstacle avoidance for goToPoint task
        factor = (this->sonarArray[RIGHT_SONAR] - this->sonarArray[LEFT_SONAR]);
        ang_vel = factor * max_ang_vel;
    }

    this->twist.linear.x = lin_vel;
    if (distanceToGoal < 0.5)
    {
        this->twist.linear.x = min_lin_vel;
    }
    this->twist.angular.z = ang_vel;
}

/**
* Wall Following function: Computes desired twist that allows robot circum-navagiating a wall/obstacle.
*/
void Bug0::wallFollower(void)
{
    // obstacle in front of the robot
    if (this->sonarArray[FRONT_SONAR] != 0)
    {
        // Too close - get back
        if (this->sonarArray[FRONT_SONAR] < 0.2)
        {
            ROS_INFO("get back");
            this->twist.angular.z = 0;
            this->twist.linear.x = -max_lin_vel / 2;
        }
        // turn left
        else
        {
            ROS_INFO("front");
            this->twist.angular.z = max_ang_vel * ((sonar_max_value - this->sonarArray[FRONT_SONAR]) / sonar_max_value);
            this->twist.linear.x = (this->sonarArray[FRONT_SONAR] - sonar_max_value / 5) * max_lin_vel;
        }
    }
    else if (this->sonarArray[FRONT_SONAR] == 0 && this->sonarArray[RIGHT_SONAR] > 0)
    {
        ROS_INFO("right");
        this->twist.linear.x = max_lin_vel * 0.8;
    }
    else
    {
        // go straight ahead (obstacle must be on robot's right side)
        ROS_INFO("else");
        this->twist.linear.x = max_lin_vel;
    }
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

    switch (state)
    {
    // State 0: Nothing to do.
    case 0:
        if (distanceToGoal > 0.1)
        {
            ROS_INFO("State 1");
            state = 1;
        }
        break;

    // State 1: Obstacle free, pursue the goal!
    case 1:
        this->goToPoint();

        if (distanceToGoal < min_distance_to_goal)
        {
            this->twist.linear.x = 0;
            this->twist.angular.z = 0;
            ROS_INFO("State 0");
            state = 0;
        }
        else if (this->sonarArray[FRONT_SONAR] > 0 && this->sonarArray[FRONT_SONAR] < min_distance_to_obstacle)
        {
            this->twist.linear.x = 0;
            this->twist.angular.z = 0;
            ROS_INFO("State 2");
            state = 2;
        }
        break;

    // State 2: Take the robot back to the closest point in respect to final goal
    case 2:
        this->wallFollower();

        // No more obstacles
        if (this->sonarArray[FRONT_SONAR] == 0 &&
            this->sonarArray[LEFT_SONAR] == 0 &&
            this->sonarArray[RIGHT_SONAR] == 0)
        {
            // Back to "goToPoint" task
            ROS_INFO("State 1");
            state = 1;
        }

        break;
    }
}

} // closing bug namespace