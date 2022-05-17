# Simultaneous Localization and Mapping with Extended Kalman Filter
This directory provides an implementation of Simultaneous Localization and
Mapping (SLAM) using Extended Kalman Filter (EKF) algorithm.

## Description
The program models a robot operating in a 2D environment (e.g., a self-driving
car operating in a city), as shown in the figure below (left). The right figure
shows the estimated trajectory of the robot, as well as the estimated location
of the landmarks in the environment. The ellipses show the uncertainties the
method accounts for in its estimations.

<p align="center">
  <img
    width="850"
    height="350"
    src="../../.images/ekfslam.png"
  >
</p>

## Code & Data
`input-logs/` includes a synthetic scenario with six landmarks. The log file
includes 1) control inputs (distance traveled by the robot and its rotation)
and 2) measurements (bearing angle and range of six landmarks). More scenarios
can be generated using, say, autonomous vehicle simulators like
[CARLA](https://carla.org).
