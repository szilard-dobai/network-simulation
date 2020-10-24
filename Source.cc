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
    int counter = 0;
    int startingMessage;
    cMessage *sendMessageEvent;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    cMessage* createMessage();
};

Define_Module(Source);

void Source::initialize() {
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime() + par("generateInterval"), sendMessageEvent);
    startingMessage = par("startingMessage").intValue();
}

cMessage* Source::createMessage() {
    int messageText = startingMessage + counter;
    std::string s = std::to_string(messageText);
    char const *messageTextString = s.c_str();
    counter++;

    return new cMessage(messageTextString);
}

void Source::handleMessage(cMessage *msg) {
    ASSERT(msg == sendMessageEvent);

    cMessage *job = createMessage();
    send(job, "out");
    double generateDelay = par("generateInterval");

    scheduleAt(simTime() + generateDelay, sendMessageEvent);
}
