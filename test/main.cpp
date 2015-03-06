#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>
#include "msgtemplates.h"
#include "msghandler.h"
#include "msgcontainer.h"

MSG_TYPE_2(Test,1,unsigned int, a, 0, unsigned int, b, 0)
MSG_TYPE_1(TestResp,2,unsigned int, a, 0)
MSG_TYPE_1(StatResp,100,unsigned char, a, 0)

int cmdProcFunc(const char * data, char * resp)
{
    qDebug() << "Req processing. " << QByteArray(data,Test::getSize()).toHex();
    TestResp * respTmp = (TestResp*) resp;
    Test * testTmp = (Test*) data;
    respTmp->a = testTmp->a + testTmp->b;
    return 0;
}


int respProcFunc(const char * data, char * resp)
{
    qDebug() << "Resp processing. Resp:" << QByteArray(data,TestResp::getSize()).toHex();
    StatResp * tmpResp = (StatResp*)resp;
    tmpResp->a = 0;
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Test b(0x1,0x1);
    qDebug() << QByteArray(b.getData(),b.getSize()).toHex();

    MsgHandler<Test,TestResp> handler(cmdProcFunc);

    TestResp c;

    MsgHandler<TestResp,StatResp> msgHandler(respProcFunc);
    MsgContainer<Test> container(b, &msgHandler);

    if (handler.isValidCmd(container.getData(),container.getSize()))
    {
        handler.procCmd(b.getData(),b.getSize());
        char * respData = handler.getRespData();
        unsigned int respLen = handler.getRespLen();
        qDebug() << "Resp: "<< QByteArray(respData,respLen).toHex();
    }

    container.procResp(c.getData(),c.getSize());

    return a.exec();
}
