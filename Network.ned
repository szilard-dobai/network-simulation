network Network
{
    @display("bgb=612,281");
    submodules:
        user: User {
            parameters:
                @display("p=293,61");
        }
        sink: Sink {
            parameters:
                @display("p=436,168");
        }
    connections:
			user.out --> sink.in;
}
