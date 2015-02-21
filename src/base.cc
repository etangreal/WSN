
//-----------------------------------------------------------------------------
// Author: Ernst Salzmann
// Date: 16 Sept 2012
// Basestation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------

#include <omnetpp.h>
#include <stdio.h>
#include <string.h>

// Include a generated file: the header file created from Msg.msg.
// It contains the definition of the Msg class, derived from
// cMessage.
#include "msg_m.h"

#include <CmdNhbrTbl.h>
#include <StatPacketTracker.h>

//-----------------------------------------------------------------------------
// CLASS-DEFINITION
//-----------------------------------------------------------------------------

class Base : public cSimpleModule {

  public:
    static int const NODES = 10;

  private:
    long _sent;                                 //Number of messages that have been sent by this node
    long _received;                             //Number of messages that have been received by this node
    long _cmdID;                                //whenever the basestation sends out a new command it increments _cmdID and sets it as the message's->commandID

    int _stats[NODES][NODES];                   //Table for keeping stats submitted by the nodes        
    StatPacketTracker _statsTracker;            //Sent&Recieved Stats messages(packets)                 [Tracking Table]

  public:
            Base();                             // Constuctor
            ~Base();                            //Destuctor

  protected:
    virtual void initialize();                  // used to setup/initialize variables etc ... 
            // CORE ROUTINE: handleMessage
    virtual void handleMessage(cMessage* msg);  // whenever the node receives a message it is passed to this routine
            void forwardMessage(Msg* msg);      // Sends a message to all nodes "directly attached" to the basestation
            Msg* generateMessage();             // Generates a "dummy" command for sending out onto the network
            void updateMsg(Msg* msg);           // update/prepare a message with current node info -> e.g: stamps it with the current node's ID

            // stats routines                   -> Routines that operate on _stats[NODES][NODES] Table
            void clearStats();                  // clears the _stats[NODES][NODES] Table
            void processStats(Msg* msg);        //when the basestation receives a stats packet this routine process it
            void displayStats();                //prints out a table in omnets console to display _stats[NODES][NODES]; 

            // omnet GUI
            void updateDisplay();               //displays the basestation's (id|sent|received) in omnet 

    virtual void finish();                      // omnet calls this routine after all simulated events/messages have finished
};

// The module class needs to be registered with OMNeT++
Define_Module(Base);

//-----------------------------------------------------------------------------
// FUNCTION-IMPLEMENTATIONS
//-----------------------------------------------------------------------------

Base::Base() { }

//-----------------------------------------------------------------------------

Base::~Base() { }

//-----------------------------------------------------------------------------

void Base::initialize() {

    // Initialize variables
    _sent = 0;
    _received = 0;
    _cmdID = 0;

    WATCH(_sent);
    WATCH(_received);
    WATCH(_cmdID);

    clearStats();

    // Boot the process scheduling the initial message as a self-message.
    Msg* msg = generateMessage();
    scheduleAt(0.0, msg);

    msg = generateMessage();
    scheduleAt(2.0, msg);

    msg = generateMessage();
    scheduleAt(4.0, msg);

    msg = generateMessage();
    scheduleAt(8.0, msg);

    msg = generateMessage();
    scheduleAt(10.0, msg);

    msg = generateMessage();
    scheduleAt(12.0, msg);

    msg = generateMessage();
    scheduleAt(14.0, msg);

    msg = generateMessage();
    scheduleAt(18.0, msg);

    msg = generateMessage();
    scheduleAt(20.0, msg);

    msg = generateMessage();
    scheduleAt(22.0, msg);

    msg = generateMessage();
    scheduleAt(24.0, msg);

    msg = generateMessage();
    scheduleAt(28.0, msg);

    msg = generateMessage();
    scheduleAt(30.0, msg);

    msg = generateMessage();
    scheduleAt(32.0, msg);

    msg = generateMessage();
    scheduleAt(34.0, msg);

    msg = generateMessage();
    scheduleAt(38.0, msg);

    msg = generateMessage();
    scheduleAt(40.0, msg);

    msg = generateMessage();
    scheduleAt(41.0, msg);

    msg = generateMessage();
    scheduleAt(42.0, msg);

    msg = generateMessage();
    scheduleAt(44.0, msg);

    msg = generateMessage();
    scheduleAt(48.0, msg);

    msg = generateMessage();
    scheduleAt(50.0, msg);

}//initialize

//-----------------------------------------------------------------------------

