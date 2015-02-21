
//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
//-----------------------------------------------------------------------------

simple Base {

    parameters:
        @display("i=block/routing");

    gates:
        inout gate[];
}

simple Node {

    parameters:
        @display("i=block/routing");

    gates:
        inout gate[];
}

network WSN {

    types:
        channel Channel extends ned.DelayChannel {
            delay = 100ms;
        }

    submodules:
        base: Base;
        node[5]: Node;

    connections:
        base.gate++ <--> Channel <--> node[0].gate++;
        base.gate++ <--> Channel <--> node[1].gate++;
        base.gate++ <--> Channel <--> node[2].gate++;

        node[0].gate++ <--> Channel <--> node[2].gate++;

        node[3].gate++ <--> Channel <--> node[2].gate++;
        node[3].gate++ <--> Channel <--> node[4].gate++;

        node[0].gate++ <--> Channel <--> node[4].gate++;
}
