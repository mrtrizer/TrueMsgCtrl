#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "assert.h"
#include "msgtemplates.h"

class AbstractMsgHandler
{
public:
    virtual bool isValidCmd(char * data, unsigned int size) const = 0;
    virtual ~AbstractMsgHandler(){}
};

template <class MsgType, class MsgRespType>
class MsgHandler:AbstractMsgHandler
{
public:
    typedef int (*MsgProcFuc)(char * data, char * resp);

    MsgHandler(MsgProcFuc msgProcFunc):msgProcFunc(msgProcFunc){}
    ~MsgHandler(){}
    bool isValidCmd(char *data, unsigned int size) const
    {
        assert(size > 0);
        return (data[0] == MsgType::getCode()) && (size == MsgType::getSize());
    }
    MsgRespType procCmd(char *data, unsigned int size)
    {
        msgProcFunc(data, (char*)&resp);
        return resp;
    }
private:
    MsgProcFuc msgProcFunc;
    MsgRespType resp;
};

#endif // MSGHANDLER_H
