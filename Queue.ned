//
// Receives and adds messages to buffer until told to
// pop and send a given number of messages.
//
simple Queue
{
    parameters:
        @display("i=block/source");
    gates:
        input in1;
        input in2;
        output out;
}
