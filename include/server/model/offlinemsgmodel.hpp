#ifndef OFFLINEMSGMODEL_HPP
#define OFFLINEMSGMODEL_HPP

#include <string>
#include <vector>

using namespace std;

// 提供离线消息表的接口方法
class OfflineMsgModel
{
public:
    // 存储用户的离线消息
    void insert(int id, string msg);
    // 删除用户的离线消息
    void remove(int id);
    // 查询用户的离线消息
    vector<string> query(int id);

private:

};

#endif