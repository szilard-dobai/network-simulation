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
    void askForQueueLengths();
    void scheduleMessages();
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

        askForQueueLengths();
        // run sch alg
        scheduleMessages();

        scheduleAt(simTime() + generateDelay, sendMessageEvent);
    } else {
        queueLength[msg->getArrivalGate()->getIndex()] = std::stoi(
                msg->getName());
    }
}

void Scheduler::askForQueueLengths() {
    for (int counter = 0; counter < gateSize("out"); counter++) {
        send(new cMessage("-1"), "out", counter);
    }
}

void Scheduler::scheduleMessages() {
    if (queueLength[0] == 5) {
        send(new cMessage("5"), "out", 0);
        EV << "USER 0 MAY SEND 5 MESSAGES." << endl;
    }
    if (queueLength[1] == 3) {
        send(new cMessage("3"), "out", 1);
        EV << "USER 1 MAY SEND 3 MESSAGES." << endl;
    }
}
