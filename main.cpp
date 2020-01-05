#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include "User.h"
#include "book.h"
using namespace std;
const int mod=97;
int cnt=0;
int now_block,now_pos;
book now_book;
vector<pair<string,pair<int,int> > >v;
void pre_work_user()
{
    ofstream tmp("user",ios::binary);
    int level=7;
    tmp.seekp(0);
    tmp.write((char*)(&level),sizeof(level));
    User rt("root","sjtu","root",7);
    tmp.write((char*)(&rt),sizeof(rt));
    tmp.close();
}
void pre_work_bill()
{
    ofstream out("bill",ios::binary);
    int cnt=0;
    double tmp=0;
    out.seekp(0);
    out.write((char*)(&cnt),sizeof(cnt));
    out.write((char*)(&tmp),sizeof(tmp));
    out.write((char*)(&tmp),sizeof(tmp));
    out.close();
}
void pre_work_bookdata()
{
    for(int i=0; i<mod; i++)
    {
        ofstream ret("bookdata"+to_string(i),ios::binary);
        ret.close();
    }
}
void pre_work_select()
{
    ofstream tmp("select",ios::binary);
    int id=0;
    tmp.seekp(0);
    tmp.write((char*)(&id),sizeof(id));
    id=-1;
    tmp.write((char*)(&id),sizeof(id));
    tmp.close();
}
void pre_work()
{
    ofstream tmp("pre",ios::binary);
    tmp.close();
    pre_work_user();
    pre_work_bookdata();
    pre_work_select();
    pre_work_bill();
}
void get_select()
{
    fstream tmp("select",ios::binary|ios::in|ios::out);
    tmp.seekg(0);
    tmp.read((char*)(&now_block),sizeof(int));
    tmp.read((char*)(&now_pos),sizeof(int));
    tmp.read((char*)(&now_book),sizeof(now_book));
    tmp.close();
}
void update_select()
{
    fstream tmp("select",ios::binary|ios::in|ios::out);
    tmp.seekp(0);
    tmp.write((char*)(&now_block),sizeof(int));
    tmp.write((char*)(&now_pos),sizeof(int));
    tmp.write((char*)(&now_book),sizeof(now_book));
    tmp.close();
}
int get_hash(const char *s)
{
    int len=strlen(s);
    int ret=0;
    for(int i=0; i<len; i++)
        ret=(ret*10+s[i]-'0')%mod;
    return ret;
}
void force_ISBN_update(book ret)
{
    fstream io("bookdata"+to_string(now_block),ios::binary|ios::in|ios::out);
    io.seekp(now_pos);
    io.write((char*)(&ret),sizeof(ret));
    io.close();
}
void select_ISBN(const char *s)
{
    now_block=get_hash(s);
    fstream io("bookdata"+to_string(now_block),ios::binary|ios::in|ios::out);
    book tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    bool found=false;
    now_pos=0;
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp(1,s))
        {
            found=true;
            break;
        }
        io.read((char*)(&tmp),sizeof(tmp));
        now_pos+=sizeof(tmp);
    }
    if(!found)
    {
        book ret(s);
        now_book=ret;
        io.seekp(now_pos);
        io.write((char*)(&ret),sizeof(ret));
    }
    else now_book=tmp;
    io.close();
    update_select();
}
bool found_ISBN(const char *s)
{
    now_block=get_hash(s);
    fstream io("bookdata"+to_string(now_block),ios::binary|ios::in|ios::out);
    book tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    bool found=false;
    now_pos=0;
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp(1,s))
            return true;
        io.read((char*)(&tmp),sizeof(tmp));
    }
    io.close();
    return false;
}
void show_ISBN(const char *s)
{
    fstream io("bookdata"+to_string(get_hash(s)),ios::binary|ios::in|ios::out);
    book tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    bool found=false;
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp(1,s))
        {
            found=true;
            break;
        }
        io.read((char*)(&tmp),sizeof(tmp));
    }
    if(found)
        tmp.display();
    io.close();
}
void pre_check()
{
    fstream io("pre",ios::binary|ios::in|ios::out);
    if(!io)
        pre_work();
    io.close();
}
void display_userlist()
{
    cout<<"!!!!"<<endl;
    fstream io("user",ios::binary|ios::in|ios::out);
    io.seekg(sizeof(int));
    User tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    while(!io.eof())
    {
        tmp.out();
        io.read((char*)(&tmp),sizeof(tmp));
    }
    io.close();
}
int get_login_level()
{
    int level;
    ifstream in("user",ios::binary|ios::in|ios::out);
    in.seekg(0);
    in.read((char*)(&level),sizeof(level));
    in.close();
    return level;
}
void set_login_level(int level)
{
    fstream io("user",ios::binary|ios::in|ios::out);
    io.seekp(0);
    io.write((char*)(&level),sizeof(level));
    io.close();
}
bool command_exist()
{
    fstream io("command.txt");
    bool fail=!io;
    io.close();
    return !fail;
}
void ui()
{
    cerr<<"enter ui mode"<<endl;
}

