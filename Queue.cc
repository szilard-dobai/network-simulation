/*
 * Queue.cc
 *
 *  Created on: Oct 24, 2020
 *     Author: Szilard Dobai
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Packet queue; see NED file for more info.
 */
class Queue: public cSimpleModule {
protected:
    cMessage *msgFromQueue;
    //cMessage *endServiceMsg;
    cQueue queue;
    //simsignal_t qlenSignal;
    //simsignal_t busySignal;
    //simsignal_t queueingTimeSignal;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Queue);

void Queue::initialize() {
//    endServiceMsg = new cMessage("end-service");
    queue.setName("queue");

//    qlenSignal = registerSignal("qlen");
//    busySignal = registerSignal("busy");
//    queueingTimeSignal = registerSignal("queueingTime");
//    emit(qlenSignal, queue.getLength());
//    emit(busySignal, false);
}

void Queue::handleMessage(cMessage *msg) {
    if (opp_strcmp(msg->getSenderModule()->getName(), "gen1") == 0) {
        EV << "RECEIVED MESSAGE FROM gen1!\n";
        queue.insert(msg);
    } else {
        EV << "RECEIVED MESSAGE FROM gen2!\n";
        int numberOfDataPackets = std::stoi(msg->getName());
        for (int counter = 0; counter < numberOfDataPackets; counter++) {
            EV << "SENDING MESSAGE " << counter << " FROM QUEUE\n";
            if (!queue.isEmpty()) {
                msgFromQueue = (cMessage*) queue.pop();
                send(msgFromQueue, "out");
            }
        }
    }
}

