//
// Network node, generates and holds messages in
// buffer until told to send.
//
module User
{
    parameters:
        int radioLinkQuality;
        @display("i=block/user");
    gates:
        input inScheduler;
        output outScheduler;
        output outSink;
    submodules:
        gen: Source {
            parameters:
                @display("p=103,61");
        }
        queue: Queue {
            parameters:
                @display("p=103,138");
        }
    connections:
        gen.out --> queue.inGenerator;
        inScheduler --> queue.inScheduler;
        queue.outScheduler --> outScheduler;
        queue.outSink --> outSink;
}
