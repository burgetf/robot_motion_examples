/*
 *robot_motion_server_example.cpp
 *
 *  Created on: May 20, 2017
 *      Author: Felix Burget
 */


#include <ros/ros.h>

//Planning services
#include <planner_msgs/planning_scene_info.h>
#include <planner_msgs/set_edge_costs.h>
#include <planner_msgs/run_planner_configs.h>
#include <planner_msgs/run_planner_poses.h>

//Execution services
#include <execution_msgs/execute_trajectory_rviz.h>

//Plannig Worlds
#include <planning_world_builder/planning_world_builder.h>


using namespace std;

int main(int argc, char** argv)
{
    //Init Node
    ros::init(argc, argv, "robot_motion_server_example_node");

    //Node Handle
    ros::NodeHandle nh;

   
    //Set up planning service clients
    ros::ServiceClient client_set_planning_scene_info = nh.serviceClient<planner_msgs::planning_scene_info>("/planning_server/set_planning_scene_info");
    ros::ServiceClient client_set_edge_cost_weights = nh.serviceClient<planner_msgs::set_edge_costs>("/planning_server/set_planner_edge_costs");

    ros::ServiceClient client_run_planner_configs = nh.serviceClient<planner_msgs::run_planner_configs>("/planning_server/run_planner_start_goal_config");
    ros::ServiceClient client_run_planner_poses = nh.serviceClient<planner_msgs::run_planner_poses>("/planning_server/run_planner_poses");


    //Set up execution service clients
    ros::ServiceClient client_execute_rviz = nh.serviceClient<execution_msgs::execute_trajectory_rviz>("/execution_server/execute_trajectory_rviz");


    // ----------------- Planning --------------------------

    //Load Planning World
    planning_world::PlanningWorldBuilder world_builder("robot_description", "omnirob_base");
    //Insert Block Scenario
    vector<double> block_pos(3);
    block_pos[0] = 0.0;
    block_pos[1] = 0.0;
    block_pos[2] = 0.0;
    vector<double> block_dim(3);
    block_dim[0] = 5.0;
    block_dim[1] = 5.0;
    block_dim[2] = 2.0;
    world_builder.insertBlock(block_pos, block_dim);

    //Set planning scene info
    planner_msgs::planning_scene_info planning_info;
    planning_info.request.env_size_x.push_back(-10.0);
    planning_info.request.env_size_x.push_back(10.0);
    planning_info.request.env_size_y.push_back(-10.0);
    planning_info.request.env_size_y.push_back(10.0);
    client_set_planning_scene_info.call(planning_info);

    //Set edge cost weights
    planner_msgs::set_edge_costs edge_costs;
    edge_costs.request.edge_costs.push_back(1.0); //x
    edge_costs.request.edge_costs.push_back(1.0); //y
    edge_costs.request.edge_costs.push_back(1.0); //theta
    client_set_edge_cost_weights.call(edge_costs);


    //++++++++ RUN PLANNER CONFIGS ++++++++++++++

    //Run planner with start and goal config
    planner_msgs::run_planner_configs planner_run_1;
    planner_run_1.request.planner_type = "bi";
    planner_run_1.request.run_id = 0;
    //Set start configuration
    planner_run_1.request.start_config.push_back(-1.0);
    planner_run_1.request.start_config.push_back(0.0);
    planner_run_1.request.start_config.push_back(0.0);
    //Set goal configuration
    planner_run_1.request.goal_config.push_back(4.0);
    planner_run_1.request.goal_config.push_back(6.0);
    planner_run_1.request.goal_config.push_back(0.5);
    //Set max planning time
    planner_run_1.request.flag_iter_or_time = 1;
    planner_run_1.request.iter_or_time = 30; //in seconds
    //Set features
    planner_run_1.request.informed_sampling = true;
    planner_run_1.request.tree_optimization = true;
    //Show tree in RViz
    planner_run_1.request.show_tree = true;

    //Run planner
    //client_run_planner_configs.call(planner_run_1);


    // ----------------- Execution --------------------------
    execution_msgs::execute_trajectory_rviz execute_trajectory_1;
    execute_trajectory_1.request.planner_type = "bi_informed_rrt_star";
    execute_trajectory_1.request.run_id = "0";

    //Execute plan
    //client_execute_rviz.call(execute_trajectory_1);


    //++++++++ RUN PLANNER POSES ++++++++++++++

    //Run planner with start and goal config
    planner_msgs::run_planner_poses planner_run_2;
    planner_run_2.request.planner_type = "bi";
    planner_run_2.request.run_id = 1;
    //Set start configuration
    planner_run_2.request.start_ee_pose.push_back(1.0); //x
    planner_run_2.request.start_ee_pose.push_back(3.0); //y
    planner_run_2.request.start_ee_pose.push_back(0.0); //z
    planner_run_2.request.start_ee_pose.push_back(0.0); //roll
    planner_run_2.request.start_ee_pose.push_back(0.0); //pitch
    planner_run_2.request.start_ee_pose.push_back(0.0); //yaw
    //Set goal configuration
    planner_run_2.request.goal_ee_pose.push_back(-4.0); //x
    planner_run_2.request.goal_ee_pose.push_back(-2.0); //y
    planner_run_2.request.goal_ee_pose.push_back(0.0);  //z
    planner_run_2.request.goal_ee_pose.push_back(0.0); //roll
    planner_run_2.request.goal_ee_pose.push_back(0.0); //pitch
    planner_run_2.request.goal_ee_pose.push_back(0.0); //yaw
    //Set max planning time
    planner_run_2.request.flag_iter_or_time = 1;
    planner_run_2.request.iter_or_time = 30; //in seconds
    //Set features
    planner_run_2.request.informed_sampling = true;
    planner_run_2.request.tree_optimization = true;
    //Show tree in RViz
    planner_run_2.request.show_tree = true;

    //Run planner
    client_run_planner_poses.call(planner_run_2);

    // ----------------- Execution --------------------------
    execution_msgs::execute_trajectory_rviz execute_trajectory_2;
    execute_trajectory_2.request.planner_type = "bi_informed_rrt_star";
    execute_trajectory_2.request.run_id = "1";

    //Execute plan
    client_execute_rviz.call(execute_trajectory_2);


    //Shutdown node
    ros::shutdown();

    return 0;
}



