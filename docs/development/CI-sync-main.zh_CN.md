<p align="right">
  <strong>简体中文</strong> · <a href="CI-sync-main.md">English</a>
</p>

# 手动引入上游更新

本仓库已脱离 `FoloToy/ai-passport` 的 fork 网络。`main` 保存正式发布的 PDKPASS
产品，不再通过定时工作流自动合并上游变更。

需要硬件基线修复时，应把上游内容获取到临时审查分支，检查完整 diff，只引入相关提交或
文件，并在合并前运行完整验证门禁。禁止自动合并上游 `main`，否则可能替换 PDKPASS 的
应用代码、文档、工作流或兼容性设置。

旧 `.github/workflows/sync-main.yml` 已在仓库脱离 fork 网络时移除。可以选择保留只读的
上游 Git remote，但这不会改变本仓库的独立关系。
