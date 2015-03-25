#include "stringconversion.h"
#include "poolbase.h"
#include "tick_tool.h"

#include <iostream>
#include <list>
#include "timetools.h"
#include "singleton.h"

using namespace std;

class OnlyOne : public Singleton<OnlyOne>
{
public:
    OnlyOne(){

    }

    ~OnlyOne(){
        //std::cout << "~Element" << std::endl;
    }

    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    std::string msg1;
    std::string msg2;
    std::string msg3;
    std::string msg4;
    std::string msg5;
};

class Element : public PoolBase<Element, 100>
{
public:
    Element(){

    }

    ~Element(){
        //std::cout << "~Element" << std::endl;
    }

    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    std::string msg1;
    std::string msg2;
    std::string msg3;
    std::string msg4;
    std::string msg5;
};

class ElementNoPool
{
public:
    ElementNoPool(){

    }

    ~ElementNoPool(){
        //std::cout << "~ElementNoPool" << std::endl;
    }

    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    std::string msg1;
    std::string msg2;
    std::string msg3;
    std::string msg4;
    std::string msg5;
};

int main(int argc, char **argv)
{
    cout<<"               ★★★★★★★★★★★★★★★★★★★★★★★★★"<<endl;
    cout<<"               ★                                              ★"<<endl;
    cout<<"               ★                Test                          ★"<<endl;
    cout<<"               ★      Copyright (c) 2014, 测试                ★"<<endl;
    cout<<"               ★             All rights reserved.             ★"<<endl;
    cout<<"               ★                                              ★"<<endl;
    cout<<"               ★★★★★★★★★★★★★★★★★★★★★★★★★\n"<<endl;
    cout<<"★★★★★★★★Initialize Test★★★★★★★★"<<endl<<endl;

    {
        {
            std::vector<Element*> objs;

            tick_t pool_tick("对象池分配对象消耗时间");
            for(int times = 0; times < 1; times++){
                for(int i = 0; i < 100; i++){
                    Element *pElem = new Element;
                    objs.push_back(pElem);
                }

                for(size_t i = 0; i < objs.size(); i++){
                    delete objs[i];
                }

                objs.clear();
            }
        }

        {
            std::vector<ElementNoPool*> objs;

            tick_t pool_tick("无对象池分配对象消耗时间");
            for(int times = 0; times < 1; times++){
                for(int i = 0; i < 100; i++){
                    ElementNoPool *pElem = new ElementNoPool;
                    objs.push_back(pElem);
                }
            }

            for(size_t i = 0; i < objs.size(); i++){
                delete objs[i];
            }

            objs.clear();
        }
    }

    {
        int max_times = 1000;
        {
            tick_t pool_tick("获取当前时间无缓存");
            for(int i = 0; i < max_times; i++){
                time64_t time = TimeTools::getCurrentTime();
                TimeTools::getTMFromTime_t(time);
            }
        }

        {
            time64_t cache_time = TimeTools::getCurrentTime();

            tick_t pool_tick("从缓存获取当前时间");
            for(int i = 0; i < max_times; i++){
                time64_t time = cache_time;
                TimeTools::getTMFromTime_t(time);
            }
        }
    }

    {
        OnlyOne::Instance();
        static OnlyOne s_onlyOne;

        int max_times = 10000000;
        {

            tick_t pool_tick("单例");
            for(int i = 0; i < max_times; i++){
                OnlyOne &one = OnlyOne::Instance();
            }
        }

        {
            tick_t pool_tick("静态全局");
            for(int i = 0; i < max_times; i++){
                OnlyOne &one = s_onlyOne;
            }
        }
    }
    
    return 0;
}
