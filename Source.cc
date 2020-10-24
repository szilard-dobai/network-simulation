/*
 * Source.cc
 *
 *  Created on: Oct 21, 2020
 *      Author: Szilard Dobai
 */

#include <omnetpp.h>

using namespace omnetpp;

class Source: public cSimpleModule {
private:
    cMessage *sendMessageEvent;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Source);

void Source::initialize() {
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime() + par("generateInterval"), sendMessageEvent);
    EV << "Initialized generator.\n";
}

void Source::handleMessage(cMessage *msg) {
    ASSERT(msg == sendMessageEvent);

    cMessage *job = new cMessage("2");
    send(job, "out");
    double generateDelay = par("generateInterval");
    EV << "Sent job with delay " << generateDelay << "\n";

    scheduleAt(simTime() + generateDelay, sendMessageEvent);
}
