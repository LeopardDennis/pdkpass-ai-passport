<p align="right">
  <strong>简体中文</strong> · <a href="README.md">English</a>
</p>

# PDKPASS macOS 模拟器

这个原生 Mac 模拟器直接运行正式固件中的 PDKPASS LVGL 页面、导航状态机、赛道轮廓、
分站主题、赛历、积分榜和成绩布局。它把设备真实的 240 × 320 RGB565 画面按最近邻方式
放大，不是另外复刻的一套网页效果图。

模拟器使用离线的 2026 赛历和积分榜快照。在线状态下打开历史成绩页时，会按分站和
场次从 OpenF1 获取真实排名，并把已经完成的前三名保存在 macOS 用户缓存目录中。
数字键可以查看固件的各种联网状态，只有在线状态会同步成绩。

## 环境要求

- macOS
- Xcode Command Line Tools
- CMake 3.16 或更高版本
- 仓库中已生成的 `managed_components/` 目录
- 首次读取尚未缓存的历史成绩时需要联网

如果缺少 `managed_components/`，先正常执行一次 ESP-IDF 配置或构建，让锁定版本的
LVGL 依赖下载到仓库。

## 构建与运行

在仓库根目录执行：

```bash
./tools/pdkpass-simulator/run.sh
```

如果复制仓库时没有保留可执行权限，改用 `bash tools/pdkpass-simulator/run.sh`。等价的
手动命令是：

```bash
cmake -S tools/pdkpass-simulator -B build/pdkpass-simulator
cmake --build build/pdkpass-simulator -j
./build/pdkpass-simulator/pdkpass-simulator
```

默认首页预览第 13 站，也可以指定首页自动选中的分站，例如：

```bash
./tools/pdkpass-simulator/run.sh --race 1
```

## 操作方式

| Mac 输入 | 对应设备操作 |
| --- | --- |
| 上 / 下方向键 | 浏览 |
| 回车或空格 | 确认 |
| 按住回车 0.65 秒 | 返回 |
| Esc | 返回 |
| `1` / `2` / `3` / `4` / `5` | 配网 / 连接中 / 校时 / 在线 / 离线 |
| `S` 或 Command-S | 把当前 240 × 320 画面保存为 PNG |

如需不打开窗口直接渲染并做冒烟测试：

```bash
./build/pdkpass-simulator/pdkpass-simulator \
  --race 13 --screenshot /tmp/pdkpass-simulator.png
```

如需不打开窗口，直接验证并截取真实的历史 FP1 成绩：

```bash
./build/pdkpass-simulator/pdkpass-simulator --race 1 --sync-results \
  --screenshot /tmp/pdkpass-australia-fp1.png
```
