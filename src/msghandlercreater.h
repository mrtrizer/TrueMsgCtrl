#ifndef MSGHANDLERCREATER_H
#define MSGHANDLERCREATER_H
#include "msghandler.h"

class MsgHandlerCreater
{
    typedef int (*MsgProcFuncNC)(const char * data, char * resp);
public:
    MsgHandlerCreater(unsigned int idSize):
    idSize(idSize)
    {
    }

    template <class MsgType, class MsgRespType, typename Context>
    MsgHandler<MsgType,MsgRespType,Context>* createHandler(MsgProcFuncNC msgProcFunc, bool setId = true)
    {
        auto handler =new MsgHandler<MsgType,MsgRespType,Context>(msgProcFunc);
        handler->setIdSize(idSize);
        if(setId)
        {
            handler->setId((char*)msgId);
            msgId++;
        }
        return handler;
    }

    template <class MsgType, class MsgRespType, typename Context>
    MsgHandler<MsgType,MsgRespType,Context>* createHandler(
            int (*msgProcFunc)(const char*,char*, const Context)&, const Context & context, bool setId = true)
    {
        auto handler = MsgHandler<MsgType,MsgRespType,Context>(msgProcFunc,context);
        handler->setIdSize(idSize);
        if(setId)
        {
            handler->setId((char*)msgId);
            msgId++;
        }
        return handler;
    }

private:
    unsigned int idSize = 0;
    unsigned int msgId = 0;
};

#endif // MSGHANDLERCREATER_H
