# 把指定路径下的源文件加入到变量SRC_LIST中
aux_source_directory(. SRC_LIST)

# 根据SRC_LIST变量生成可执行程序chatclient
add_executable(chatclient ${SRC_LIST} ${DB_LIST} ${MODEL_LIST})

# 表示chatclient这个目标程序需要连接这些库
target_link_libraries(chatclient pthread)