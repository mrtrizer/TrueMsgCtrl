#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>
#include "msgtemplates.h"
#include "msghandler.h"
#include "msgcontainer.h"

//ШАГ 0: Описание типов команд.
//Формат: MSG_TYPE_<кол-во аргументов>(<название>,<номер>[,<тип аргумента>,<имя аргумента>,<значение по-умолчанию>[, ...]])
MSG_TYPE_2(TestReq,1,unsigned int, a, 0, unsigned int, b, 0) //Запрос
MSG_TYPE_1(TestResp,2,unsigned int, a, 0) //Ответ

MSG_TYPE_1(StatResp,100,unsigned char, a, 0) //Потвержение ответа. Этот тип может быть общим для всех типов команд.

//Функция обработки запроса. Сюда передается указатель на запрос и на ответ.
//Т.к нам заранее известно, какую команду мы обрабатывает и что отправляем в ответ,
//мы можем воспользоваться приведением типа. (1) Теперь мы имеем доступ к полям
//структуры команды.
int cmdProcFunc(const char * data, char * resp)
{
    qDebug() << "Req processing. " << QByteArray(data,TestReq::getSize()).toHex();
    TestResp * respTmp = (TestResp*) resp; //(1)
    TestReq * testTmp = (TestReq*) data;
    respTmp->a = testTmp->a + testTmp->b;
    return 0;
}

//Функция обработки ответа. Идея та же что описана выше.
int respProcFunc(const char * data, char * resp)
{
    qDebug() << "Resp processing. Resp:" << QByteArray(data,TestResp::getSize()).toHex();
    StatResp * tmpResp = (StatResp*)resp; //(1)
    tmpResp->a = 0;
    return 0;
}


//Для тестирования я не разделяю код на клиент и сервер. Но, что бы ориентироваться:
//Запросы - отправляет клиент -> серверу.
//Ответы - отвечает сервер -> клиенту.
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //ШАГ 1: Создание экземпляра запроса. (см. шаг 0)
    TestReq b(0x1,0x1);
    qDebug() << QByteArray(b.getData(),b.getSize()).toHex();

    //ШАГ 2: Создание обработчика запроса. Обратите внимание, обработчик запроса
    //создается один раз и должен жить пока вы собираетесь принимать соответствующий тип команд.
    MsgHandler<TestReq,TestResp> handler(cmdProcFunc); //Обработчик запроса

    //ШАГ 3: Создание экземпляра ответа
    TestResp c;

    //ШАГ 4: Создание обработчика ответа
    MsgHandler<TestResp,StatResp> msgHandler(respProcFunc); //Обработчик ответа

    //ШАГ 5: Создание контейнера команды. Содержит запрос и обработчик ответа.
    MsgContainer<TestReq> container(b, &msgHandler); //Контейнер для запроса. Он содержит команду и обработчик ответа.

    //ШАГ 6: Когда мы получаем запросы, мы должны пройтись по обработчикам и скормить команду первому
    //согласившимуся обработчику. После этого будет вызвана функция обработки.
    //Эту проверку (4) можно выполнять в цикле для обработчиков всех типов команд.
    //Проверяется соответствие длины команды и формата.
    if (handler.isValidCmd(container.getData(),container.getSize())) //(4)
    {
        //Обработка данных, ответ и его длинну можно прочитать из методов getRespData() (2) и getRespLen() (3)
        handler.procCmd(b.getData(),b.getSize());
        char * respData = handler.getRespData(); //(2)
        unsigned int respLen = handler.getRespLen(); //(3)
        qDebug() << "Resp: "<< QByteArray(respData,respLen).toHex();
    }

    //ШАГ 7: Обработка ответа. На шаге 6 мы обработали команду и ответили. Ответ нужно обработать.
    container.procResp(c.getData(),c.getSize());

    //ШАГ 8, если нужно. После обработки ответа у нас еще есть подтверждение обработки ответа.
    //получить доступ к нему можно так:
    //container.getRespHandler().getRespData();

    return a.exec();
}
