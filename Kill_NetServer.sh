#!/bin/bash

# 使用 ps 和 grep 获取 NetServer 进程号，过滤掉 grep 进程本身
pid=$(ps aux | grep NetServer | grep -v grep | awk '{print $2}')

# 检查是否找到进程号
if [ -z "$pid" ]; then
  echo "NetServer 进程未运行"
else
  echo "找到 NetServer 进程，进程号: $pid"
  # 杀死进程
  kill -9 $pid
  # 检查 kill 命令是否成功
  if [ $? -eq 0 ]; then
    echo "成功终止 NetServer 进程"
  else
    echo "终止 NetServer 进程失败"
  fi
fi