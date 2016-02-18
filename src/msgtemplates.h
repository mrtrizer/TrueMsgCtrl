#ifndef MSGTEMPLATES_H
#define MSGTEMPLATES_H

#include <inttypes.h>
#include <cstring>
#include <algorithm>

class AMsgType
{
public:
    AMsgType(){}
    virtual ~AMsgType(){}
    virtual char* getData() = 0;
    virtual unsigned int getSize() const= 0;
};

///New MsgType class
template <class T, int MSG_TYPE>
class __attribute__((packed)) MsgType : public AMsgType {
public:
    MsgType(const T & data):type(MSG_TYPE),data(data){}
    MsgType():type(MSG_TYPE){}
    ~MsgType()
    {
        if(dataBuf)
            free(dataBuf);
    }

    int type;
    T data;
    char* dataBuf = nullptr;
    char * getData()
    {
        if(!dataBuf)
        {
            dataBuf =(char*) malloc(sizeof(type)+sizeof(T));
            memcpy(dataBuf,&type,sizeof(type));
            memcpy(dataBuf+sizeof(type),&data,sizeof(T));
        }
        return dataBuf;
    }
    virtual unsigned int getSize() const override{return getSizeS();}
    static unsigned int getSizeS(){return sizeof(MsgType<T,MSG_TYPE>);}
    static inline unsigned char getCode(){return MSG_TYPE;}
};
#define MSG_TYPE(msg_name,number,data) \
struct __attribute__((packed)) msg_name ## _T data ; \
typedef MsgType< msg_name ## _T, number> msg_name;

#define MSG_START(name)\
    class __attribute__((packed)) name\
    {\
    public:

#define MSG_END\
    };

#define MSG_TYPE_HEADER(code) \
    type(code)

#define MSG_TYPE_BODY(name,code) \
    char * getData(){return (char *)this;}\
    static unsigned int getSize(){return sizeof(name);}\
    static inline unsigned char getCode(){return code;}\
    uint8_t type;

#define MSG_TYPE_0(name, code) \
    MSG_START(name)\
        name():MSG_TYPE_HEADER(code){}\
        MSG_TYPE_BODY(name,code)\
    MSG_END

#define MSG_TYPE_1(name, code, type1, arg1, def1) \
    MSG_START(name)\
        name(type1 arg1 = def1):MSG_TYPE_HEADER(code),arg1(arg1){}\
        MSG_TYPE_BODY(name,code)\
        type1 arg1;\
    MSG_END

#define MSG_TYPE_2(name, code, type1, arg1, def1, type2, arg2, def2) \
    MSG_START(name)\
        name(type1 arg1 = def1, type2 arg2 = def2):MSG_TYPE_HEADER(code),arg1(arg1),arg2(arg2){}\
        MSG_TYPE_BODY(name,code)\
        type1 arg1;\
        type2 arg2;\
    MSG_END


#endif // MSGTEMPLATES_H

