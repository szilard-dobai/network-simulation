module User
{
    parameters:
        @display("i=block/user");
    gates:
        input in;
        output out;
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
        in --> queue.inScheduler;
        queue.out --> out;
}