void try_login(const char *id,const char *pd)
{
    int level=get_login_level();
    ifstream in("user",ios::binary|ios::in|ios::out);
    User tmp;
    bool match=false;
    in.seekg(sizeof(int));
    in.read((char*)(&tmp),sizeof(tmp));
    while(!in.eof())
    {
        if(tmp.exist()&&tmp.cmp_id(id)&&(level==7||tmp.cmp_pd(pd)))
        {
            match=true;
            break;
        }
        in.read((char*)(&tmp),sizeof(tmp));
    }
    in.close();
    if(match)
        set_login_level(tmp.get_level());
    else puts("Invalid");
}
void try_delete(const char *id)
{
    bool found=false;
    fstream io("user",ios::binary|ios::in|ios::out);
    User tmp;
    io.seekg(sizeof(int));
    io.read((char*)(&tmp),sizeof(tmp));
    int cnt=0;
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp_id(id))
        {
            tmp.del();
            found=true;
            io.seekp(sizeof(int)+cnt*sizeof(tmp));
            io.write((char*)(&tmp),sizeof(tmp));
            break;
        }
        io.read((char*)(&tmp),sizeof(tmp));
        cnt++;
    }
    io.close();
    if(!found)
        puts("Invalid");
}
bool check_id_exist(const char *id)
{
    fstream io("user",ios::binary|ios::in|ios::out);
    io.seekg(sizeof(int));
    User tmp;
    io.read((char*)&tmp,sizeof(tmp));
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp_id(id))
            return true;
        io.read((char*)(&tmp),sizeof(tmp));
    }
    return false;
}
void change_pd(const char *id,const char *pd_old,const char *pd_new)
{
    int level=get_login_level();
    fstream io("user",ios::binary|ios::in|ios::out);
    User tmp;
    bool match=false;
    io.seekg(sizeof(int));
    io.read((char*)(&tmp),sizeof(tmp));
    int cnt=0;
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp_id(id))
        {
            if(level==7||tmp.cmp_pd(pd_old))
            {
                tmp.change_pd(pd_new);
                io.seekp(sizeof(int)+cnt*sizeof(tmp));
                io.write((char*)(&tmp),sizeof(tmp));
                match=true;
            }
            break;
        }
        io.read((char*)(&tmp),sizeof(tmp));
        cnt++;
    }
    io.close();
    if(!match)
        puts("Invalid");
}
int modify_id(const string &s)
{
    if(s[0]!='-')
        return 0;
    int eq_pos=0;
    for(int i=5; i<s.length(); i++)
        if(s[i]=='=')
        {
            eq_pos=i;
            break;
        }
    if(!eq_pos)
        return 0;
    string id=s.substr(1,eq_pos-1);
    if(id=="ISBN")
        return 1;
    if(id=="price")
        return 5;
    if(eq_pos+2>=s.length()||s[eq_pos+1]!='"'||s[s.length()-1]!='"')
        return 0;
    //if(id=="ISBN")
    //    return 1;
    if(id=="name")
        return 2;
    if(id=="author")
        return 3;
    if(id=="keyword")
        return 4;
    if(id=="price")
        return 5;
    return 0;
}
void search_each(int page,int id,const char *s)
{
    fstream io("bookdata"+to_string(page),ios::binary|ios::in|ios::out);
    book tmp;
    io.seekg(0);
    io.read((char*)(&tmp),sizeof(tmp));
    int pos=0;
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp(id,s))
            v.emplace_back(tmp.ib_to_string(),make_pair(page,pos));
        pos+=sizeof(tmp);
        io.read((char*)(&tmp),sizeof(tmp));
    }
}
void dp_book(int block,int pos)
{
    fstream io("bookdata"+to_string(block),ios::binary|ios::in|ios::out);
    io.seekg(pos);
    book tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    tmp.display();
}
void search_all(int id,const char *s)
{
    v.clear();
    for(int i=0; i<mod; i++)
        search_each(i,id,s);
    sort(v.begin(),v.end());
    for(auto & i : v)
        dp_book(i.second.first,i.second.second);
}
void show_solve(const string &s)
{
    int eq_pos=0;
    for(int i=5; i<s.length(); i++)
        if(s[i]=='=')
        {
            eq_pos=i;
            break;
        }
    string id=s.substr(1,eq_pos-1),val=s.substr(eq_pos+2,s.length()-eq_pos-3);
    if(id=="ISBN")
    {
        val=s.substr(eq_pos+1,s.length()-eq_pos-1);
        show_ISBN(val.c_str());
        return;
    }
    else if(id=="name")
        search_all(2,val.c_str());
    else if(id=="author")
        search_all(3,val.c_str());
    else if(id=="keyword")
        search_all(4,val.c_str());
    else cerr<<"show error"<<endl;
}
void modify_select(const string &s)
{
    int eq_pos=0;
    for(int i=5; i<s.length(); i++)
        if(s[i]=='=')
        {
            eq_pos=i;
            break;
        }
    string id=s.substr(1,eq_pos-1),newv=s.substr(eq_pos+2,s.length()-eq_pos-3);
    if(id=="ISBN")
    {
        newv=s.substr(eq_pos+1,s.length()-eq_pos-1);
        if(found_ISBN(newv.c_str()))
        {
            puts("Invalid");
            return;
        }
        book old_book=now_book;
        now_book.del();
        force_ISBN_update(now_book);
        select_ISBN(newv.c_str());
        now_book=old_book;
        now_book.modify(1,newv.c_str());
        force_ISBN_update(now_book);
    }
    else if(id=="name")
    {
        now_book.modify(2,newv.c_str());
        force_ISBN_update(now_book);
    }
    else if(id=="author")
    {
        now_book.modify(3,newv.c_str());
        force_ISBN_update(now_book);
    }
    else if(id=="keyword")
    {
        now_book.modify(4,newv.c_str());
        force_ISBN_update(now_book);
    }
    else if(id=="price")
    {
        newv=s.substr(eq_pos+1,s.length()-eq_pos-1);
        now_book.modify(5,newv.c_str());
        force_ISBN_update(now_book);
    }
}

