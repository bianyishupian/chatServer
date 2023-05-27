
# 把指定路径下的源文件加入到变量SRC_LIST中
aux_source_directory(. SRC_LIST)
aux_source_directory(./database DB_LIST)

# 根据SRC_LIST变量生成可执行程序chatserver
add_executable(chatserver ${SRC_LIST} ${DB_LIST})

# 表示chatserver这个目标程序需要连接这些库
target_link_libraries(chatserver muduo_net muduo_base mysqlclient pthread)

