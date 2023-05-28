#ifndef FRIENDMODEL_HPP
#define FRIENDMODEL_HPP

#include "user.hpp"
#include "../database/database.h"
#include <vector>

// 好友信息的操作接口
class FriendModel
{
public:
    // 添加好友
    bool insert(int userid, int friendid);
    // 根据id查询好友信息
    std::vector<User> query(int userid);

private:

};

#endif