<p align="right">
  <a href="CI-sync-main.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Manual Upstream Updates

This repository is independent from the `FoloToy/ai-passport` fork network.
`main` contains the released PDKPASS product, and no scheduled workflow merges
upstream changes into it.

When a hardware-baseline fix is needed, fetch the upstream repository into a
temporary review branch, inspect the complete diff, and import only the relevant
commits or files. Run the complete validation gate before merging. Never merge
upstream `main` automatically because doing so could replace PDKPASS application
code, documentation, workflows, or compatibility settings.

The old `.github/workflows/sync-main.yml` workflow was removed when this
repository left the fork network. Keeping upstream as a read-only Git remote is
optional and does not change the standalone repository relationship.
