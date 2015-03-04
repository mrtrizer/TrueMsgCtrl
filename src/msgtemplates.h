#ifndef MSGTEMPLATES_H
#define MSGTEMPLATES_H

#include <inttypes.h>

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

