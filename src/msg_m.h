//
// Generated file, do not edit! Created by opp_msgc 4.2 from msg.msg.
//

#ifndef _MSG_M_H_
#define _MSG_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0402
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>msg.msg</tt> by opp_msgc.
 * <pre>
 * message Msg {
 * 
 *     
 * 
 *     int senderID    = 0;
 *     int hopCount    = 0;
 * 
 *     
 * 
 *     long commandID	= 0;
 * 
 *     
 * 
 *     
 *     int statsSenderID = 0;
 *     
 *     
 *     int statsPacketCount = 0;
 * 
 *     
 *     int statsIDs[10];
 * 
 *     
 *     int stats[10];
 * 
 * }
 * </pre>
 */
class Msg : public ::cMessage
{
  protected:
    int senderID_var;
    int hopCount_var;
    long commandID_var;
    int statsSenderID_var;
    int statsPacketCount_var;
    int statsIDs_var[10];
    int stats_var[10];

  private:
    void copy(const Msg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Msg&);

  public:
    Msg(const char *name=NULL, int kind=0);
    Msg(const Msg& other);
    virtual ~Msg();
    Msg& operator=(const Msg& other);
    virtual Msg *dup() const {return new Msg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getSenderID() const;
    virtual void setSenderID(int senderID);
    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
    virtual long getCommandID() const;
    virtual void setCommandID(long commandID);
    virtual int getStatsSenderID() const;
    virtual void setStatsSenderID(int statsSenderID);
    virtual int getStatsPacketCount() const;
    virtual void setStatsPacketCount(int statsPacketCount);
    virtual unsigned int getStatsIDsArraySize() const;
    virtual int getStatsIDs(unsigned int k) const;
    virtual void setStatsIDs(unsigned int k, int statsIDs);
    virtual unsigned int getStatsArraySize() const;
    virtual int getStats(unsigned int k) const;
    virtual void setStats(unsigned int k, int stats);
};

inline void doPacking(cCommBuffer *b, Msg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Msg& obj) {obj.parsimUnpack(b);}


#endif // _MSG_M_H_
