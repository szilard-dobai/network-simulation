network Network
{
    parameters:
        int usersCount;
        int channelsCount;
        double networkLoad;
        string algorithm;
        @display("bgb=612,340");
    submodules:
        scheduler: Scheduler {
            parameters:
                @display("p=150,168");
        }
        user[usersCount]: User {
            parameters:
                @display("p=290,61");
        }
        sink: Sink {
            parameters:
                @display("p=440,168");
        }
        flc: FLC {
            parameters:
                @display("p=440,257");
        }
        gen_flc: GeneratorFLC {
            parameters:
                @display("p=150,257");
        }
    connections:
        for i=0..usersCount-1 {
            scheduler.out++ --> user[i].inScheduler;
            scheduler.in++ <-- user[i].outScheduler;
            user[i].outSink --> sink.in++;
        }
        gen_flc.out --> flc.in;
}
