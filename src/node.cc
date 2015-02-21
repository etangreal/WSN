
//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
// Node
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <omnetpp.h>

#include <stdio.h>
#include <string.h>

//#include <pthread.h>
//#include <unistd.h>

// Include a generated file: the header file created from Msg.msg ( by typing into the terminal> opp_msgc msg.msg )
// It contains the definition of the Msg class, derived from
// cMessage.
#include "msg_m.h"

#include <CmdNhbrTbl.h>
#include <StatPacketTracker.h>

//-----------------------------------------------------------------------------
// CLASS-DEFINITION
//-----------------------------------------------------------------------------

class Node : public cSimpleModule {

  private:
    long _sent;                                 //Number of messages that have been sent by this node
    long _received;                             //Number of messages that have been received by this node
    int _statsPacketCount;                      //Number of stats Packets(messages) that have been sent by this node 

    CmdNhbrTbl _cmdTbl;                         //Sent&Recieved messages - of Commands from Neighbors   [Tracking Table]
    StatPacketTracker _statsTracker;            //Sent&Recieved Stats messages(packets)                 [Tracking Table]

  public:
            Node();                             //Constuctor
            ~Node();                            //Destructor

  protected:
    virtual void initialize();                  // used to setup/initialize variables etc ... 
            // CORE ROUTINE: handleMessage
    virtual void handleMessage(cMessage* msg);  // whenever the node receives a message it is passed to this routine
            bool processMessage(Msg* msg);      // returns true if the processed msg is "new" -> i.e. never been seen by this node
            void forwardMessage(Msg* msg);      // sends a message to all the nodes connected to this node
            void updateMsg(Msg* msg);           // update/prepare a message with current node info -> e.g: stamps it with the current node's ID

            void sendStats();                   // get the stats (using: getCurrentStats), prepare message and dispatch it to neighbors
            Msg* getCurrentStats();             // aggregates the current stats and puts them in a message - ready to go ... 
            //_cmdTbl access
            CmdNhbrTbl* CmdTbl();               // the Command/Neighbors Tracking Table

            //udpate omnet GUI
            void updateDisplay();               // updates omnet with the node info (id|send|received)

    virtual void finish();                      // omnet calls this routine after all simulated events/messages have finished
};

// The module class needs to be registered with OMNeT++
Define_Module(Node);

//-----------------------------------------------------------------------------
// FUNCTION-IMPLEMENTATIONS
//-----------------------------------------------------------------------------

//constructor
Node::Node() {
    EV << "Node Constructor: id: " << getId() << "\n";

    //pthread_mutex_init ( &lock , NULL );
}

//-----------------------------------------------------------------------------

//destructor
Node::~Node() {
    //pthread_mutex_destroy(&lock);
}

//-----------------------------------------------------------------------------

//initialization
void Node::initialize() {

    // Initialize variables
    _sent = 0;
    _received = 0;
    _statsPacketCount = 0;

    WATCH(_sent);
    WATCH(_received);
    WATCH(_statsPacketCount);

}//initialize

//-----------------------------------------------------------------------------

void Node::handleMessage(cMessage* cmsg) {

    Msg* msg = check_and_cast<Msg*>(cmsg);
    EV << "\n[START: Node::handleMessage] (senderID:"<<msg->getSenderID()<<"|commandID:"<<msg->getCommandID()<<")\n";

    //-MESSAGE-ARRIVED-

    //statsSndID -> stats Sender ID
    //statsPktCount -> stats Packet Count
    int statsSndID = msg->getStatsSenderID();
    int statsPktCount = msg->getStatsPacketCount();
    // if its a stat packet and this node has not seen it before - dont drop! forward so that it can reach the base station;
    if (statsSndID > 0 && _statsTracker.accept(statsSndID, statsPktCount) ) { 
        EV << "\t[STATS] (statsSenderID:"<<statsSndID<<"|statsPacketCount:"<<statsPktCount<<")\n";
        bubble("FORWARDING STATS!!");
        forwardMessage(msg);
        return;
    }

    if (uniform(0,1) < 0.1) { //loose a message with probability 10%
        EV << "\t[ELSE: Node::handleMessage] (sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<")\t[ \"Losing\" message....]\n";
        bubble("LOST-MSG");
        delete msg;
        return;
    }

    _received++;

    if ( processMessage(msg) ) { //-IF-IT-IS-A-NEW-MESSAGE-

        EV << "\t[AFTER: if(processMssage) in Node::handleMessage] (sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<")\t[is NEW]\n";
        bubble("NEW-MSG");
        forwardMessage(msg);

    } else { //-ELSE->HAVE-SEEN-THIS-MESSAGE-BEFORE-

        EV << "\t[ELSE: Node::handleMessage] (sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<")\t[was RECEIVED PREVIOSLY - Deleting ...]\n";
        //bubble("SEEN-MSG");
        delete msg;

    }//if

    //-UPDATE-DISPLAY-

    if (ev.isGUI())
        updateDisplay();

}//handleMessage

