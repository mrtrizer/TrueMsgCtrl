#ifndef MSGCONTAINER
#define MSGCONTAINER

#include "msghandler.h"

class AbstractMsgContainer
{
public:
    AbstractMsgContainer(const AbstractMsgHandler & respHandler):respHandler(respHandler.newCopy()){}
    AbstractMsgContainer(const AbstractMsgContainer & instance){this->respHandler = instance.respHandler->newCopy();}
    virtual ~AbstractMsgContainer(){delete respHandler;}
    virtual char * getData() = 0;
    virtual unsigned int getSize() const = 0;
    virtual uint8_t getType() const = 0;
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
    unsigned int getSize() const {return msg.getSize();}
    uint8_t getType() const {return msg.type;}

    AbstractMsgContainer * newCopy() const {return new MsgContainer<MsgType>(*this);}

private:
    MsgType msg;
};

#define MSG(type,proc,...) MsgContainer<type>(type(__VA_ARGS__),proc)

#endif // MSGCONTAINER

