#ifndef GROUP_HPP
#define GROUP_HPP

#include "groupuser.hpp"
#include <vector>
#include <string>

using namespace std;

// ALLGroup表的ORM类
class Group
{    
public:
    Group(int id = -1, string name = "", string desc = "")
    :m_id(id), m_name(name), m_desc(desc) {}
    
    void setId(int id) { this->m_id = id; }
    void setName(string name) { this->m_name = name; }
    void setDesc(string desc) { this->m_desc = desc; }

    int getId() { return this->m_id; }
    string getName() { return this->m_name; }
    string getDesc() { return this->m_desc; }
    vector<GroupUser> &getUsers() { return this->m_users; }

private:
    int m_id;
    string m_name;
    string m_desc;
    vector<GroupUser> m_users;

};




#endif