//
// Tells users when and how many packets to send
// based on scheduling algorithm.
//
simple Scheduler
{
    parameters:
        @display("i=block/dispatch");
    gates:
        output out[];
}
