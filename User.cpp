//
// Created by Barrin on 2019/12/14.
//
#include <fstream>
#include <iostream>
#include <cstring>
#include "User.h"
using namespace std;


User::User(const char *_id, const char *_pd, const char *_nm,int _level)
{
    use=true;
    level=_level;
    strcpy(id,_id);
    strcpy(pd,_pd);
    strcpy(nm,_nm);
}

void User::del()
{
    use=false;
}

bool User::cmp_id(const char *_id)
{
    return strcmp(id,_id)==0;
}

bool User::cmp_pd(const char *_pd)
{
    return strcmp(pd,_pd)==0;
}

int User::get_level()
{
    return level;
}

bool User::exist()
{
    return use;
}

/*void User::add()
{
    fstream io("user");
    io.seekp(0,ios::end);
    io.write(reinterpret_cast<const char*>(this),sizeof(*this));
    if(!io)
        puts("failfail");
    io.close();
}*/

void User::change_pd(const char *_pd)
{
    strcpy(pd,_pd);
}

void User::out()
{
    cout<<use<<' '<<level<<' '<<id<<' '<<pd<<' '<<nm<<endl;
}


