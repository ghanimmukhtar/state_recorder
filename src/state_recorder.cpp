#include <ros/ros.h>
#include <vector>
#include <iostream>
#include "rosbag/bag.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/JointState.h"
#include <image_transport/image_transport.h>


rosbag::Bag bag;
rosbag::Bag bag_2;
ros::Time my_time_1, my_time_2;

void save_image_CB(const sensor_msgs::ImageConstPtr& my_image ){
    my_time_1 = ros::Time::now();
    std::cout << "callback" << std::endl;
    bag.write("/cameras/head_camera/image", my_time_1, my_image);
    //std::cout << "image time stamp for now is: " << my_time.toSec() << std::endl;
}

void save_joint_states_CB(sensor_msgs::JointState my_state ){
    my_time_2 = ros::Time::now();
    bag_2.write("/robot/joint_states", my_time_2, my_state);
    //std::cout << "joint time stamp for now is: " << my_time.toSec() << std::endl;
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "bag_recorder");
  ros::NodeHandle node;
  bag.open("image.bag", rosbag::bagmode::Write);
  bag_2.open("joint.bag", rosbag::bagmode::Write);
  ros::Rate rate(15.0);
/*
  ros::Subscriber save_image = node.subscribe<sensor_msgs::Image>("/cameras/head_camera/image",1,save_image_CB);
  ros::Subscriber save_joint = node.subscribe<sensor_msgs::JointState>("/robot/joint_states",1,save_joint_states_CB);
*/

  image_transport::ImageTransport it(node);

  // fonctionne mais enregistre les images brutes
  ros::param::set("~image_transport", "compressed");
  std::string s;
  ros::param::get("~image_transport", s);
  std::cout << s << std::endl;
  image_transport::Subscriber sub1 = it.subscribe("/cameras/head_camera/image", 1, save_image_CB);
  

  while (ros::ok())
  {
      ros::spinOnce();
      rate.sleep();
      //std::cout << "difference is: " << my_time_1.toSec() - my_time_2.toSec() << std::endl;
  }
  bag.close();
  bag_2.close();
  return 0;
}
