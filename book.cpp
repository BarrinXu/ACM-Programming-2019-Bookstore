//
// Created by Barrin on 2019/12/14.
//
#include <iostream>
#include <cstring>
#include "book.h"
using namespace std;
book::book(const char *_ib, const char *_nm, const char *_at, const char *_kw, double _pc, int _qt)
{
    use=true;
    pc=_pc,qt=_qt;
    strcpy(ib,_ib);
    strcpy(nm,_nm);
    strcpy(at,_at);
    strcpy(kw,_kw);

}

bool book::cmp(int id,const char *s)
{
    if(id==0)
        return true;
    if(id==1)
        return strcmp(ib,s)==0;
    if(id==2)
        return strcmp(nm,s)==0;
    if(id==3)
        return strcmp(at,s)==0;
    if(id==4)
    {
        int lens=strlen(s);
        int st=0,len=strlen(kw);
        for(int i=0; i<len;)
        {
            int j=i;
            while(j+1<len&&kw[j+1]!='|')
                j++;
            if(j-i+1==lens)
            {
                bool found=true;
                for(int k=0; k<lens; k++)
                    if(kw[i+k]!=s[k])
                    {
                        found=false;
                        break;
                    }
                if(found)
                    return true;
            }
            i=j+2;
        }
        return false;
    }
    cerr<<"book cmp error"<<endl;
    return false;
}

void book::del()
{
    use=false;
}

bool book::exist()
{
    return use;
}

void book::modify(int id, const char *s)
{
    if(id==1)
    {
        strcpy(ib,s);
        return;
    }
    if(id==2)
    {
        strcpy(nm,s);
        return;
    }
    if(id==3)
    {
        strcpy(at,s);
        return;
    }
    if(id==4)
    {
        strcpy(kw,s);
        return;
    }
    if(id==5)
    {
        sscanf(s,"%lf",&pc);
        return;
    }
    if(id==6)
    {
        int new_add;
        sscanf(s,"%d",&new_add);
        qt+=new_add;
    }
}

void book::display()
{
    printf("%s\t%s\t%s\t%s\t%.2f\t%d本\n",ib,nm,at,kw,pc,qt);
}

std::string book::ib_to_string()
{
    return ib;
}

bool book::buy(int cnt)
{
    if(cnt<=qt)
    {
        qt-=cnt;
        return true;
    }
    return false;
}

double book::get_pc()
{
    return pc;
}

