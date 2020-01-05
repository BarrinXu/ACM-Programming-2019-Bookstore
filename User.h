//
// Created by Barrin on 2019/12/14.
//

#ifndef BOOK_STORE_USER_H
#define BOOK_STORE_USER_H
class User
{
    bool use;
    int level;
    char id[31],pd[31],nm[21];
public:
    //User(){};
    explicit User(const char *_id="", const char *_pd="", const char *_nm="", int _level=0);
    bool cmp_id(const char*_id);
    bool cmp_pd(const char*_pd);
    int get_level();
    bool exist();
    void del();
    //void add();
    void out();
    void change_pd(const char*_pd);
};
#endif //BOOK_STORE_USER_H
