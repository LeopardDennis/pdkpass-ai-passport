<p align="right">
  <a href="fork-guide.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Standalone Repository Workflow

`LeopardDennis/ai-passport-pdkpass` is an independently maintained PDKPASS
repository derived from the open-source `FoloToy/ai-passport` hardware and
firmware baseline. It is not part of the upstream fork network. The original
license, copyright notices, and attribution remain applicable.

This file keeps its historical name so links inherited from the upstream
baseline remain valid.

## Branch roles

- `main` is the authoritative PDKPASS product and release branch.
- Use short-lived `feature/*`, `fix/*`, or documentation branches for focused
  changes when review is useful.
- Do not keep a second clean baseline on `main`; PDKPASS code belongs on `main`.
- Tags and GitHub releases are created from validated `main` commits.

The root `README.md` and `README.zh_CN.md` describe PDKPASS. Hardware-baseline
documentation remains under `docs/`, and PDKPASS-specific supporting material
may remain under `docs/assets/`.

## Upstream updates

There is no automatic upstream synchronization. When an upstream hardware or
compatibility fix is useful:

1. Fetch `FoloToy/ai-passport` into a read-only `upstream` remote or a temporary
   clone.
2. Review the complete change on a temporary branch.
3. Import only the relevant commits or files and resolve them against PDKPASS.
4. Run the static and firmware validation gates before merging into `main`.

Never automatically merge upstream `main` into this repository's `main`.
Upstream changes can modify application code, workflows, documentation, build
settings, or the BLE recovery compatibility contract.

## Contributing changes upstream

Reusable improvements may still be proposed to `FoloToy/ai-passport`, but this
standalone repository is not a GitHub fork. Create or use a separate fork of the
upstream repository for such a pull request, and copy only the relevant general
changes into that branch. PDKPASS product rules and assets stay here. Opening an
upstream issue or pull request remains an external action that requires explicit
user authorization.
