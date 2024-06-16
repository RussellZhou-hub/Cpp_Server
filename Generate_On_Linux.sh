# 项目根目录（可以根据需要进行修改）
PROJECT_ROOT=$(pwd)

# 构建目录
BUILD_DIR="$PROJECT_ROOT/build"

# 如果构建目录不存在，则创建它
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

# 进入构建目录
cd "$BUILD_DIR"

# 运行 CMake 以生成构建文件
cmake ..

# 构建项目
make