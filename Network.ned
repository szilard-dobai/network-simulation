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
                @display("p=150,253;i=block/blackboard");
        }
    connections:
        for i=0..usersCount-1 {
            scheduler.out++ --> user[i].inScheduler;
            scheduler.in++ <-- user[i].outScheduler;
            user[i].outSink --> sink.in++;
        }
        scheduler.outFlc --> flc.in;
        scheduler.inFlc <-- flc.out;
}
