#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "assert.h"
#include "msgtemplates.h"

class AbstractMsgHandler
{
public:
    virtual bool isValidCmd(const char * data, unsigned int size) const = 0;
    virtual int procCmd(const char *data, unsigned int size) = 0;
    virtual AbstractMsgHandler * newCopy() = 0;
    virtual ~AbstractMsgHandler(){}
};

template <class MsgType, class MsgRespType>
class MsgHandler:public AbstractMsgHandler
{
public:
    typedef int (*MsgProcFuc)(const char * data, char * resp);

    MsgHandler(MsgProcFuc msgProcFunc):msgProcFunc(msgProcFunc){}
    ~MsgHandler(){}
    bool isValidCmd(const char *data, unsigned int size) const
    {
        assert(size > 0);
        return (data[0] == MsgType::getCode()) && (size == MsgType::getSize());
    }
    int procCmd(const char *data, unsigned int size)
    {
        assert(size == MsgType::getSize());
        return msgProcFunc(data, (char*)&resp);
    }
    inline MsgRespType getResp(){return resp;}
    AbstractMsgHandler * newCopy() {return new MsgHandler<MsgType, MsgRespType>(*this);}

private:
    MsgProcFuc msgProcFunc;
    MsgRespType resp;
};

#endif // MSGHANDLER_H
