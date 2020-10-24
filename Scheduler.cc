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
protected:

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Scheduler);

void Scheduler::initialize() {
    EV << "SCHEDULER INITIALIZED." << endl;
}

void Scheduler::handleMessage(cMessage *msg) {

}
