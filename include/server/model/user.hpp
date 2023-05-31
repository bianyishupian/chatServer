#ifndef USER_HPP
#define USER_HPP

#include <string>

using namespace std;

// user表的ORM类
class User
{
public:
    User(int id = -1,string name = "",string pwd = "", string state = "offline")
    :m_id(id), m_name(name), m_password(pwd), m_state(state)
    {

    }

    void setId(int id) { this->m_id = id; }
    void setName(string name) { this->m_name = name; }
    void setPassword(string pwd) { this->m_password = pwd; }
    void setState(string state) { this->m_state = state; }

    int getId() { return this->m_id; }
    string getName() { return this->m_name; }
    string getPassword() { return this->m_password; }
    string getState() { return this->m_state; }
    

protected:
    int m_id;
    string m_name;
    string m_password;
    string m_state;

};

#endif