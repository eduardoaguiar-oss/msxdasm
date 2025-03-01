#ifndef def_msxdasm
#define def_msxdasm

#include <string>

typedef unsigned char byte;
typedef unsigned short int word;

class msx_address {
private:
    int amount;
    msx_address_node *firstnode;
public:
    msx_address(void);
    std::string getname(int);
    std::string getcomment(int);
    int  loaddef(const std::string&);
    int  islabel(int);
    void showdef(void);
    void setaddress(int,const std::string&);
};

class msx_opcode {
public:
    std::string gettext(int);
    int getsize(int);

private:
    int  getsize_1(int);
    int  getsize_2(int);
    int  getsize_ed(int);
    int  getsize_ddfd(int);
    std::string gettext_1(int);
    std::string gettext_2(int);
    std::string gettext_ed(int);
    std::string gettext_cb(int);
    std::string gettext_ddfd(int);
    std::string gettext_ddfdcb(int);
};

class msx_dasm {
private:
    int   i_hl;
    byte *i_mem;
public:
    msx_dasm(void);
    ~msx_dasm(void);
    void navigate(int);
    void print(void);
private:
    void nav_jphla(int);
};

#endif
