#include "../../../include/server/model/usermodel.hpp"
#include "../../../include/server/database/database.h"

#include <iostream>
#include <string>

bool UserModel::insert(User &user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into User(name,password,state) value('%s','%s','%s')",
            user.getName().c_str(), user.getPassword().c_str(), user.getState().c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键id并设置到user中
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}