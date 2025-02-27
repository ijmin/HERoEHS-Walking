/*
 * online_walking_pattern_generator.cpp
 *
 *  Created on: 2018. 2. 27.
 *      Author: Crowban
 */


#include "heroehs_online_walking_pattern_generator/online_walking_pattern_generator.h"

using namespace heroehs;

OnlineWalkingPatternGenerator::OnlineWalkingPatternGenerator()
{
  current_balancing_index_ = 0;
  switching_ratio_ = 0;

  reference_foot_left_x_  = 0; reference_foot_left_y_  = 0; reference_foot_left_z_ = 0;
  reference_foot_right_x_ = 0; reference_foot_right_y_ = 0; reference_foot_right_z_ = 0;
}

OnlineWalkingPatternGenerator::~OnlineWalkingPatternGenerator()
{ }

void OnlineWalkingPatternGenerator::initialize(double lipm_height_m, double preview_time_sec, double control_time_sec)
{
  x_lipm_.fill(0.0);       y_lipm_.fill(0.0);
  ep_calculator_.initialize(lipm_height_m, preview_time_sec, control_time_sec);
  process();
}

void OnlineWalkingPatternGenerator::process()
{
  ep_calculator_.calcDesiredPose();


// yitaek
  reference_zmp_x_ = ep_calculator_.reference_zmp_x_.coeff(0);
  reference_zmp_y_ = ep_calculator_.reference_zmp_y_.coeff(0);

  switching_ratio_ = ep_calculator_.switching_ratio_;

  current_balancing_index_ = ep_calculator_.current_balancing_index_;

  x_lipm_ = ep_calculator_.x_lipm_;
  y_lipm_ = ep_calculator_.y_lipm_;

// yitaek
  reference_body_x_ = x_lipm_.coeff(0);
  reference_body_y_ = y_lipm_.coeff(0);

  reference_foot_left_x_  = ep_calculator_.present_left_foot_pose_.x;
  reference_foot_left_y_  = ep_calculator_.present_left_foot_pose_.y;
  reference_foot_left_z_  = ep_calculator_.present_left_foot_pose_.z;
  reference_foot_right_x_ = ep_calculator_.present_right_foot_pose_.x;
  reference_foot_right_y_ = ep_calculator_.present_right_foot_pose_.y;
  reference_foot_right_z_ = ep_calculator_.present_right_foot_pose_.z;


//

  mat_g_to_rfoot_  = robotis_framework::getTransformationXYZRPY(ep_calculator_.present_right_foot_pose_.x, ep_calculator_.present_right_foot_pose_.y, ep_calculator_.present_right_foot_pose_.z,
      ep_calculator_.present_right_foot_pose_.roll, ep_calculator_.present_right_foot_pose_.pitch, ep_calculator_.present_right_foot_pose_.yaw);
  mat_g_to_lfoot_  = robotis_framework::getTransformationXYZRPY(ep_calculator_.present_left_foot_pose_.x, ep_calculator_.present_left_foot_pose_.y, ep_calculator_.present_left_foot_pose_.z,
      ep_calculator_.present_left_foot_pose_.roll, ep_calculator_.present_left_foot_pose_.pitch, ep_calculator_.present_left_foot_pose_.yaw);
  mat_g_to_pelvis_ = robotis_framework::getTransformationXYZRPY(ep_calculator_.present_body_pose_.x, ep_calculator_.present_body_pose_.y, ep_calculator_.present_body_pose_.z,
      ep_calculator_.present_body_pose_.roll, ep_calculator_.present_body_pose_.pitch, ep_calculator_.present_body_pose_.yaw);

  pose_g_to_pelvis_ = ep_calculator_.present_body_pose_;
  mat_pelvis_to_g_ = robotis_framework::getInverseTransformation(mat_g_to_pelvis_);

  mat_robot_to_pelvis_ = robotis_framework::getRotation4d(ep_calculator_.present_body_pose_.roll, ep_calculator_.present_body_pose_.pitch, 0);
  mat_pelvis_to_robot_ = robotis_framework::getInverseTransformation(mat_robot_to_pelvis_);

  mat_g_to_robot_ = mat_g_to_pelvis_* mat_pelvis_to_robot_;
  mat_robot_to_g_ = robotis_framework::getInverseTransformation(mat_g_to_robot_);
}

void OnlineWalkingPatternGenerator::start()//ss
{
  ep_calculator_.start();
}

void OnlineWalkingPatternGenerator::stop()
{

}

bool OnlineWalkingPatternGenerator::isRunning()
{
  return ep_calculator_.isRunning();
}

void OnlineWalkingPatternGenerator::addStepData(robotis_framework::StepData& step_data)
{
  ep_calculator_.addStepData(step_data);
}

void OnlineWalkingPatternGenerator::eraseLastStepData()
{
  ep_calculator_.eraseLastStepData();
}

int  OnlineWalkingPatternGenerator::getNumofRemainingUnreservedStepData()
{
  return ep_calculator_.getNumofRemainingUnreservedStepData();
}

void OnlineWalkingPatternGenerator::getReferenceStepDatafotAddition(robotis_framework::StepData *ref_step_data_for_addition)
{
  ep_calculator_.getReferenceStepDatafotAddition(ref_step_data_for_addition);
}

void OnlineWalkingPatternGenerator::setInitialPose(robotis_framework::Pose3D r_foot, robotis_framework::Pose3D l_foot, robotis_framework::Pose3D pelvis)
{
  ep_calculator_.setInitialPose(r_foot, l_foot, pelvis);
  mat_g_to_rfoot_  = robotis_framework::getTransformationXYZRPY(r_foot.x, r_foot.y, r_foot.z, r_foot.roll, r_foot.pitch, r_foot.yaw);
  mat_g_to_lfoot_  = robotis_framework::getTransformationXYZRPY(l_foot.x, l_foot.y, l_foot.z, l_foot.roll, l_foot.pitch, l_foot.yaw);
  mat_g_to_pelvis_ = robotis_framework::getTransformationXYZRPY(pelvis.x, pelvis.y, pelvis.z, pelvis.roll, pelvis.pitch, pelvis.yaw);

  mat_pelvis_to_g_ = robotis_framework::getInverseTransformation(mat_g_to_pelvis_);

  mat_robot_to_pelvis_ = robotis_framework::getRotation4d(pelvis.roll, pelvis.pitch, 0);
  mat_pelvis_to_robot_ = robotis_framework::getInverseTransformation(mat_robot_to_pelvis_);

  mat_g_to_robot_ = mat_g_to_pelvis_* mat_pelvis_to_robot_;
  mat_robot_to_g_ = robotis_framework::getInverseTransformation(mat_g_to_robot_);
}


