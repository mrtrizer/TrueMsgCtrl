#ifndef MSGCONTAINER
#define MSGCONTAINER

class AbstractMsgContainer
{
public:
    virtual ~AbstractMsgContainer(){}
    virtual char * getData() = 0;
    virtual unsigned int getSize() = 0;
};

template <class MsgType>
class MsgContainer:AbstractMsgContainer
{
public:
    MsgContainer(MsgType msg):msg(msg){}
    ~MsgContainer(){}
    char * getData() {return msg.getData();}
    unsigned int getSize() {return msg.getSize();}

private:
    MsgType msg;
};

#endif // MSGCONTAINER

