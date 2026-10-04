# cay
# CAY 纯C语言编写的轻量AUR助手，适用于Arch Linux。 仅依赖 git、makepkg、libcurl、libgit2，资源占用低，低配机器也可流畅运行。 实现AUR搜索、信息查看、源码拉取、编译安装基础功能，代码模块化，便于阅读与二次修改。  ## 使用 `cay -Ss &lt;包名>` 搜索AUR软件包 `cay -Si &lt;包名>` 查看包详情、版本及依赖 `cay -S &lt;包名>` 拉取源码，编译并安装包  编译：执行 make 安装：运行安装脚本，程序部署至 /opt/cay，注册全局命令。 提供卸载脚本，一键删除全部文件，无系统残留。  提示：非Arch系统仅支持搜索、查询功能，安装依赖pacman与makepkg，无法使用。欢迎提交bug反馈。