void bill_out(const string &s)
{
    double nowout,nowin;
    fstream io("bill",ios::binary|ios::in|ios::out);
    io.seekg(0);
    int cnt;
    io.read((char*)(&cnt),sizeof(cnt));
    cnt++;
    io.seekp(0);
    io.write((char*)(&cnt),sizeof(cnt));
    io.seekg(-16,ios::end);
    io.read((char*)(&nowin),sizeof(nowin));
    io.read((char*)(&nowout),sizeof(nowout));
    double tmp;
    sscanf(s.c_str(),"%lf",&tmp);
    nowout+=tmp;
    io.seekp(0,ios::end);
    io.write((char*)(&nowin),sizeof(nowin));
    io.write((char*)(&nowout),sizeof(nowout));
    io.close();
}
void bill_in(double v)
{
    double nowout,nowin;
    fstream io("bill",ios::binary|ios::in|ios::out);
    io.seekg(0);
    int cnt;
    io.read((char*)(&cnt),sizeof(cnt));
    cnt++;
    io.seekp(0);
    io.write((char*)(&cnt),sizeof(cnt));
    io.seekg(-16,ios::end);
    io.read((char*)(&nowin),sizeof(nowin));
    io.read((char*)(&nowout),sizeof(nowout));
    nowin+=v;
    io.seekp(0,ios::end);
    io.write((char*)(&nowin),sizeof(nowin));
    io.write((char*)(&nowout),sizeof(nowout));
    io.close();
}
void buy(const char *isbn,const char *quantity)
{
    fstream io("bookdata"+to_string(get_hash(isbn)),ios::binary|ios::in|ios::out);
    io.seekg(0);
    book tmp;
    io.read((char*)(&tmp),sizeof(tmp));
    bool found=false;
    int pos=0,want;
    sscanf(quantity,"%d",&want);
    while(!io.eof())
    {
        if(tmp.exist()&&tmp.cmp(1,isbn))
        {
            found=true;
            break;
        }
        io.read((char*)(&tmp),sizeof(tmp));
        pos+=sizeof(tmp);
    }
    if(found)
    {
        if(tmp.buy(want))
        {
            bill_in(want*tmp.get_pc());
            io.seekp(pos);
            io.write((char*)(&tmp),sizeof(tmp));
        }
        else puts("Invalid");
    }
    else puts("Invalid");
    io.close();
}
pair<double,double> get_finance(int cnt)
{
    fstream io("bill",ios::binary|ios::in|ios::out);
    io.seekg(sizeof(int)+2*sizeof(double)*cnt);
    double a,b;
    io.read((char*)(&a),sizeof(a));
    io.read((char*)(&b),sizeof(b));
    io.close();
    return make_pair(a,b);
}
int get_finance_time()
{
    fstream io("bill",ios::binary|ios::in|ios::out);
    int cnt;
    io.seekg(0);
    io.read((char*)(&cnt),sizeof(cnt));
    io.close();
    return cnt;
}
void show_finance(const char *s)
{
    int tot=get_finance_time();
    int need=tot;
    if(strlen(s))
        sscanf(s,"%d",&need);
    if(need>tot)
    {
        puts("Invalid");
        return;
    }
    pair<double,double>L=get_finance(tot-need),R=get_finance(tot);
    printf("+ %.2f - %.2f\n",R.first-L.first,R.second-L.second);
}
void add_user(User u)
{
    fstream io("user",ios::binary|ios::in|ios::out);
    io.seekp(0,ios::end);
    io.write((char*)(&u),sizeof(u));
    io.close();
}
int analyze(const string& tt)
{
    vector<string>s;
    bool flag=false;
    int len=tt.length();
    for(int i=0; i<len;)
        if(tt[i]!=' ')
        {
            if(tt[i]=='"')
                flag^=1;
            int j=i;
            while(j+1<len&&(tt[j+1]!=' '||flag))
            {
                j++;
                if(tt[j]=='"')
                    flag^=1;
            }
            s.push_back(tt.substr(i,j-i+1));
            i=j+2;
        }
    if(s.empty())
        return 1;
    if(s.size()==1&&s[0]=="exit")
        return 0;
    if((s.size()==3||s.size()==2)&&s[0]=="su")
    {
        if(s[1].size()>30||s[2].size()>30)
            return -1;
        if(s.size()==2)
            s.emplace_back("");
        try_login(s[1].c_str(),s[2].c_str());
        return 1;
    }
    if(s.size()==1&&s[0]=="logout")
    {
        if(get_login_level()<1)
            return -1;
        set_login_level(0);
        return 1;
    }
    if(s.size()==5&&s[0]=="useradd")
    {
        if(s[3]!="7"&&s[3]!="3"&&s[3]!="1")
            return -1;
        if(s[1].size()>30||s[2].size()>30||s[4].size()>20)
            return -1;
        int level=s[3][0]-'0';
        if(level>=get_login_level())
            return -1;
        if(check_id_exist(s[1].c_str()))
            return -1;
        User tmp(s[1].c_str(),s[2].c_str(),s[4].c_str(),level);
        add_user(tmp);
        return 1;
    }
    if(s.size()==4&&s[0]=="register")
    {
        if(s[1].size()>30||s[2].size()>30||s[3].size()>20)
            return -1;
        if(check_id_exist(s[1].c_str()))
            return -1;
        User tmp(s[1].c_str(),s[2].c_str(),s[3].c_str(),1);
        add_user(tmp);
        return 1;
    }
    if(s.size()==2&&s[0]=="delete")
    {
        if(get_login_level()<7)
            return -1;
        try_delete(s[1].c_str());
        return 1;
    }
    if((s.size()==3||s.size()==4)&&s[0]=="passwd")
    {
        if(s.size()==3)
            s.emplace_back(""),swap(s[2],s[3]);
        if(s[1].size()>30||s[2].size()>30||s[3].size()>30)
            return -1;
        if(get_login_level()<1)
            return -1;
        change_pd(s[1].c_str(),s[2].c_str(),s[3].c_str());
        return 1;
    }
    if(s.size()==2&&s[0]=="select")
    {
        if(get_login_level()<3)
            return -1;
        select_ISBN(s[1].c_str());
        return 1;
    }
    if(s[0]=="modify"&&s.size()>=2)
    {
        if(get_login_level()<3)
            return -1;
        bool use[6];
        memset(use,0,sizeof(use));
        for(int i=1; i<s.size(); i++)
        {
            int id=modify_id(s[i]);
            if(id==0)
                return -1;
            if(use[id])
                return -1;
            use[id]=true;
        }
        get_select();
        if(now_pos==-1)
            return -1;
        for(int i=1; i<s.size(); i++)
            modify_select(s[i]);
        update_select();
        return 1;
    }
    if(s[0]=="import"&&s.size()==3)
    {
        get_select();
        if(now_pos==-1)
            return -1;
        bill_out(s[2]);
        now_book.modify(6,s[1].c_str());
        update_select();
        force_ISBN_update(now_book);
        return 1;
    }
    if(s[0]=="show"&&s.size()>=2&&s[1]=="finance")
    {
        if(get_login_level()<7)
            return -1;
        if(s.size()==2)
            show_finance("");
        else
            show_finance(s[2].c_str());
        return 1;
    }
    if(s[0]=="show")
    {
        if(get_login_level()<1)
            return -1;
        if(s.size()==1)
        {
            search_all(0,"");
            return 1;
        }
        int id=modify_id(s[1]);
        if(!id)
            return -1;
        show_solve(s[1]);
        return 1;
    }
    if(s[0]=="buy"&&s.size()==3)
    {
        if(get_login_level()<1)
            return -1;
        buy(s[1].c_str(),s[2].c_str());
        return 1;
    }
    return -1;
}
int main()
{
    //int cnt=0;
    //freopen("out.txt","w",stdout);
    pre_check();
    //if(!command_exist())
    //    ui();
    //ifstream in("command.txt");
    while(true)
    //while(!in.eof())
    {
        cnt++;
        //cout<<cnt<<endl;
        //display_userlist();
        string s;
        //getline(in,s);
        getline(cin,s);
        int id=analyze(s);
        //display_userlist();
        if(id==0)
            break;//!!!change
        if(id==-1)
        {
            puts("Invalid");
            continue;
        }
        //now_book.display();
    }
    //in.close();
    return 0;
}