//-----------------------------------------------------------------------------

// returns -> TRUE if the message successfully processed AND the processed msg was "new" -> i.e. never been seen by this node
//               (if msg was new -> we want to resend it to the neighbors...)
//            FALSE if the message was been processed previously ... 
//               (we want to updated however table to indicate/record stats on who sent us the message)
//               (but other than that it should not be resent)
//
// msg -> the message to be processed
bool Node::processMessage(Msg* msg) {

    EV << "[START Node::processMessage(msg)] (sndID:"<<msg->getSenderID()<<"|cmdID: "<<msg->getCommandID()<<")\n";

    int cmdID = msg->getCommandID();
    int sndID = msg->getSenderID();

    bool isNew = ( CmdTbl()->getCmdIdx(cmdID) < 0 );

    if ( !CmdTbl()->addUpd(sndID,cmdID) ) { //failed to addUpd the Command/Neighbor Table -> because its full ... 
        EV << "[END Node::processMessage] Command/Neighborhood table is full. Sending out Stats.... \n";
        
        sendStats();
        
        CmdTbl()->clear();
        CmdTbl()->addUpd(sndID,cmdID);
    }

    return isNew;
}

//-----------------------------------------------------------------------------

void Node::forwardMessage(Msg* msg) {
    EV << "Node::forwardMessage(msg) -> nodeID: "<< getId()<<"\n";

    // Increment hop count.
    msg->setHopCount( msg->getHopCount()+1 );
    updateMsg(msg);

    bubble("SEND-TO-ALL");
    // send the msg to all
    int n = gateSize("gate");
    for(int i=0;i<n;i++) {
        EV << "\tNode::forwardMessage(msg) -> sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<"\t GATE[" << i << "]\n";
        _sent++;
        send(msg->dup(), "gate$o", i);
    }

    delete msg;
}

//-----------------------------------------------------------------------------

void Node::updateMsg(Msg* msg) {

    msg->setSenderID( getId() );

    char desc[100];
    sprintf(desc, "s:%d|c:%ld", msg->getSenderID(), msg->getCommandID() );
    msg->setName(desc);
}

//-----------------------------------------------------------------------------

void Node::sendStats() {

    Msg* statsMsg = getCurrentStats();
    CmdTbl()->displayOmnet();
    CmdTbl()->displayStatsOmnet();

    bubble("SENDING-STATS!");

    //update this node's _statsTracker so that it does not Re-broadcast the statsMessage after receving it again from a neighbor
    _statsTracker.accept(statsMsg->getStatsSenderID(), statsMsg->getStatsPacketCount());
    forwardMessage(statsMsg);
}

//-----------------------------------------------------------------------------

Msg* Node::getCurrentStats() { 

    int statsIDs[CmdNhbrTbl::iSENDERS];
    int stats[CmdNhbrTbl::iSENDERS];
    CmdTbl()->getStats(statsIDs, stats);

    Msg* msg = new Msg(); //sMsg -> stats Message
    msg->setSenderID( getId() );
    msg->setCommandID( 0 );

    // The combination of statsSenderID & statsPacketCount is unique... 
    //      - used by the other nodes to identify the combination they have already "forwarded"
    //      - used by the server to identify a combination and only assimulate it once 
    msg->setStatsSenderID( getId() );
    msg->setStatsPacketCount( ++_statsPacketCount );

    for(int i=0;i<CmdNhbrTbl::iSENDERS;i++) {
        msg->setStatsIDs(i, statsIDs[i] );
        msg->setStats(i, stats[i] );
    }

    return msg;
}

//-----------------------------------------------------------------------------

CmdNhbrTbl* Node::CmdTbl() {
    return &_cmdTbl;
}

//-----------------------------------------------------------------------------

void Node::updateDisplay() {

    char buf[40];
    sprintf(buf, "id:%d|s:%ld|r:%ld", getId(), _sent, _received);
    getDisplayString().setTagArg("t",0,buf);

}//updateDisplay

//-----------------------------------------------------------------------------

void Node::finish() {
    EV << "[FINISH Node::finish() id:"<< getId() <<"]\n";

    CmdTbl()->displayOmnet();
    CmdTbl()->displayStatsOmnet();
}

//-----------------------------------------------------------------------------
