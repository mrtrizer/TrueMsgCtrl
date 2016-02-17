#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <assert.h>
#include <algorithm>
#include <cstring>
#include "msgtemplates.h"

class AbstractMsgHandler
{
public:
    virtual bool isValidCmd(const char * data, unsigned int size) const = 0;
    virtual int procCmd(const char *data, unsigned int size) = 0;
    virtual char * getRespData() = 0;
    virtual unsigned int getRespLen() = 0;

    virtual AbstractMsgHandler * newCopy() const = 0;

    void setId(const char* idSrc)
    {
        assert(idSize);
        if(!nullptr)
            id = (char*)realloc(id,idSize);
        memcpy(id,idSrc,idSize);
    }

    unsigned int getIdSize()const {return idSize;}

    virtual ~AbstractMsgHandler(){}
protected:
    char* id = nullptr;
    unsigned int idSize = 0;
};

template <class MsgType, class MsgRespType, typename Context>
class MsgHandler:public AbstractMsgHandler
{
public:
    typedef int (*MsgProcFunc)(const char * data, char * resp, const Context & context);
    typedef int (*MsgProcFuncNC)(const char * data, char * resp);

    MsgHandler(MsgProcFunc msgProcFunc, const Context & context):msgProcFunc((void*)msgProcFunc),context(context),needContext(true){}
    MsgHandler(MsgProcFuncNC msgProcFunc):msgProcFunc((void*)msgProcFunc),needContext(false){}
    ~MsgHandler()
    {
        if(id)
            free(id);
        if(respData)
            free(respData);
    }
    bool isValidCmd(const char *data, unsigned int size) const
    {
        assert(size > 0);
        data+=idSize;
        return (data[0] == MsgType::getCode()) && (size == MsgType::getSize()) &&
                (!idSize || (idSize && memcmp(data-idSize,this->id,idSize) == 0));
    }
    int procCmd(const char *data, unsigned int size)
    {
        assert(size == MsgType::getSize());
        data+=idSize;
        if (needContext)
            return ((MsgProcFunc)msgProcFunc)(data, (char*)&resp, context);
        else
            return ((MsgProcFuncNC)msgProcFunc)(data, (char*)&resp);
    }
    MsgRespType getResp(){return resp;}
    char * getRespData()
    {
        if(idSize)
        {
            respData = (char*)realloc(respData,getRespLen());
            memcpy(respData,id,idSize);
            memcpy(respData+idSize,&resp,sizeof(resp));
            return respData;
        }
        else
            return (char *)&resp;
    }
    unsigned int getRespLen(){return sizeof(resp)+idSize;}
    AbstractMsgHandler * newCopy() const {return new MsgHandler<MsgType, MsgRespType, Context>(*this);}

private:
    void * msgProcFunc;
    MsgRespType resp;
    Context context;
    bool needContext;
    char *respData = nullptr;
};

#define NC int
#define EC 0
#define MSG_HANDLER(in,out,contextType,...) MsgHandler<in, out, contextType>(__VA_ARGS__)

#endif // MSGHANDLER_H
