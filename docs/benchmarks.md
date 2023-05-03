---
title: RTRBench Listing of Benchmarks 
---

#  RTRBench Listing of Benchmarks

Following, a listing of RTRBench kernels is included. Follow the links for the
kernels to get more details, including the specifications of the code and data.
We break the kernels into three groups based on their stage in robots' software
pipeline.

### Perception

- **01.pfl**  
Robot localization using Particle Filter (PF).  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/perception/01.pfl)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/perception/01.pfl/README.md)]

- **02.ekfslam**  
Robot localization and mapping using Extended Kalman Filter (EKF).  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/perception/02.ekfslam)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/perception/02.ekfslam/README.md)]

- **03.srec**  
Scene reconstruction using the Iterative Closest Point (ICP) algorithm.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/perception/03.srec)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/perception/03.srec/README.md)]

### Planning

- **04.pp2d**  
Mobile robot path planning using the A\* algorithm in 2D environments.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/04.pp2d)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/04.pp2d/README.md)]

- **05.pp3d**  
Mobile robot path planning using the A\* algorithm in 3D environments.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/05.pp3d)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/05.pp3d/README.md)]

- **06.movtar**  
Mobile robot path planning using the A\* algorithm with a moving target.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/06.movtar)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/06.movtar/README.md)]

- **07.prm**  
Stationary robotic arm planning using the Probabilistic RoadMap (PRM)
algorithm.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/07-10.robot-arm)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/07-10.robot-arm/README.md)]

- **08.rrt**  
Stationary robotic arm planning using the Rapidly-exploring Random Tree (RRT)
algorithm.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/07-10.robot-arm)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/07-10.robot-arm/README.md)]

- **09.rrtstar**  
Stationary robotic arm planning using the Rapidly-exploring Random Tree Star
(RRT*) algorithm.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/07-10.robot-arm)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/07-10.robot-arm/README.md)]

- **10.rrtpp**  
Stationary robotic arm planning using the Rapidly-exploring Random Tree (RRT)
algorithm with path shortcutting.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/07-10.robot-arm)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/07-10.robot-arm/README.md)]

- **11.sym-blkw**  
Solving Blocksworld problem using symbolic planning.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/11-12.sym-planner)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/11-12.sym-planner/README.md)]

- **12.sym-fext**  
Solving a firefighting problem using symbolic planning.  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/planning/11-12.sym-planner)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/planning/11-12.sym-planner/README.md)]

### Control

- **13.dmp**  
Path tracking using Dynamic Movement Primitives (DMP).  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/control/13.dmp)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/control/13.dmp/README.md)]

- **14.mpc**  
Path tracking using Model Predictive Control (MPC).  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/control/14.mpc)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/control/14.mpc/README.md)]

- **15.cem**  
Reinforcement learning using Cross-Entropy Method (CEM).  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/control/15.cem)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/control/15.cem/README.md)]

- **16.bo**  
Reinforcement learning using Bayesian Optimization (BO).  
[[Code](https://github.com/cmu-roboarch/rtrbench/tree/main/control/16.bo)] 
[[Doc](https://github.com/cmu-roboarch/rtrbench/blob/main/control/16.bo/README.md)]
