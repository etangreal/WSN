//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
//-----------------------------------------------------------------------------

message Msg {

    //---MESSAGE-PAYLOAD---------------------------------------------------------------------------

    int senderID    = 0;
    int hopCount    = 0;

    //---COMMAND-PAYLOAD---------------------------------------------------------------------------

    long commandID	= 0;

    //---STATS-PAYLOAD-----------------------------------------------------------------------------

    // the node responsible for having collected the stats
    int statsSenderID = 0;
    
    // the consecutive stat packet number - used to keeping track of packets to prevent recounting/resending stats
    int statsPacketCount = 0;

    // statsIDs[0...9] => the node's "overheard" sending messages -i.e: the neigbor's IDs
    int statsIDs[10];

    // stats[0...9] => the corresponding neighbors aggregate statistics
    int stats[10];

}