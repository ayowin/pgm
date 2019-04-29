# 指定cmake的最低版本
cmake_minimum_required(VERSION 2.8)

# 项目名称
project(pgm)

# 指定头文件目录
include_directories(include HEADERS_PATH)

# 指定源文件目录
aux_source_directory(src SOURCES_DIR)

# 指定连接目录
set(LINK_PATH ${SOURCES_DIR})

# 添加要编译的可执行文件
add_executable(${PROJECT_NAME} ${SOURCES_DIR})

# 添加依赖的库（如果需要依赖别的库的话，则需要填写）
# 命名规则：libxxx.so，只需要填写xxx部分即可。
#target_link_libraries(${PROJECT_NAME} xxx)