#ifndef MSGCONTAINER
#define MSGCONTAINER

#include "msghandler.h"

class AbstractMsgContainer
{
public:
    AbstractMsgContainer(const AbstractMsgHandler & respHandler):respHandler(respHandler.newCopy()){}
    virtual ~AbstractMsgContainer(){}
    virtual char * getData() = 0;
    virtual unsigned int getSize() = 0;
    virtual AbstractMsgContainer * newCopy() const = 0;
    void procResp(const char * data, unsigned int size)
    {
        if (respHandler->isValidCmd(data,size))
            respHandler->procCmd(data,size);
    }

    AbstractMsgHandler * getRespHandler(){return respHandler;}

private:
    AbstractMsgHandler * respHandler;
};

template <class MsgType>
class MsgContainer:public AbstractMsgContainer
{
public:

    MsgContainer(MsgType msg, const AbstractMsgHandler & respHandler):AbstractMsgContainer(respHandler), msg(msg){}
    ~MsgContainer(){}
    char * getData() {return msg.getData();}
    unsigned int getSize() {return msg.getSize();}

    AbstractMsgContainer * newCopy() const {return new MsgContainer(*this);}

private:
    MsgType msg;
};

#define MSG(type,proc,...) MsgContainer<type>(type(__VA_ARGS__),proc)
#define MSG_A(type,respProc,stat,...) MSG(type##Req, (MsgHandler<type##Resp,stat>(respProc)),__VA_ARGS__)

#endif // MSGCONTAINER

