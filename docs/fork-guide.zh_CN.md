<p align="right">
  <strong>简体中文</strong> · <a href="fork-guide.md">English</a>
</p>

# 独立仓库工作流

`LeopardDennis/ai-passport-pdkpass` 是独立维护的 PDKPASS 仓库，源自开源的
`FoloToy/ai-passport` 硬件与固件基线，但已不属于上游 fork 网络。原许可证、版权声明与
来源说明继续适用。

本文保留历史文件名，以保证从上游基线继承的链接仍然有效。

## 分支职责

- `main` 是 PDKPASS 权威的产品与发布分支。
- 需要审查时，为聚焦改动使用短期 `feature/*`、`fix/*` 或文档分支。
- 不再用 `main` 保存第二份干净上游基线；PDKPASS 代码应位于 `main`。
- tag 与 GitHub Release 从通过验证的 `main` 提交创建。

根目录的 `README.md` 与 `README.zh_CN.md` 介绍 PDKPASS。硬件基线文档继续保存在
`docs/`，PDKPASS 专属补充材料可以保存在 `docs/assets/`。

## 引入上游更新

本仓库不再自动同步上游。需要引入上游硬件或兼容性修复时：

1. 通过只读的 `upstream` remote 或临时 clone 获取 `FoloToy/ai-passport`。
2. 在临时分支检查完整变更。
3. 只引入相关提交或文件，并根据 PDKPASS 解决冲突。
4. 合入 `main` 前运行静态与固件验证门禁。

禁止把上游 `main` 自动合并到本仓库的 `main`。上游更新可能修改应用代码、工作流、文档、
构建设置或 BLE Recovery 兼容契约。

## 向上游贡献

通用改进仍可提议给 `FoloToy/ai-passport`，但本独立仓库不是 GitHub fork。需要提交上游
Pull Request 时，应另建或使用上游项目的独立 fork，并只把相关通用改动复制到该分支。
PDKPASS 产品规则与素材保留在本仓库。创建上游 issue 或 Pull Request 属于外部操作，仍需
用户明确授权。
