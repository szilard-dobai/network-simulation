/*
 * Queue.cc
 *
 *  Created on: Oct 24, 2020
 *     Author: Szilard Dobai
 */

#include <omnetpp.h>
#define min(a,b) a<=b ? a : b;

using namespace omnetpp;

typedef struct map {
    int weight;
    int index;
} map;

/**
 * Packet scheduler; see NED file for more info.
 */
class Scheduler: public cSimpleModule {
private:
    cMessage *sendMessageEvent;
    int *queueLength;
    int *radioLinkQuality;
    int *timeSinceLastServed;
    int responseReceivedCounter = 0;
//    int timeSinceLastServed[gateSize("out")];
//    int userWeight[];

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void askForQueueLengths();
    int* runPFAlgo();
    void scheduleMessages();
    const char* intToString(int num);
};

Define_Module(Scheduler);

void Scheduler::initialize() {
    EV << "SCHEDULER INITIALIZED." << endl;
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime() + par("generateInterval"), sendMessageEvent);

    int usersCount = (int) getAncestorPar("usersCount");

    queueLength = (int*) malloc(sizeof(int) * usersCount);
    radioLinkQuality = (int*) malloc(sizeof(int) * usersCount);
    timeSinceLastServed = (int*) malloc(sizeof(int) * usersCount);

    for (int counter = 0; counter < usersCount; counter++) {
        radioLinkQuality[counter] = (int) getParentModule()->getSubmodule(
                "user", counter)->par("radioLinkQuality");
    }

    for (int counter = 0; counter < usersCount; counter++) {
        timeSinceLastServed[counter] = 1;
    }
}

void Scheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        ASSERT(msg == sendMessageEvent);

        double generateDelay = par("generateInterval");

        askForQueueLengths();

        scheduleAt(simTime() + generateDelay, sendMessageEvent);
    } else {
        queueLength[msg->getArrivalGate()->getIndex()] = std::stoi(
                msg->getName());
        responseReceivedCounter++;
        if (responseReceivedCounter == (int) getAncestorPar("usersCount")) {
            responseReceivedCounter = 0;
            runPFAlgo();
        }
    }
}

const char* Scheduler::intToString(int num) {
    std::string s = std::to_string(num);
    char const *messageTextString = s.c_str();

    return messageTextString;
}

void Scheduler::askForQueueLengths() {
    for (int counter = 0; counter < gateSize("out"); counter++) {
        send(new cMessage("-1"), "out", counter);
    }
}

int* Scheduler::runPFAlgo() {
    int usersCount = (int) getAncestorPar("usersCount");
    map userList[usersCount], aux;
    int freeChannels = (int) getAncestorPar("channelsCount"), servedAmount = 0;

    int i = 0, j = 0;

    for (i = 0; i < usersCount; i++) {
        userList[i].index = i;
        userList[i].weight = 0;
    }

    for (i = 0; i < usersCount; i++)
        {
        userList[i].weight = radioLinkQuality[i] * timeSinceLastServed[i];
        EV << "WEIGHT = "<<  userList[i].weight << "\n";
        }

//    printf ("\n\n");
//    for (i = 0; i < n; i++)
//      printf ("Computed weight of %d is %d.\n", i + 1, userList[i].weight);
//
    for (i = 0; i < usersCount - 1; i++)
        for (j = i; j < usersCount; j++)
            if (userList[i].weight <= userList[j].weight) {
                aux = userList[i];
                userList[i] = userList[j];
                userList[j] = aux;
            }



    for (i = 0; i < usersCount; i++) {
        servedAmount = min(userList[i].weight, queueLength[userList[i].index]);
        if (freeChannels == 0) {
            timeSinceLastServed[userList[i].index]++;
        } else {
            if (freeChannels >= servedAmount) {
                send(new cMessage(intToString(servedAmount)), "out",
                        userList[i].index);
//                EV << "USER " << userList[i].index + 1 << " SEND "
//                          << servedAmount << " MESSAGES.\n";
                timeSinceLastServed[userList[i].index] = 1;
                freeChannels -= servedAmount;
            } else {

                send(new cMessage(intToString(servedAmount)), "out",
                        userList[i].index);
//                EV << "USER " << userList[i].index + 1 << " SEND "
//                          << servedAmount << " MESSAGES.\n";
                timeSinceLastServed[userList[i].index] = 1;
                freeChannels -= freeChannels;
            }

        }
    }
    return 0;
}