//MAIN ROUTINE: WHEN-MESSAGE-ARRIVES-AT-THE-BASE-STATION-THIS-ROUTINE IS CALLED
void Base::handleMessage(cMessage* cmsg) {

    Msg* msg = check_and_cast<Msg*>(cmsg);

    processStats(msg); //if this is a stats message it will be processed...

    EV<<"Base::handleMessage(msg) -> sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<"\t[ARRIVED].\n";
    _received++;

    // if the messages where Scheduled they will be introduced the handleMessage at some point in time...
    //  we check to see if the senderID == to the basestation's ID. 
    if( msg->getSenderID() == getId() ) { //If this is the case forward it to all nearby nodes

        forwardMessage(msg);

    } else { //else since the message is not "scheduled" or a stats message -> delete it

        EV << "Base::handleMessage(msg) -> sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<"\t[DELETED].\n";
        delete msg;
    }

    if (ev.isGUI())
        updateDisplay();

}//handleMessage

//-----------------------------------------------------------------------------

void Base::forwardMessage(Msg* msg) {

    msg->setHopCount(msg->getHopCount()+1);    
    updateMsg(msg);

    // send the msg to all
    int n = gateSize("gate");
    for(int i=0;i<n;i++) {
        EV << "Base::forwardMessage(msg) -> sndID:"<<msg->getSenderID()<<"|cmdID:"<<msg->getCommandID()<<"\t GATE[" << i << "]\n";
        _sent++;
        send(msg->dup(), "gate$o", i);
    }

    delete msg;

}//forwardMessage

//-----------------------------------------------------------------------------

Msg* Base::generateMessage() {

    _cmdID++;
    EV << "[START Base::generateMessage()] commandID:"<<_cmdID<<"\n";

    // Produce source and destination addresses.
    int sndID = getId();   // our module index

    char desc[40];
    sprintf(desc, "s:%d|c:%ld", sndID, _cmdID);

    // Create message object and set source and destination field.
    Msg* msg = new Msg(desc);
    msg->setSenderID(sndID);
    msg->setCommandID(_cmdID);

    return msg;

}//generateMessage

//-----------------------------------------------------------------------------

void Base::updateMsg(Msg* msg) {

    msg->setSenderID( getId() );

    char desc[40];
    sprintf(desc, "s:%d|c:%ld", msg->getSenderID(), msg->getCommandID());
    msg->setName(desc);
}

//-----------------------------------------------------------------------------

void Base::clearStats() {

    for(int n=0; n<NODES;n++)
        for(int m=0; m<NODES;m++)
            _stats[n][m]=0;
}

//-----------------------------------------------------------------------------

void Base::processStats(Msg* msg) {

    int statsSndID = msg->getStatsSenderID();
    int statsPktCount = msg->getStatsPacketCount();

    if (statsSndID < 0) { // just incase
        EV << "\n--------------------- INVALID STATS MESSAGE - SENDER ID < 0 !!!! --------------------- \n";
        return;
    }    

    // if the Stats-Sender-ID is 0 then its not a Stats-Packet ... return;
    // if the _statsTracker does not accept the (statsSenderID/statsPacketCount[aka stats-Packet-number]) ... return
    if (statsSndID == 0 || !_statsTracker.accept(statsSndID,statsPktCount) ) return;


    if (statsSndID >= NODES) { //no space in the table to store this senderID
        EV << "\n--------------------- ERROR ON UPDATING STATS - SENDER ID TOO BIG !!!! --------------------- \n";
        return;
    }


    EV << "\n--------------------- PROCESSING STATS  --------------------- \n";

    bubble("PROCESSING-STATS");

    for(int s=0;s<CmdNhbrTbl::iSENDERS; s++) {
        int id = msg->getStatsIDs(s);
        int stats = msg->getStats(s);

        if(id > 0)
            _stats[statsSndID][id] += stats;
    }

} //processStats

//-----------------------------------------------------------------------------

void Base::displayStats() {
    char buff[200];

    EV << "\n-------------------------------------------------------------------------------------\n";

    EV << "               recieved node stats\n";

    EV << "       ";
    for(int m=0;m<NODES;m++) {
        sprintf(buff,"{ %3d }", m); EV << buff;
    }
    EV << "\n";

    for(int n=0;n<NODES;n++) {
        sprintf(buff, "{ %3d }", n); EV << buff;

        for(int m=0;m<NODES;m++) {
            sprintf(buff, "[ %3d ]", _stats[n][m]); EV << buff;
        }
        EV << "\n";
    }

    EV << "\n-------------------------------------------------------------------------------------\n";
}

//-----------------------------------------------------------------------------

void Base::updateDisplay() {

    char buf[40];
    sprintf(buf, "id:%d|s:%ld|r:%ld", getId(), _sent, _received);
    getDisplayString().setTagArg("t",0,buf);

}//updateDisplay

//-----------------------------------------------------------------------------

void Base::finish() {
    EV << "[FINISH Base::finish()]\n";

    displayStats();
}

//-----------------------------------------------------------------------------
