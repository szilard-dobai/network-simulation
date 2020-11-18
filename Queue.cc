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
private:
    cOutVector queueLengthBA, queueLengthAA;
protected:
    cMessage *msgFromQueue;
    cQueue queue;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    const char* intToString(int num);
};

Define_Module(Queue);

void Queue::initialize() {
    queue.setName("queue");
    queueLengthBA.setName("Queue before scheduler's run.");
    queueLengthAA.setName("Queue after scheduler's run.");
}

const char* Queue::intToString(int num) {
    std::string s = std::to_string(num);
    char const *messageTextString = s.c_str();

    return messageTextString;
}

void Queue::handleMessage(cMessage *msg) {
    if (opp_strcmp(msg->getArrivalGate()->getName(), "inGenerator") == 0) {
        //EV << "Received message: " << msg->getName() << endl;
        queue.insert(msg);
    } else {
        //EV << "RECEIVED MESSAGE FROM scheduler!\n";
        int messageFromScheduler = std::stoi(msg->getName());
        if (messageFromScheduler == -1) {
            send(new cMessage(intToString(queue.getLength())), "outScheduler");
            queueLengthBA.record(queue.getLength());
        } else {
            for (int counter = 0; counter < messageFromScheduler; counter++) {

                if (!queue.isEmpty()) {
                    msgFromQueue = (cMessage*) queue.pop();
                    send(msgFromQueue, "outSink");
                }
            }
            queueLengthAA.record(queue.getLength());
        }
    }
}
