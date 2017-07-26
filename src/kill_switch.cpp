#include "kill_switch.h"

#include <stddef.h>
#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/Bool.h"

#include "gpio.h"


const bool kReverse = true;

KillSwitch::KillSwitch(unsigned int pin) : pin_(pin), node_() {
  pin_.SetDirection(GPIO::IN);
  pin_.SetActiveState(GPIO::ACTIVE_LOW);
}

KillSwitch::~KillSwitch() {}

void KillSwitch::publishMessage(bool value, ros::Publisher pub) {
  // Build the message
  std_msgs::Bool msg;
  msg.data = value;

  ROS_INFO("Kill Switch: %s", msg.data ? "pressed" : "released");
  pub.publish(msg);
}

void KillSwitch::Run() {
  ros::Publisher pub = node_.advertise<std_msgs::Bool>(KILL_SWITCH_TOPIC, 1);

  while(pub.getNumSubscribers() == 0);

  GPIO::LogicLevel value = pin_.GetValue();

  bool pressed = (value == GPIO::HIGH);
  bool last_pressed = pressed;
  publishMessage(pressed, pub);

  ros::Rate rate(10);
  while(ros::ok()) {
      rate.sleep();
      value = pin_.GetValue();
      pressed = (value == GPIO::HIGH);
      if (pressed != last_pressed) {
          // Transition
          publishMessage(pressed, pub);
      }
      last_pressed = pressed;
      ros::spinOnce();
  }
}
