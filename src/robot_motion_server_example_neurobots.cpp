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

//Planning with configs/poses given in map frame
#include <planner_msgs/run_planner_map_goal_config.h>
#include <planner_msgs/run_planner_map_goal_pose.h>

//Planning with configs/poses given in base_link frame
#include <planner_msgs/run_planner_configs.h>

//Execution services
#include <execution_msgs/execute_trajectory_rviz.h>


using namespace std;

int main(int argc, char** argv)
{
    //Init Node
    ros::init(argc, argv, "robot_motion_server_example_neurobots_node");

    //Node Handle
    ros::NodeHandle nh;

   
    //Set up planning service clients
    ros::ServiceClient client_set_planning_scene_info = nh.serviceClient<planner_msgs::planning_scene_info>("/planning_server/set_planning_scene_info");
    ros::ServiceClient client_set_edge_cost_weights = nh.serviceClient<planner_msgs::set_edge_costs>("/planning_server/set_planner_edge_costs");

    //Planning with configs/poses given w.r.t the current base_link frame
    ros::ServiceClient client_run_planner_configs = nh.serviceClient<planner_msgs::run_planner_configs>("/planning_server/run_planner_start_goal_config");

    //Planning with configs/poses given w.r.t map frame
    ros::ServiceClient client_run_planner_map_configs = nh.serviceClient<planner_msgs::run_planner_map_goal_config>("/planning_server/run_planner_map_goal_config");
    ros::ServiceClient client_run_planner_map_poses = nh.serviceClient<planner_msgs::run_planner_map_goal_pose>("/planning_server/run_planner_map_goal_pose");


    //Set up execution service clients
    ros::ServiceClient client_execute_rviz = nh.serviceClient<execution_msgs::execute_trajectory_rviz>("/execution_server/execute_trajectory_rviz");


    // ----------------- Planning --------------------------

    //Set planning scene info
    planner_msgs::planning_scene_info planning_info;
    planning_info.request.env_size_x.push_back(-4.0);
    planning_info.request.env_size_x.push_back(1.0);
    planning_info.request.env_size_y.push_back(-1.0);
    planning_info.request.env_size_y.push_back(4.0);
    client_set_planning_scene_info.call(planning_info);

    //Set edge cost weights
    planner_msgs::set_edge_costs edge_costs;
    edge_costs.request.edge_costs.push_back(1.0); //x
    edge_costs.request.edge_costs.push_back(1.0); //y
    edge_costs.request.edge_costs.push_back(3.0); //theta
    client_set_edge_cost_weights.call(edge_costs);


    //++++++++ RUN PLANNER WITH CONFIGS/POSES GIVEN IN MAP FRAME ++++++++++++++

    //Run planner with start and goal config
    planner_msgs::run_planner_map_goal_config planner_run_1;
    planner_run_1.request.planner_type = "bi";
    planner_run_1.request.run_id = 0;
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
    //client_run_planner_map_configs.call(planner_run_1);


    // --- Execution ---
    execution_msgs::execute_trajectory_rviz execute_trajectory_1;
    execute_trajectory_1.request.planner_type = "bi_informed_rrt_star";
    execute_trajectory_1.request.run_id = "0";

    //Execute plan
    //client_execute_rviz.call(execute_trajectory_1);


    //++++++++ RUN PLANNER POSES ++++++++++++++

    //Run planner with start and goal config
    planner_msgs::run_planner_map_goal_pose planner_run_2;
    planner_run_2.request.planner_type = "bi";
    planner_run_2.request.run_id = 1;
    //Set goal configuration
    planner_run_2.request.goal_pose.position.x = -3.0; //x
    planner_run_2.request.goal_pose.position.y = 2.5;  //y
    planner_run_2.request.goal_pose.position.z = 0.0;  //z
    planner_run_2.request.goal_pose.orientation.x = 0.0; //q.x
    planner_run_2.request.goal_pose.orientation.y = 0.0; //q.y
    planner_run_2.request.goal_pose.orientation.z = 0.0; //q.z
    planner_run_2.request.goal_pose.orientation.w = 0.0; //q.w
    //Set max planning time
    planner_run_2.request.flag_iter_or_time = 1;
    planner_run_2.request.iter_or_time = 30; //in seconds
    //Set features
    planner_run_2.request.informed_sampling = true;
    planner_run_2.request.tree_optimization = true;
    //Show tree in RViz
    planner_run_2.request.show_tree = true;

    //Run planner
    //client_run_planner_map_poses.call(planner_run_2);

    // --- Execution ---
    execution_msgs::execute_trajectory_rviz execute_trajectory_2;
    execute_trajectory_2.request.planner_type = "bi_informed_rrt_star";
    execute_trajectory_2.request.run_id = "1";

    //Execute plan
    //client_execute_rviz.call(execute_trajectory_2);


    //++++++++ RUN PLANNER WITH CONFIGS/POSES GIVEN IN BASE_LINK FRAME ++++++++++++++

    //Run planner with start and goal config
    planner_msgs::run_planner_configs planner_run_3;
    planner_run_3.request.planner_type = "bi";
    planner_run_3.request.run_id = 2;
    //Set start configuration
    planner_run_3.request.start_config.push_back(0.0);
    planner_run_3.request.start_config.push_back(2.5);
    planner_run_3.request.start_config.push_back(0.0);
    //Set goal configuration
    planner_run_3.request.goal_config.push_back(-3.0);
    planner_run_3.request.goal_config.push_back(2.5);
    planner_run_3.request.goal_config.push_back(0.0);
    //Set max planning time
    planner_run_3.request.flag_iter_or_time = 1;
    planner_run_3.request.iter_or_time = 30; //in seconds
    //Set features
    planner_run_3.request.informed_sampling = true;
    planner_run_3.request.tree_optimization = true;
    //Show tree in RViz
    planner_run_3.request.show_tree = true;

    //Run planner
    client_run_planner_configs.call(planner_run_3);


    // ----------------- Execution --------------------------
    execution_msgs::execute_trajectory_rviz execute_trajectory_3;
    execute_trajectory_3.request.planner_type = "bi_informed_rrt_star";
    execute_trajectory_3.request.run_id = "2";

    //Execute plan
    client_execute_rviz.call(execute_trajectory_3);


    //Shutdown node
    ros::shutdown();

    return 0;
}




