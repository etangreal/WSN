//
// Generated file, do not edit! Created by opp_msgc 4.2 from msg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "msg_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(Msg);

Msg::Msg(const char *name, int kind) : cMessage(name,kind)
{
    this->senderID_var = 0;
    this->hopCount_var = 0;
    this->commandID_var = 0;
    this->statsSenderID_var = 0;
    this->statsPacketCount_var = 0;
    for (unsigned int i=0; i<10; i++)
        this->statsIDs_var[i] = 0;
    for (unsigned int i=0; i<10; i++)
        this->stats_var[i] = 0;
}

Msg::Msg(const Msg& other) : cMessage(other)
{
    copy(other);
}

Msg::~Msg()
{
}

Msg& Msg::operator=(const Msg& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void Msg::copy(const Msg& other)
{
    this->senderID_var = other.senderID_var;
    this->hopCount_var = other.hopCount_var;
    this->commandID_var = other.commandID_var;
    this->statsSenderID_var = other.statsSenderID_var;
    this->statsPacketCount_var = other.statsPacketCount_var;
    for (unsigned int i=0; i<10; i++)
        this->statsIDs_var[i] = other.statsIDs_var[i];
    for (unsigned int i=0; i<10; i++)
        this->stats_var[i] = other.stats_var[i];
}

void Msg::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->senderID_var);
    doPacking(b,this->hopCount_var);
    doPacking(b,this->commandID_var);
    doPacking(b,this->statsSenderID_var);
    doPacking(b,this->statsPacketCount_var);
    doPacking(b,this->statsIDs_var,10);
    doPacking(b,this->stats_var,10);
}

void Msg::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->senderID_var);
    doUnpacking(b,this->hopCount_var);
    doUnpacking(b,this->commandID_var);
    doUnpacking(b,this->statsSenderID_var);
    doUnpacking(b,this->statsPacketCount_var);
    doUnpacking(b,this->statsIDs_var,10);
    doUnpacking(b,this->stats_var,10);
}

int Msg::getSenderID() const
{
    return senderID_var;
}

void Msg::setSenderID(int senderID)
{
    this->senderID_var = senderID;
}

int Msg::getHopCount() const
{
    return hopCount_var;
}

void Msg::setHopCount(int hopCount)
{
    this->hopCount_var = hopCount;
}

long Msg::getCommandID() const
{
    return commandID_var;
}

void Msg::setCommandID(long commandID)
{
    this->commandID_var = commandID;
}

int Msg::getStatsSenderID() const
{
    return statsSenderID_var;
}

void Msg::setStatsSenderID(int statsSenderID)
{
    this->statsSenderID_var = statsSenderID;
}

int Msg::getStatsPacketCount() const
{
    return statsPacketCount_var;
}

void Msg::setStatsPacketCount(int statsPacketCount)
{
    this->statsPacketCount_var = statsPacketCount;
}

unsigned int Msg::getStatsIDsArraySize() const
{
    return 10;
}

int Msg::getStatsIDs(unsigned int k) const
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    return statsIDs_var[k];
}

void Msg::setStatsIDs(unsigned int k, int statsIDs)
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    this->statsIDs_var[k] = statsIDs;
}

unsigned int Msg::getStatsArraySize() const
{
    return 10;
}

int Msg::getStats(unsigned int k) const
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    return stats_var[k];
}

void Msg::setStats(unsigned int k, int stats)
{
    if (k>=10) throw cRuntimeError("Array of size 10 indexed by %lu", (unsigned long)k);
    this->stats_var[k] = stats;
}

class MsgDescriptor : public cClassDescriptor
{
  public:
    MsgDescriptor();
    virtual ~MsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MsgDescriptor);

MsgDescriptor::MsgDescriptor() : cClassDescriptor("Msg", "cMessage")
{
}

MsgDescriptor::~MsgDescriptor()
{
}

bool MsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Msg *>(obj)!=NULL;
}

const char *MsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount(object) : 7;
}

unsigned int MsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *MsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "senderID",
        "hopCount",
        "commandID",
        "statsSenderID",
        "statsPacketCount",
        "statsIDs",
        "stats",
    };
    return (field>=0 && field<7) ? fieldNames[field] : NULL;
}

int MsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderID")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "commandID")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "statsSenderID")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "statsPacketCount")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "statsIDs")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "stats")==0) return base+6;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "long",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : NULL;
}

const char *MsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Msg *pp = (Msg *)object; (void)pp;
    switch (field) {
        case 5: return 10;
        case 6: return 10;
        default: return 0;
    }
}

std::string MsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Msg *pp = (Msg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSenderID());
        case 1: return long2string(pp->getHopCount());
        case 2: return long2string(pp->getCommandID());
        case 3: return long2string(pp->getStatsSenderID());
        case 4: return long2string(pp->getStatsPacketCount());
        case 5: return long2string(pp->getStatsIDs(i));
        case 6: return long2string(pp->getStats(i));
        default: return "";
    }
}

bool MsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Msg *pp = (Msg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSenderID(string2long(value)); return true;
        case 1: pp->setHopCount(string2long(value)); return true;
        case 2: pp->setCommandID(string2long(value)); return true;
        case 3: pp->setStatsSenderID(string2long(value)); return true;
        case 4: pp->setStatsPacketCount(string2long(value)); return true;
        case 5: pp->setStatsIDs(i,string2long(value)); return true;
        case 6: pp->setStats(i,string2long(value)); return true;
        default: return false;
    }
}

const char *MsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<7) ? fieldStructNames[field] : NULL;
}

void *MsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Msg *pp = (Msg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


