//
// Tells users when and how many packets to send
// based on scheduling algorithm.
//
simple Scheduler
{
    parameters:
        @display("i=block/dispatch");
        double generateInterval @unit(s);
    gates:
        output out[];
        input in[];
        output outFlc;
        input inFlc;
}
