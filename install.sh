#!/bin/bash
set -e

echo "=== Cay install script (system /opt directory) ==="

# 依赖检查
if ! command -v gcc &>/dev/null; then
    echo "ERROR: gcc not found!"
    exit 1
fi
if ! pkg-config --libs libcurl &>/dev/null; then
    echo "ERROR: libcurl not found!"
    echo "Arch: sudo pacman -S curl"
    exit 1
fi
if ! pkg-config --libs libgit2 &>/dev/null; then
    echo "ERROR: libgit2 not found!"
    echo "Arch: sudo pacman -S libgit2"
    exit 1
fi

# 校验所有源文件是否都存在
SRC_FILES="cay.c aur.c rpc.c json.c git.c makepkg.c"
for f in $SRC_FILES; do
    if [ ! -f "$f" ]; then
        echo "ERROR: missing source file $f"
        exit 1
    fi
done

# 1.编译
echo "[1/4] Compile multi-source cay"
gcc -O2 -Wall -o cay cay.c aur.c rpc.c json.c git.c makepkg.c -lcurl -lgit2

# 2. 创建/opt/cay 目录
echo "[2/4] Create /opt/cay directory"
sudo mkdir -p /opt/cay

# 3. 复制二进制到 /opt/cay
echo "[3/4] Install binary to /opt/cay"
sudo cp ./cay /opt/cay/cay
sudo chmod +x /opt/cay/cay

# 4. 添加全局PATH：profile.d(登录shell) + bash.bashrc(XFCE普通终端)
echo "[4/4] Add /opt/cay to global PATH"
PROFILE_FILE="/etc/profile.d/cay.sh"
if [ ! -f "${PROFILE_FILE}" ]; then
    echo 'export PATH="/opt/cay:$PATH"' | sudo tee "${PROFILE_FILE}"
    sudo chmod 644 "${PROFILE_FILE}"
fi

# 写入全局bashrc，适配XFCE图形终端（非登录shell）
if ! grep -qF 'export PATH="/opt/cay:$PATH"' /etc/bash.bashrc 2>/dev/null; then
    echo 'export PATH="/opt/cay:$PATH"' | sudo tee -a /etc/bash.bashrc
fi

echo ""
echo "✅ Cay installed to /opt/cay"
echo "👉 生效方式：关闭全部终端，重新打开新终端即可直接使用 cay"
echo ""
echo "运行： cay"
echo ""
echo "===== 卸载命令 ====="
echo "sudo rm -rf /opt/cay"
echo "sudo rm -f /etc/profile.d/cay.sh"
echo "sudo sed -i '\\|^export PATH=\"/opt/cay:\$PATH\"\$|d' /etc/bash.bashrc"

# 清理本地编译产物
rm -f ./cay
