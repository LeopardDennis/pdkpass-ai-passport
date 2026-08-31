<p align="right">
  <strong>简体中文</strong> · <a href="README.md">English</a>
</p>

# PDKPASS

PDKPASS 是为 FoloToy AI Passport 打造的离线优先 F1 周末伴侣，将 240 × 320
随身屏幕变成一张可以随时查看的赛历、车手积分榜与比赛详情通行证。

## 首个版本

- 开机直接进入下一场比赛页面。
- 浏览 2026 赛季剩余赛程，每站使用独立强调色。
- 显示截至 2026 年 8 月 31 日的完整车手积分榜快照。
- 比赛详情包含中国标准时间、赛道长度与圈数。
- 完全离线运行，闲置后自动降低并关闭背光。
- 保留 AI Passport 小程序安装、受保护的 `cardid`、永久 Recovery，以及长按上键
  5 秒进入 Recovery 的能力。

## 按键

| 页面 | 上键 / 下键 | 确定键 | 长按确定键 |
| --- | --- | --- | --- |
| 下一场 | 积分榜 / 赛历 | 比赛详情 | — |
| 赛历 | 选择比赛 | 比赛详情 | 首页 |
| 积分榜 | 滚动车手 | — | 首页 |
| 比赛详情 | 上一场 / 下一场 | 返回 | 返回 |

## 离线数据

内置数据是 2026 年 8 月 31 日获取的离线快照，来源为
[F1 官方 2026 赛历](https://www.formula1.com/en/racing/2026)与
[F1 官方车手积分榜](https://www.formula1.com/en/results/2026/drivers)。页面中的比赛
时间已转换为中国标准时间（UTC+8）。

PDKPASS 是独立车迷项目，与 Formula 1、FIA 或 FoloToy 没有隶属或背书关系。
Formula 1 及相关标识归各自权利人所有。

## 构建

使用 ESP-IDF 5.5.3，并执行：

```bash
./tools/validate.sh --static
./tools/validate.sh --firmware
```

仅分发通过验证的 `build/FoloToy-AI-Passport-full.bin`。
