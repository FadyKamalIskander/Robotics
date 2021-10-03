#!/bin/sh
xterm  -e  " source $(pwd)/../../devel/setup.bash && export ROBOT_INITIAL_POSE='-x 4 -y -5 -z 0' && roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(pwd)/../worlds/myWorld.xml " &
sleep 15
xterm -e " source $(pwd)/../../devel/setup.bash && roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(pwd)/../map/map.yaml " &
sleep 5
xterm -e " source $(pwd)/../../devel/setup.bash && roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 5
xterm -e " source $(pwd)/../../devel/setup.bash && rosrun add_markers add_markers_amcl " &
sleep 10
xterm -e " source $(pwd)/../../devel/setup.bash && roslaunch pick_objects pick_objects.launch "
