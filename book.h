//
// Created by Barrin on 2019/12/14.
//

#ifndef BOOK_STORE_BOOK_H
#define BOOK_STORE_BOOK_H
class book
{
    bool use{};
    double pc{};
    int qt{};
    char ib[21]{},nm[61]{},at[61]{},kw[61]{};
public:
    explicit book(const char*_ib="",const char*_nm="",const char*_at="",const char*_kw="", double pc=0,int qt=0);
    bool cmp(int id,const char *s);
    void del();
    bool exist();
    void modify(int id,const char *s);
    void display();
    bool buy(int cnt);
    double get_pc();
    std::string ib_to_string();
};
#endif //BOOK_STORE_BOOK_H
