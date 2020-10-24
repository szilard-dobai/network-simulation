/*
 * Sink.cc
 *
 *  Created on: Oct 22, 2020
 *      Author: Szilard Dobai
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Packet sink; see NED file for more info.
 */
class Sink: public cSimpleModule {
private:
    simsignal_t lifetimeSignal;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Sink);

void Sink::initialize() {
    lifetimeSignal = registerSignal("lifetime");
}

void Sink::handleMessage(cMessage *msg) {
    simtime_t lifetime = simTime() - msg->getCreationTime();
    EV << "RECEIVED MESSAGE \"" << msg->getName() << "\" FROM "
              << msg->getSenderModule()->getParentModule()->getName() << "["
              << msg->getSenderModule()->getParentModule()->getIndex() << "]"
              << ", LIFETIME: " << lifetime << "s." << endl;
    emit(lifetimeSignal, lifetime);
    delete msg;
}
