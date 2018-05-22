/*
 * online_walking_pattern_generator.h
 *
 *  Created on: 2018. 2. 27.
 *      Author: Crowban
 */

#include "heroehs_online_walking_pattern_generator/online_endpoint_calculator.h"

namespace heroehs
{

class OnlineWalkingPatternGenerator
{
public:
  OnlineWalkingPatternGenerator();
  ~OnlineWalkingPatternGenerator();

  void initialize(double lipm_height_m, double preview_time_sec, double control_time_sec);

  void process();
  void start();
  void stop();

  bool isRunning();

  void addStepData(robotis_framework::StepData& step_data);
  void eraseLastStepData();
  int  getNumofRemainingUnreservedStepData();
  void getReferenceStepDatafotAddition(robotis_framework::StepData *ref_step_data_for_addition);

  void setInitialPose(robotis_framework::Pose3D r_foot, robotis_framework::Pose3D l_foot,
      robotis_framework::Pose3D pelvis);

  robotis_framework::Pose3D pose_g_to_pelvis_;
  Eigen::Matrix4d mat_pelvis_to_g_,  mat_g_to_pelvis_;
  Eigen::Matrix4d mat_robot_to_pelvis_, mat_pelvis_to_robot_;
  Eigen::Matrix4d mat_robot_to_g_, mat_g_to_robot_;

  Eigen::Matrix4d mat_g_to_rfoot_, mat_g_to_lfoot_;

  int current_balancing_index_;

private:
  OnlineEndpointCalculator ep_calculator_;

};

}
