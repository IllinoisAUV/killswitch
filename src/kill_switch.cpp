#include "kill_switch.h"

#include <stddef.h>
#include "ros/ros.h"
#include "std_msgs/Bool.h"

#include "gpio.h"




KillSwitch::KillSwitch(unsigned int pin) : pin_(pin), node_() {
  pin_.SetDirection(GPIO::IN);
  pin_.SetActiveState(GPIO::ACTIVE_HIGH);
}

KillSwitch::~KillSwitch() {}

void KillSwitch::publishMessage(GPIO::Edge value, ros::Publisher pub) {
  // Build the message
  std_msgs::Bool msg;
  msg.data = (value == GPIO::RISING ? true : false);

  pub.publish(msg);
}

void KillSwitch::Run() {
  ros::Publisher pub = node_.advertise<std_msgs::Bool>(KILL_SWITCH_TOPIC, 1);

  while(pub.getNumSubscribers() == 0);

  GPIO::LogicLevel value = pin_.GetValue();

  bool pressed = (value == GPIO::HIGH);
  publishMessage(pressed ? GPIO::RISING : GPIO::FALLING, pub);




  while(ros::ok()) {
    if(pressed) {
      pin_.WaitOn(GPIO::FALLING);
      pressed = false;
      publishMessage(GPIO::FALLING, pub);

    } else {
      pin_.WaitOn(GPIO::RISING);
      pressed = true;
      publishMessage(GPIO::RISING, pub);
    }
  }
}
