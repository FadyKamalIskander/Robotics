#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

float posx = 0.0F;
float posy = 0.0F;

void amclCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& amclMsg)
{
  posx = amclMsg->pose.pose.position.x;
  ROS_INFO("amclMsg X: [%f]", amclMsg->pose.pose.position.x);
  posy= amclMsg->pose.pose.position.y;
  ROS_INFO("amclMsg Y: [%f]", amclMsg->pose.pose.position.y);
}

//simple Robot class to update status and properties
class Robot
{
  
public:
    enum RobotStatus
    {
      INIT,
      AT_PICKUP,
      TOWARDS_DROPOFF,
      AT_DROPOFF
    };
  
    RobotStatus status;
    float pickUpPosx;
    float pickUpPosy;
    float dropOffPosx;
    float dropOffPosy;
    Robot(): status(INIT), pickUpPosx(-1.0F), pickUpPosy(0.0F), dropOffPosx(5.0F), dropOffPosy(0.0F)
    {
    }
    void updateRobotStatus()
    {
        if ((status == INIT) && (std::abs(pickUpPosx - posx) > 0.15F) && (std::abs(pickUpPosy - posy) > 0.15F) )
        {
            status = INIT;
        }
        else if ((status == INIT) && (std::abs(pickUpPosx - posx) < 0.15F) && (std::abs(pickUpPosy - posy) < 0.15F) )
        {
            status = AT_PICKUP;
        }
        else if ((status == AT_PICKUP) && (std::abs(pickUpPosx - posx) < 0.15F) && (std::abs(pickUpPosy - posy) < 0.15F))
        {
            status = AT_PICKUP;
        }
        else if ((status == AT_PICKUP) && (std::sqrt((pickUpPosx - posx)*(pickUpPosx - posx) + (pickUpPosy - posy)*(pickUpPosy - posy)) > 0.15F) &&                      (std::sqrt((dropOffPosx - posx)*(dropOffPosx- posx) + (dropOffPosy - posy)*(dropOffPosy - posy)) > 0.15F)) 
        {
            status = TOWARDS_DROPOFF;
        }
        else if ((status == TOWARDS_DROPOFF) && (std::sqrt((pickUpPosx - posx)*(pickUpPosx - posx) + (pickUpPosy - posy)*(pickUpPosy - posy)) > 0.15F)           && (std::sqrt((dropOffPosx - posx)*(dropOffPosx- posx) + (dropOffPosy - posy)*(dropOffPosy - posy)) < 0.15F)) 
        {
            status = AT_DROPOFF;
        }
        else
        {
        }
    }
};

int main( int argc, char** argv )
{
    ros::init(argc, argv, "add_markers_amcl");
    ros::NodeHandle n;
    ros::Rate r(1);
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    ros::Subscriber amclSub = n.subscribe("/amcl_pose", 10, amclCallback);

    // Set our initial shape type to be a cube
    uint32_t shape = visualization_msgs::Marker::CUBE;

    // create Robot instance
    Robot mRobot;

    while (ros::ok())
    {
        visualization_msgs::Marker marker;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time::now();

        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker.ns = "add_markers_amcl";
        marker.id = 0;

        // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
        marker.type = shape;

        //update Robot status
        mRobot.updateRobotStatus();
		ROS_INFO("Current status [%d]", mRobot.status);
        if (mRobot.status == 0)
        {
            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::ADD;
            // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
            marker.pose.position.x = -1.0;
            marker.pose.position.y = 0;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;

            // Set the scale of the marker -- 1x1x1 here means 1m on a side
            marker.scale.x = 0.4;
            marker.scale.y = 0.4;
            marker.scale.z = 0.4;

            // Set the color -- be sure to set alpha to something non-zero!
            marker.color.r = 1.0f;
            marker.color.g = 0.0f;
            marker.color.b = 0.0f;
            marker.color.a = 1.0;

            marker.lifetime = ros::Duration();

            // Publish the marker
            while (marker_pub.getNumSubscribers() < 1)
            {
                if (!ros::ok())
                {
                    return 0;
                }
                ROS_WARN_ONCE("Please create a subscriber to the marker");
                sleep(1);
            }
            marker_pub.publish(marker);
            
        }
        else if (mRobot.status == 1)
        {
            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::DELETE;
            while (marker_pub.getNumSubscribers() < 1)
            {
                if (!ros::ok())
                {
                    return 0;
                }
                ROS_WARN_ONCE("Please create a subscriber to the marker");
                sleep(1);
            }
            marker_pub.publish(marker);
        }
        else if (mRobot.status == 2)
        {}
        else if (mRobot.status == 3)
        {
            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::ADD;

            // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
            marker.pose.position.x = 5.0;
            marker.pose.position.y = 0;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;

            // Set the scale of the marker -- 1x1x1 here means 1m on a side
            marker.scale.x = 0.4;
            marker.scale.y = 0.4;
            marker.scale.z = 0.4;

            // Set the color -- be sure to set alpha to something non-zero!
            marker.color.r = 1.0f;
            marker.color.g = 0.0f;
            marker.color.b = 0.0f;
            marker.color.a = 1.0;

            // Publish the marker
            while (marker_pub.getNumSubscribers() < 1)
            {
                if (!ros::ok())
                {
                    return 0;
                }
                ROS_WARN_ONCE("Please create a subscriber to the marker");
                sleep(1);
            }
            marker_pub.publish(marker);
        }
        ros::spinOnce();
        r.sleep();
    }
}