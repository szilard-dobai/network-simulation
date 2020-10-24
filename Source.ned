//
// Generates jobs (messages) with the given generateInterval time,
// which can be a random variate.
//
simple Source
{
    parameters:
        int startingMessage;
        double generateInterval @unit(s);
        @display("i=block/source");
    gates:
        output out;
}
