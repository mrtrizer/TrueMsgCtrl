#ifndef MSGCONTAINER
#define MSGCONTAINER

#include "msghandler.h"

class AbstractMsgContainer
{
public:
    AbstractMsgContainer(AbstractMsgHandler * respHandler):respHandler(respHandler->newCopy()){}
    virtual ~AbstractMsgContainer(){}
    virtual char * getData() = 0;
    virtual unsigned int getSize() = 0;
    virtual AbstractMsgContainer * newCopy() const = 0;
    void procResp(const char * data, unsigned int size)
    {
        if (respHandler->isValidCmd(data,size))
            respHandler->procCmd(data,size);
    }

private:
    AbstractMsgHandler * respHandler;
};

template <class MsgType>
class MsgContainer:public AbstractMsgContainer
{
public:

    MsgContainer(MsgType msg, AbstractMsgHandler * respHandler):AbstractMsgContainer(respHandler), msg(msg){}
    ~MsgContainer(){}
    char * getData() {return msg.getData();}
    unsigned int getSize() {return msg.getSize();}

    AbstractMsgContainer * newCopy() const {return new MsgContainer(*this);}

private:
    MsgType msg;
};

#endif // MSGCONTAINER

