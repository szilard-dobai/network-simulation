/*
 * Queue.cc
 *
 *  Created on: Oct 24, 2020
 *     Author: Szilard Dobai
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Packet scheduler; see NED file for more info.
 */
class Scheduler: public cSimpleModule {
private:
    cMessage *sendMessageEvent;
    int queueLength[];

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Scheduler);

void Scheduler::initialize() {
    EV << "SCHEDULER INITIALIZED." << endl;
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime() + par("generateInterval"), sendMessageEvent);
}

void Scheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        ASSERT(msg == sendMessageEvent);

        double generateDelay = par("generateInterval");

        // call scheduling algorithm

        scheduleAt(simTime() + generateDelay, sendMessageEvent);
    } else {
        queueLength[msg->getArrivalGate()->getIndex()] = std::stoi(msg->getName());
    }
}
