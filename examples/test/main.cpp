#include <iostream>
#include <string>
#include <sstream>
#include "msgtemplates.h"
#include "msghandler.h"
#include "msgcontainer.h"

using namespace std;

//ШАГ 0: Описание типов команд.
//Формат: MSG_TYPE_<кол-во аргументов>(<название>,<номер>[,<тип аргумента>,<имя аргумента>,<значение по-умолчанию>[, ...]])
MSG_TYPE(TestReq,1,{unsigned int a; unsigned int b;}) //Запрос
MSG_TYPE(TestResp,2,{unsigned int a;}) //Ответ
MSG_TYPE(StatResp,2,{unsigned int a;}) //Ответ

string hexDump(const char * data, int size) 
{
	stringstream stream;
	for (int i = 0; i < size; i++) 
	{
		stream << hex << (unsigned int)data[i];
		if ((i+1) % 16 == 0)
			stream << endl;
	}
	return stream.str();
}

//Функция обработки запроса. Сюда передается указатель на запрос и на ответ.
//Т.к нам заранее известно, какую команду мы обрабатывает и что отправляем в ответ,
//мы можем воспользоваться приведением типа. (1) Теперь мы имеем доступ к полям
//структуры команды.
int cmdProcFunc(const char * data, char * resp)
{
    cout << "Request processing. " << hexDump(data,TestReq::getSize()) << endl;
    TestResp * respTmp = (TestResp*) resp; //(1)
    TestReq * testTmp = (TestReq*) data;
    respTmp->data.a = testTmp->data.a + testTmp->data.b;
    return 0;
}

//Функция обработки ответа. Идея та же что описана выше.
int respProcFunc(const char * data, char * resp)
{
    cout << "Response processing. Response:" << hexDump(data,TestResp::getSize()) << endl;
    return 0;
}


//Для тестирования я не разделяю код на клиент и сервер. Но, что бы ориентироваться:
//Запросы - отправляет клиент -> серверу.
//Ответы - отвечает сервер -> клиенту.
int main(int argc, char *argv[])
{
    //ШАГ 1: Создание экземпляра запроса. (см. шаг 0)
    TestReq request({0x1,0x1});
    cout << "Request: " << hexDump(request.getData(),request.getSize()) << endl;

    //ШАГ 2: Создание обработчика запроса. Обратите внимание, обработчик запроса
    //создается один раз и должен жить пока вы собираетесь принимать соответствующий тип команд.
    MsgHandler<TestReq,TestResp,int> handler(cmdProcFunc); //Обработчик запроса

    //ШАГ 4: Создание обработчика ответа
    MsgHandler<TestResp,StatResp,int> msgHandler(respProcFunc); //Обработчик ответа

    //ШАГ 5: Создание контейнера команды. Содержит запрос и обработчик ответа.
    MsgContainer<TestReq> container(request, msgHandler); //Контейнер для запроса. Он содержит команду и обработчик ответа.

    //ШАГ 6: Когда мы получаем запросы, мы должны пройтись по обработчикам и скормить команду первому
    //согласившимуся обработчику. После этого будет вызвана функция обработки.
    //Эту проверку (4) можно выполнять в цикле для обработчиков всех типов команд.
    //Проверяется соответствие длины команды и формата.
    if (handler.isValidCmd(request.getData(),request.getSize())) //(4)
    {
        //Обработка данных, ответ и его длинну можно прочитать из методов getRespData() (2) и getRespLen() (3)
        handler.procCmd(request.getData(),request.getSize());
        char * respData = handler.getRespData(); //(2)        
        unsigned int respLen = handler.getRespLen(); //(3)
        cout << "Response: "<< hexDump(respData,respLen) << endl;
    }

    //ШАГ 7: Обработка ответа. На шаге 6 мы обработали команду и ответили. Ответ нужно обработать.
    TestResp response = handler.getResp();
    container.procResp(response.getData(),response.getSize());

    //ШАГ 8, если нужно. После обработки ответа у нас еще есть подтверждение обработки ответа.
    //получить доступ к нему можно так:
    //container.getRespHandler().getRespData();zzz

    return 0;
}
