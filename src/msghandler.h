#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <assert.h>
#include "msgtemplates.h"

class AbstractMsgHandler
{
public:
    virtual bool isValidCmd(const char * data, unsigned int size) const = 0;
    virtual int procCmd(const char *data, unsigned int size) = 0;
    virtual char * getRespData() = 0;
    virtual unsigned int getRespLen() = 0;

    virtual AbstractMsgHandler * newCopy() const = 0;

    virtual ~AbstractMsgHandler(){}
};

template <class MsgType, class MsgRespType, typename Context>
class MsgHandler:public AbstractMsgHandler
{
public:
    typedef int (*MsgProcFunc)(const char * data, char * resp, const Context & context);
    typedef int (*MsgProcFuncNC)(const char * data, char * resp);

    MsgHandler(MsgProcFunc msgProcFunc, const Context & context):msgProcFunc((void*)msgProcFunc),context(context),needContext(true){}
    MsgHandler(MsgProcFuncNC msgProcFunc):msgProcFunc((void*)msgProcFunc),context(0),needContext(false){}
    ~MsgHandler(){}
    bool isValidCmd(const char *data, unsigned int size) const
    {
        assert(size > 0);
        return (data[0] == MsgType::getCode()) && (size == MsgType::getSize());
    }
    int procCmd(const char *data, unsigned int size)
    {
        assert(size == MsgType::getSize());
        if (needContext)
            return ((MsgProcFunc)msgProcFunc)(data, (char*)&resp, context);
        else
            return ((MsgProcFuncNC)msgProcFunc)(data, (char*)&resp);
    }
    char * getRespData(){return (char *)&resp;}
    unsigned int getRespLen(){return sizeof(resp);}
    AbstractMsgHandler * newCopy() const {return new MsgHandler<MsgType, MsgRespType, Context>(*this);}

private:
    void * msgProcFunc;
    MsgRespType resp;
    Context context;
    bool needContext;
};

#define NC int
#define EC 0
#define MSG_HANDLER(in,out,contextType,...) MsgHandler<in, out, contextType>(__VA_ARGS__)

#endif // MSGHANDLER_H
