# 3 users scenario - Proportionally Fair algorithm

## Setup

**Network** consisting of a **scheduler**, **users** and a **sink**.

Each **user** consists of a **generator** and a **queue**.
  
For this analysis, we chose the following weights for the users: **w = [1, 2, 3]**.

The scheduler runs every **1ms** and has the task of distributing the **30 channels** between the users. 

## 90% network load

![_3UsersSetupPF_90%_QB](./Network load 90%/_3UsersSetupPF_90%_QA.svg)

![_3UsersSetupPF_90%_QA](./Network load 90%/_3UsersSetupPF_90%_QB.svg)

![_3UsersSetupPF_90%_F](./Network load 90%/_3UsersSetupPF_90%_QF.svg)

![_3UsersSetupPF_90%_TQ](./Network load 90%/_3UsersSetupPF_90%_TQ.svg)

Each user gets to send its messages. There are free channels remaining after each scheduling cycle. The sum of the users' queue lengths is 0.

<br/>
<br/>
<br/>

## 100% network load

![_3UsersSetupPF_100%_QB](./Network load 100%/_3UsersSetupPF_100%_QA.svg)

![_3UsersSetupPF_100%_QA](./Network load 100%/_3UsersSetupPF_100%_QB.svg)

![_3UsersSetupPF_100%_F](./Network load 100%/_3UsersSetupPF_100%_QF.svg)

![_3UsersSetupPF_100%_TQ](./Network load 100%/_3UsersSetupPF_100%_TQ.svg)

Each user gets to send its messages. There are barely any free channels remaining after each scheduling cycle. The sum of the users' queue lengths is 0.

<br/>
<br/>
<br/>

## 150% network load

![_3UsersSetupPF_150%_QB](./Network load 150%/_3UsersSetupPF_150%_QA.svg)

![_3UsersSetupPF_150%_QA](./Network load 150%/_3UsersSetupPF_150%_QB.svg)

![_3UsersSetupPF_150%_F](./Network load 150%/_3UsersSetupPF_150%_QF.svg)

![_3UsersSetupPF_150%_TQ](./Network load 150%/_3UsersSetupPF_150%_TQ.svg)

More "important" users (those with higher weights) get to send their messages. There aren't any free channels remaining after each scheduling cycle. The sum of the users' queue goes up linearly.

<br/>
<br/>
<br/>