<p align="right">
  <a href="readme-update.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Action E: Update the Root README

This action updates the standalone repository's root `README.md` to reflect the
newly released or archived application. It is one of the six optional closing
actions listed in the [project completion](../project-completion.md).

The root README pair describes PDKPASS. The inherited AI Passport hardware
overview remains at `docs/README.md` and is maintained separately.

`main` is the PDKPASS product branch. A short-lived feature branch may update
the README as part of its focused change, but the final product documentation
must be present on `main`.

## When this is recommended

The README update is an **optional** action like the other five, and it is also
the default companion to archiving: when the application is archived to `plays/`
(action D), the README sync runs as part of that action. Archiving itself is
optional — the developer may decline — but whenever a project is completed, the
README should be refreshed on `main` so the application remains accurately
described on the repository landing page.

## Rules

- Update the PDKPASS root READMEs (`README.md` / `README.zh_CN.md`); do not
  modify the upstream project overview at `docs/README.md`.
- The `main` root README is the authoritative product description: include what
  the application does and how to use it, including interactions, modes, keys,
  persistence, and important notes.
- Commit the README with the product change through the normal branch and review
  workflow. Open an upstream PR only for a separate, reusable upstream change.
- Follow the repository language rule: English at the default `.md` path and
  Simplified Chinese at the paired `.zh_CN.md`, aligned in the same change.

## Steps

1. Confirm consent and a GitHub channel (GitHub MCP, a GitHub skill, or `gh`).
2. On the working branch: create the bilingual README pair if it is
   missing, or update it to add or refresh the application's own description.
3. Merge the reviewed README pair with the product change into `main`.
4. Confirm the repository landing page displays the current PDKPASS description.

## Related documents

- Standalone repository workflow: [fork-guide.md](../../fork-guide.md)
- Application archive skill: [plays-archive](../../../skills/plays-archive/SKILL.md)
- Documentation conventions: [doc-conventions.md](../../contribution/doc-conventions.md)
