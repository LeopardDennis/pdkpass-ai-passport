<p align="right">
  <strong>简体中文</strong> · <a href="readme-update.md">English</a>
</p>

# 动作 E：更新根 README

本动作更新独立仓库根目录的 `README.md`，反映新发布或归档的应用。它是[项目开发完成流程](../project-completion.md)列出的六项可选动作之一。

根 README 配对用于介绍 PDKPASS；继承的 AI Passport 硬件概览仍位于 `docs/README.md`，两者独立维护。

`main` 是 PDKPASS 产品分支。短期功能分支可以随聚焦改动更新 README，但最终产品文档必须进入 `main`。

## 何时建议

README 更新与其它五项一样是**可选**动作，也是归档的默认伴随动作：当应用归档到 `plays/`（动作 D）时，README 同步会作为该动作的一部分执行。归档本身是可选——开发者可以拒绝——但每当项目完成时，都应刷新 `main` 的 README，让仓库首页保持准确。

## 规则

- 更新 PDKPASS 根 README（`README.md` / `README.zh_CN.md`）；不修改 `docs/README.md` 的上游项目概览。
- `main` 根 README 是权威产品说明，应完整说明应用功能与使用方式，包括交互、模式、按键、持久化与重要备注。
- README 随产品改动走正常分支与审查流程。只有独立、可复用的上游改动才提交上游 PR。
- 遵循仓库语言规则：默认 `.md` 用英文，配对的 `.zh_CN.md` 用简体中文，同一变更内对齐。

## 步骤

1. 确认同意与可用的 GitHub 通道（GitHub MCP、GitHub skill 或 `gh`）。
2. 在工作分支上：缺 README 则补齐双语 README 对，已经有则更新应用说明。
3. 将通过审查的 README 与产品改动一起合入 `main`。
4. 确认仓库首页显示当前 PDKPASS 说明。

## 相关文档

- 独立仓库工作流：[fork-guide.md](../../fork-guide.md)
- 应用归档 skill：[plays-archive](../../../skills/plays-archive/SKILL.md)
- 文档规范：[doc-conventions.md](../../contribution/doc-conventions.md)
