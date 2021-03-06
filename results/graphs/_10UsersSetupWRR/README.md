# 10 users scenario - Weighted Round Robin algorithm

## Setup

**Network** consisting of a **scheduler**, **users** and a **sink**.

Each **user** consists of a **generator** and a **queue**.
  
For this analysis, we chose the following weights for the users: **w = [1, 2, 4, 8, 1, 2, 4, 8, 1, 2]**.

The scheduler runs every **1ms** and has the task of distributing the **30 channels** between the users. 

## 90% network load

![_10UsersSetupWRR_90%_QB](./Network%20load%2090%25/_10UsersSetupWRR_90%25_QB.svg)

![_10UsersSetupWRR_90%_QA](./Network%20load%2090%25/_10UsersSetupWRR_90%25_QA.svg)

![_10UsersSetupWRR_90%_F](./Network%20load%2090%25/_10UsersSetupWRR_90%25_F.svg)

![_10UsersSetupWRR_90%_TQ](./Network%20load%2090%25/_10UsersSetupWRR_90%25_TQ.svg)

**Each** user gets to send its messages. 

There **are** free channels remaining after each scheduling cycle. 

The sum of the users' queue lengths is **0**.

<br/>
<br/>
<br/>

## 100% network load

![_10UsersSetupWRR_100%_QB](./Network%20load%20100%25/_10UsersSetupWRR_100%25_QB.svg)

![_10UsersSetupWRR_100%_QA](./Network%20load%20100%25/_10UsersSetupWRR_100%25_QA.svg)

![_10UsersSetupWRR_100%_F](./Network%20load%20100%25/_10UsersSetupWRR_100%25_F.svg)

![_10UsersSetupWRR_100%_TQ](./Network%20load%20100%25/_10UsersSetupWRR_100%25_TQ.svg)

**Each** user gets to send its messages. 

There **are barely** any free channels remaining after each scheduling cycle. 

The sum of the users' queue lengths is **sometimes 0**.

<br/>
<br/>
<br/>

## 150% network load

![_10UsersSetupWRR_150%_QB](./Network%20load%20150%25/_10UsersSetupWRR_150%25_QB.svg)

![_10UsersSetupWRR_150%_QA](./Network%20load%20150%25/_10UsersSetupWRR_150%25_QA.svg)

![_10UsersSetupWRR_150%_F](./Network%20load%20150%25/_10UsersSetupWRR_150%25_F.svg)

![_10UsersSetupWRR_150%_TQ](./Network%20load%20150%25/_10UsersSetupWRR_150%25_TQ.svg)

More **"important"** users (those with higher weights) get to send their messages. 

There **aren't** any free channels remaining after each scheduling cycle. 

The sum of the users' queue **goes up linearly**.

WRR makes it a point to serve **each user**, however it can.

<br/>
<br/>
<br/>
