#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>
#include "msgtemplates.h"
#include "msghandler.h"
#include "msgcontainer.h"

MSG_TYPE_2(Test,1,unsigned int, a, 0, unsigned int, b, 0)
MSG_TYPE_1(TestResp,2,unsigned int, a, 0)

int cmdProcFunc(char * data, char * resp)
{
    TestResp * respTmp = (TestResp*) resp;
    Test * testTmp = (Test*) data;
    respTmp->a = testTmp->a + testTmp->b;
    return 0;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Test b(0x1,0x1);
    qDebug() << QByteArray(b.getData(),b.getSize()).toHex();

    MsgHandler<Test,TestResp> handler(cmdProcFunc);

    TestResp c;

    MsgContainer<Test> container(b);

    if (handler.isValidCmd(container.getData(),container.getSize()))
    {
        c = handler.procCmd(b.getData(),b.getSize());
        qDebug() << QByteArray(c.getData(),c.getSize()).toHex();
    }

    return a.exec();
}
