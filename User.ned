network User
{
    submodules:
        gen1: Source {
            parameters:
                @display("p=69,174");
        }
        gen2: Source {
            parameters:
                @display("p=69,44");
        }
        sink: Sink {
            parameters:
                @display("p=329,100");
        }
        queue: Queue {
            parameters:
                @display("p=165,100");
        }
    connections:
        gen1.out --> queue.in1;
        gen2.out --> queue.in2;
        queue.out --> sink.in;
}
