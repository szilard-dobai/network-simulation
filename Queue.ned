//
// Receives and adds messages to buffer until told to
// pop and send a given number of messages.
//
simple Queue
{
    parameters:
        @display("i=block/queue");
    gates:
        input inGenerator;
        input inScheduler;
        output outScheduler;
        output outSink;
}
