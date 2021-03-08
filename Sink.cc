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
    cOutVector waitTimeVector[100];
    cOutVector hpWaitTimeVector;
    double waitTimeSum[100];
    int waitTimeCount[100];

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Sink);

void Sink::initialize() {
    lifetimeSignal = registerSignal("lifetime");
    hpWaitTimeVector.setName("High Priority Users Average Wait Time");
    for (int i = 0; i < (int) getAncestorPar("usersCount"); i++) {
        std::string vecName = "Wait Time For User ";
        vecName += std::to_string(i);

        int userWeight = getParentModule()->getSubmodule("user", i)->par(
                "radioLinkQuality").intValue();
        if (userWeight == 1)
            vecName += " (LP).";
        else if (userWeight == 2)
            vecName += " (MP).";
        else if (userWeight == 4)
            vecName += " (HP).";

        waitTimeVector[i].setName(vecName.c_str());

        waitTimeSum[i] = 0;
        waitTimeCount[i] = 0;
    }
}

void Sink::handleMessage(cMessage *msg) {
    simtime_t lifetime = simTime() - msg->getCreationTime();
    EV << "RECEIVED MESSAGE \"" << msg->getName() << "\" FROM "
              << msg->getSenderModule()->getParentModule()->getName() << "["
              << msg->getSenderModule()->getParentModule()->getIndex() << "]"
              << ", LIFETIME: " << lifetime << "s." << endl;

    int msgSenderUserIndex =
            msg->getSenderModule()->getParentModule()->getIndex();
    waitTimeVector[msgSenderUserIndex].record(lifetime);
    waitTimeSum[msgSenderUserIndex] += lifetime.dbl();
    waitTimeCount[msgSenderUserIndex] += 1;
//    getParentModule()->getSubmodule("user", msgSenderUserIndex)->par(
//            "avgWaitTime").setDoubleValue(
//            waitTimeSum[msgSenderUserIndex]
//                    / waitTimeCount[msgSenderUserIndex]);
    getParentModule()->getSubmodule("user", msgSenderUserIndex)->par(
            "avgWaitTime").setDoubleValue(lifetime.dbl());

    if (msg->getSenderModule()->getParentModule()->par("radioLinkQuality").intValue()
            > 2) {
        hpWaitTimeVector.record(lifetime);
    }

    emit(lifetimeSignal, lifetime);
    delete msg;
}
