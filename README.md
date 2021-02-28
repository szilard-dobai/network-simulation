# network-simulation
OMNeT++ University Project


## The simulation model consists of the following OMNeT++ modules:
- A number of mobile users. In the first stages of the model you can implement two identical users, then you can consider a number of K users, organized as an array of users.
- A scheduler. The scheduler reads the lengths of users' queues and implements a scheduling algorithm that allocates a number of radio channels to each user. Then it informs each users how many radio blocks to send during the current scheduling cycle.
You can consider that the total number of radio channels is B, for example B=30 in LTE. The scheduler is activated every TB ms (a scheduling cycle). The value of the scheduling cycle is 1 ms in LTE.
- A sink. The sink models the destination of the data. When the data packets created by an user arrive to the sink module, the sink simply deletes the OMNeT++ messages representing the data packets. Also, the sink is used to collect statistics about the simulation, statistics that can be for each user and for the entire system. This statistical information can be: the number of data packets that arrive to the sink, the mean, minimum and maximum delay of the data packets, etc.


A user consists of a source module (or generator) and a buffer module, implemented as a queue. The generator generates a number of data packets at certain time intervals. The data packets are stored in the buffer. When the scheduler informs the user that it can send a number of, e.g. m data packets, then the firsts m data packets from the buffer are send to the destination (the sink module).


We can consider that all data packets have a fixed length.

## Fuzzy Logic Controller Upgrade

Consider a radio system (a cell) where there can be at most K users (eg K = 10). The resource allocation algorithm is of the WRR (Weighted Round Robin) type. We consider that of the K users, some have W_HP (high priority) weight, others W_MP (medium priority), and others W_LP (low priority). For example W_HP = 4, W_MP = 2 and W_LP = 1. The total capacity of the network is B data packets per ms Tc (milliseconds) (eg B = 8 and Tc = 20 at GPRS, or B = 20 and Tc = 1 at LTE). All data packets are considered to have a fixed length equal to 1. Adjust the share of HP users so that the average delay of their packets is not (as far as possible) greater than a given value. Fuzzy inference will be used for this purpose.

