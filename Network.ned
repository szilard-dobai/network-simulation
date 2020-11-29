network Network
{
    parameters:
        int usersCount;
        int channelsCount;
        double networkLoad;
        string algorithm;
        @display("bgb=612,281");
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
    connections:
        for i=0..usersCount-1 {
            scheduler.out++ --> user[i].inScheduler;
            scheduler.in++ <-- user[i].outScheduler;
            user[i].outSink --> sink.in++;
        }
}
