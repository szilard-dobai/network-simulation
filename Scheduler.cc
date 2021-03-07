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
    int *radioLinkQuality, *initialRadioLinkQuality;
    int *timeSinceLastServed;
    int responseReceivedCounter = 0;
    int lastUserServed = 0;
    cOutVector freeChannelsVec, totalQLVec;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void askForQueueLengths();
    int runPFAlgo();
    int runWRRAlgo();
    void scheduleMessages();
    const char* intToString(int num);
};

Define_Module(Scheduler);

void Scheduler::initialize() {
    EV << "SCHEDULER INITIALIZED." << endl;
    freeChannelsVec.setName("Scheduler free channels");
    totalQLVec.setName("Total users' queues lengths");
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime() + par("generateInterval"), sendMessageEvent);

    int usersCount = (int) getAncestorPar("usersCount");

    queueLength = (int*) malloc(sizeof(int) * usersCount);
    radioLinkQuality = (int*) malloc(sizeof(int) * usersCount);
    initialRadioLinkQuality = (int*) malloc(sizeof(int) * usersCount);
    timeSinceLastServed = (int*) malloc(sizeof(int) * usersCount);

    for (int counter = 0; counter < usersCount; counter++) {
        initialRadioLinkQuality[counter] =
                (int) getParentModule()->getSubmodule("user", counter)->par(
                        "radioLinkQuality");
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

            for (int counter = 0; counter < (int) getAncestorPar("usersCount");
                    counter++) {
                radioLinkQuality[counter] =
                        (int) getParentModule()->getSubmodule("user", counter)->par(
                                "radioLinkQuality");
            }

            if (getAncestorPar("algorithm").stdstringValue().compare(
                    std::string("pf")) == 0)
                runPFAlgo();
            else
                runWRRAlgo();
        }

        for (int counter = 0; counter < (int) getAncestorPar("usersCount");
                counter++) {
            getParentModule()->getSubmodule("user", counter)->par(
                    "radioLinkQuality").setIntValue(
                    initialRadioLinkQuality[counter]);
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

int Scheduler::runPFAlgo() {
    int usersCount = (int) getAncestorPar("usersCount");
    map userList[usersCount], aux;
    int freeChannels = (int) getAncestorPar("channelsCount"),
            intendedServedAmount = 0, actualServedAmount = 0, emptyQueues = 0,
            statisticalSum = 0;

    int i = 0, j = 0;
    for (i = 0; i < usersCount; i++) {
        userList[i].index = i;
        userList[i].weight = 0;
    }

    for (i = 0; i < usersCount; i++) {
        userList[i].weight = radioLinkQuality[i] * timeSinceLastServed[i];
    }

    for (i = 0; i < usersCount - 1; i++)
        for (j = i; j < usersCount; j++)
            if (userList[i].weight <= userList[j].weight) {
                aux = userList[i];
                userList[i] = userList[j];
                userList[j] = aux;
            }

    for (j = 0; j < usersCount; j++)
        if (queueLength[j] == 0)
            emptyQueues++;
    while (emptyQueues != usersCount && freeChannels > 0) {
        for (i = 0; i < usersCount; i++) {
            if (freeChannels == 0) {
                timeSinceLastServed[userList[i].index]++;
            }

            intendedServedAmount = min(userList[i].weight,
                    queueLength[userList[i].index])
            ;
            actualServedAmount = min(intendedServedAmount, freeChannels)
            ;
            if (actualServedAmount > 0) {
                send(new cMessage(intToString(actualServedAmount)), "out",
                        userList[i].index);
                EV << "USER " << userList[i].index + 1 << " MAY SEND "
                          << actualServedAmount << " MESSAGES.\n";
                freeChannels -= actualServedAmount;
                queueLength[userList[i].index] -= actualServedAmount;
                timeSinceLastServed[userList[i].index] = 1;
            }
        }
        for (i = 0; i < usersCount; i++) {
            userList[i].weight = radioLinkQuality[userList[i].index]
                    * timeSinceLastServed[userList[i].index];
        }
        emptyQueues = 0;
        for (j = 0; j < usersCount; j++)
            if (queueLength[j] == 0)
                emptyQueues++;
    }

    freeChannelsVec.record(freeChannels);
    for (i = 0; i < usersCount; i++)
        statisticalSum += queueLength[i];
    totalQLVec.record(statisticalSum);

    return 0;

}

int Scheduler::runWRRAlgo() {
    int usersCount = (int) getAncestorPar("usersCount");
    int freeChannels = (int) getAncestorPar("channelsCount"),
            intendedServedAmount = 0, actualServedAmount = 0, emptyQueues = 0,
            statisticalSum = 0;

    int i = 0, j = 0;

    for (i = lastUserServed; i <= usersCount; i++) {
        if (i == usersCount) {
            lastUserServed = 0;
            i = 0;
        }

        for (j = 0; j < usersCount; j++)
            if (queueLength[j] == 0)
                emptyQueues++;
        if (emptyQueues == usersCount) {
            lastUserServed = i;
            break;
        }
        emptyQueues = 0;

        if (freeChannels == 0) {
            lastUserServed = i;
            break;
        } else {
            intendedServedAmount = min(radioLinkQuality[i], queueLength[i])
            ;
            actualServedAmount = min(intendedServedAmount, freeChannels)
            ;

            if (actualServedAmount > 0) {
                send(new cMessage(intToString(actualServedAmount)), "out", i);
                EV << "USER " << i + 1 << " MAY SEND " << actualServedAmount
                          << " MESSAGES.\n";
                freeChannels -= actualServedAmount;
                queueLength[i] -= actualServedAmount;
            }
        }

    }

    freeChannelsVec.record(freeChannels);
    for (i = 0; i < usersCount; i++)
        statisticalSum += queueLength[i];
    totalQLVec.record(statisticalSum);

    return 0;
}

