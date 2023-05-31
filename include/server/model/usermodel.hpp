#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "user.hpp"

// user表的数据操作类
class UserModel
{
public:
    // 插入user信息
    bool insert(User &user);
    // 根据id查询user信息
    User query(int id);
    // 更新用户状态信息
    bool updateState(User user);
    // 重置用户状态信息
    void resetState();
};



#endif