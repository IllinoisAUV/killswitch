#include "kill_switch.h"

#include "ros/ros.h"
#include "ros/console.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


const int pin = 186;


int main(int argc, char *argv[]) {
  ros::init(argc, argv, "kill_switch", ros::init_options::NoSigintHandler);

  ROS_INFO("Starting kill switch node");

  /* if(argc != 2) { */
  /*   printf("Please specify pin number\n"); */
  /*   exit(-1); */
  /* } */
  /* int pin = atoi(argv[1]); */


  KillSwitch ks(pin);
  ks.Run();
}
