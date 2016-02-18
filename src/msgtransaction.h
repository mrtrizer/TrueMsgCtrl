#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "msgtemplates.h"
#include <algorithm>
#include <cstring>
#include <assert.h>

class MsgTransaction
{
public:
    MsgTransaction(char* id = nullptr, unsigned int idSize = 0):
        idSize(idSize),
        id(nullptr)
    {
        setId(id);
    }
    ~MsgTransaction()
    {
        if(respData)
            free(respData);
        if(reqData)
            free(reqData);
        if(id)
            free(id);
    }

    const char* getId()const{return id;}
    const char* getRespDataWithId()
    {
        if(respData) return respData;
        return getDataWithId(resp,respData);
    }
    const char* getRespData()
    {
        if(reqData) return reqData;
        return resp->getData();
    }
    const char* getReqData()
    {
        return req->getData();
    }

    const char* getReqDataWithId()
    {
        return getDataWithId(req,reqData);
    }


    void setId(const char* data)
    {
        if(idSize)
        {
            id = (char*)realloc(id,idSize);
            memcpy(id,data,idSize);
        }
    }
    bool checkId(char* id)
    {
        assert(idSize);
        return (0 == memcmp(this->id,id,idSize));
    }

    void setReq(AMsgType *value){req = value;}

    void setResp(AMsgType *value){resp = value;}
    void setReq(const char *value, unsigned int size)
    {
        setData(value,reqData,size);
        reqDataSize = size +idSize;
    }

    void setResp(const char *value, unsigned int size)
    {
        setData(value,respData,size);
        respDataSize = size +idSize;
    }

    AMsgType *getReq() const
    {
        return req;
    }

    AMsgType *getResp() const
    {
        return resp;
    }

    unsigned int getReqSize()
    {
        if(req)
            return sizeof(*req);
        else return reqDataSize;
    }
    unsigned int getRespSize()
    {
        if(resp)
            return sizeof(*resp);
        else return respDataSize;
    }

    void setIdSize(unsigned int size)
    {
        idSize = size;
    }

    unsigned int getIdSize()const{return idSize;}
private:
    const char* getDataWithId(AMsgType* src, char* dest)
    {
        if(idSize)
        {
            dest = (char*)realloc(dest,src->getSize()+idSize);
            memcpy(dest,id,idSize);
            memcpy(dest+idSize,src->getData(),src->getSize());
            return dest;
        }
        else
            return src->getData();
    }

    void setData(const char *src, char* dest, unsigned int size)
    {
        assert(!dest);
        dest = (char*)malloc(size+idSize);
        memcpy(dest,id,idSize);
        memcpy(dest+idSize,src,size);
    }


    AMsgType* req;
    AMsgType* resp;
    char* id = nullptr;
    unsigned int idSize = 0;
    char* respData = nullptr;
    char* reqData = nullptr;
    unsigned int respDataSize = 0;
    unsigned int reqDataSize = 0;
};




#endif // TRANSACTION_H




