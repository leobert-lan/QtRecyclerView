QT += core gui widgets
TEMPLATE = app

# 指定包含目录
INCLUDEPATH += src

# 添加所有源文件
SOURCES += src/main.cpp \
           src/RecyclerView.cpp \
           src/LinearLayoutManager.cpp \
           src/GridLayoutManager.cpp \
           src/myviewholder.cpp

# 添加所有头文件
HEADERS += src/RecyclerView.h \
          src/AbstractLayoutManager.h \
          src/LinearLayoutManager.h \
          src/GridLayoutManager.h \
          src/RecyclerAdapter.h \
          src/ViewHolder.h \
          src/RecyclerCachePool.h \
          src/myviewholder.h

# 设置目标名称
TARGET = QtRecyclerView

# 启用C++17标准
CONFIG += c++17
