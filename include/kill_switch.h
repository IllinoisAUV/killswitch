#ifndef KILL_SWITCH_H
#define KILL_SWITCH_H
#include "ros/ros.h"

#include "gpio.h"

#define KILL_SWITCH_TOPIC "kill_switch"




class KillSwitch {
public:
  KillSwitch(unsigned int pin);
  ~KillSwitch();
  void Run();
private:
  void publishMessage(bool value, ros::Publisher pub);
  ros::NodeHandle node_;
  GPIO pin_;
};

#endif // KILL_SWITCH_H
