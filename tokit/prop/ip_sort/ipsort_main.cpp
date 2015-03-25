///<------------------------------------------------------------------------------
//< @file:   ip_main.cpp
//< @author: hongkunan
//< @date:   2013年12月14日 21:13:20
//< @brief:  
//< Copyright (c) 2013 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include <string>
#include <fstream>
#include <iostream>

#include <echoutil.h>
#include <file_util.h>
#include <tickutil.h>
#include <algorithm>

#include <list>

using namespace std;

typedef unsigned int uint32_t;

void read_whole_file(const string &iptxt, char **buffer, long &size)
{
    filebuf *pbuf;  
    ifstream filestr;  

    // 要读入整个文件，必须采用二进制打开   
    filestr.open (iptxt.c_str(), ios::binary);  
    // 获取filestr对应buffer对象的指针   
    pbuf=filestr.rdbuf();  

    // 调用buffer对象方法获取文件大小  
    size = (long)pbuf->pubseekoff (0,ios::end,ios::in);  
    pbuf->pubseekpos (0,ios::in);  

    // 分配内存空间  
    *buffer = new char[size];

    // 获取文件内容  
    pbuf->sgetn (*buffer,size);  

    filestr.close();  
}

struct ip_t
{
    uint32_t ip;
    uint32_t ip_end;
    char* start;
    char* end;

    char* addr;
};

typedef std::list<ip_t> iplist_t;
typedef std::vector<ip_t> ipvec_t;
typedef std::vector<char*> linevec_t;

uint32_t to_uint_ip(char *buf_ip)
{
    /*
    uint32_t uint_ip = inet_addr(ip);
    return uint_ip;
    */

    char ip[64] = {0};
    memcpy(ip, buf_ip, strlen(buf_ip));

    for (int j = 0; ip[j]; j++)
    {
        (ip[j] == '.') ?
            ip[j] = '\0':
            0;
    }

    char *ip1 = ip;

    int i = 0;
    for(; ip[i]; i++){}
    char *ip2 = ip + i + 1;
    i++;

    for(; ip[i]; i++){}
    char *ip3 = ip + i + 1;
    i++;

    for(; ip[i]; i++){}
    char *ip4 = ip + i + 1;
    
    uint32_t p1 = atoi(ip1);
    uint32_t p2 = atoi(ip2);
    uint32_t p3 = atoi(ip3);
    uint32_t p4 = atoi(ip4);

    uint32_t uint_ip = (p1 << 24) + (p2 << 16) + (p3 << 8);
    return uint_ip;    
}

void read_into_ipvec(char *buf, long size, ipvec_t &ipvec)
{
    for(int i = 0; i < size; i++)
    {
        buf[i] == '\n' ?
            buf[i] = '\0' :
            0;
    }

    linevec_t linevec;

    bool start = true;
    for(int i = 0; i < size; i++)
    {
        if(start)
        {
            linevec.push_back(buf + i);
            start = false;
        }

        if(buf[i] == '\0' && (i + 1) < size)
        {
            linevec.push_back(buf + i + 1);
        }
    }

    ipvec.reserve(linevec.size());
    for(size_t i = 0; i < linevec.size(); i++)
    {
        char* line = linevec[i];

        for(int j = 0; line[j]; j++)
        {
            line[j] == ' ' ?
                line[j] = '\0' :
                0;
        }

        ip_t ip;
        ip.start = line;

        int x = 0;
        for(x += 7; line[x]; x++){}
        for(; !line[x]; x++){}

        ip.end = line + x;

        for(x += 7; line[x]; x++){}
        for(; !line[x]; x++){}
        
        ip.addr = line + x;
        ip.ip = to_uint_ip(ip.start);
        ip.ip_end = to_uint_ip(ip.end);

        ipvec.push_back(ip);
    }
}

struct ipcmp_t {
    bool operator() (ip_t &ip1, ip_t &ip2) { return (ip1.ip<ip2.ip);}
} ipcmp;

void write_to_file(ipvec_t &ipvec)
{
    std::sort (ipvec.begin(), ipvec.end(), ipcmp);

    /*
    iplist_t iplist;

    for(ipvec_t::iterator itr = ipvec.begin(); itr != ipvec.end(); ++itr)
    {
        ip_t &ip = *itr;
        insert_by_ip(iplist, ip);
    }
    */

    ofstream o("sort_ip.txt");

    char buf[64] = {0};

    for(ipvec_t::iterator itr = ipvec.begin(); itr != ipvec.end(); ++itr)
    {
        ip_t &ip = *itr;

        sprintf_s(buf, sizeof(buf), "%-16s", ip.start);
        o << buf << ' ';

        sprintf_s(buf, sizeof(buf), "%-16s", ip.end);
        o << buf << ' ';
        o << ip.addr << endl;
    }
}

char* ip2addr(ipvec_t &ipvec, uint32_t ip)
{
    return NULL;
}

void readip(string &iptxt)
{
    Tick tick_now = tickutil::get_tick();

    char *buf = NULL;
    long size;

    read_whole_file(iptxt, &buf, size);

    ipvec_t ipvec;
    read_into_ipvec(buf, size, ipvec);

    write_to_file(ipvec);

    uint32_t ip = 12;
    const char* addr = ip2addr(ipvec, ip);

    dword passed_ms = tickutil::tick_diff(tick_now);
    double passed_sec = (double)passed_ms / 1000;

    std::cout << "载入" << iptxt.c_str() << "成功, 共耗时<" << passed_sec << ">秒" << std::endl;
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        ECHO_WARN("命令错误，格式应为: ");
        ECHO_WARN("   ip ip文件");

        return 0;
    }

    string iptxt = argv[1];

    if(false == fileuitl::exist(iptxt))
    {
        ECHO_ERR("参数错误: %s 路径不存在", iptxt.c_str());
        return 0;
    }

    readip(iptxt);
    system("pause");
    return 0;
}
