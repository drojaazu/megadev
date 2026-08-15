# Contributing to Megadev

## Branching

Megadev follows git-flow.

| branch | purpose |
| --- | --- |
| `master` | released versions only. Never receives a topic branch directly. |
| `develop` | integration. All topic branches start here and return here. |
| `feature/*` | new capability, or a cross-cutting change to how the library works. |
| `fix/*` | repair of a specific defect. |
| `docs/*` | documentation only, with no change to emitted code. |
| `release/*` | version preparation, branched from `develop` at a freeze. |
| `hotfix/*` | urgent repair of a released version, branched from `master`. |

Choose `feature/` over `fix/` when the change alters how the library behaves for
every user, even if it began as a bug: `feature/pragma-once` and
`feature/macro-suffix-rename` are features by this rule, while
`fix/hextoa-terminator` repairs one routine.

**One branch, one concern.** Do not carry a refactor and a behavioural change on
the same branch, and branch from `develop` rather than from another topic
branch. Stacking makes the lower branch impossible to merge or revert on its
own, and its commits then arrive as part of something unrelated.

## Merging

Topic branches merge into `develop` locally, with `--no-ff`:

    git checkout develop
    git merge --no-ff feature/my-change

The merge commit is not ceremony: it keeps the branch a discrete unit that can
be reverted or read as one change long after the individual commits have blurred
together.

A release merges into `master` **and** back into `develop`, and is tagged on
`master`.

Pull requests are used only at the `master` boundary — a release or a hotfix.
For a topic branch merging into `develop` a PR adds a click and no scrutiny, so
it is skipped. Topic branches are still pushed, as offsite backup.

## Before merging into `develop`

`make check` must pass. It builds every example project and the template,
assembles every library source, and runs the ODR, symbol resolution, semantic
assertion and incremental rebuild tiers.

A change that alters emitted code should say what it measured. Comparing the
`.iso` is not a valid check: ISO9660 volume and directory records embed a
creation timestamp, so two builds of identical sources differ. Compare the
constituent artifacts instead — `build/ip.bin`, `build/sp.bin`, `build/boot.bin`
and each `disc/*.mmd` and `disc/*.smd`.

## Commit messages

One brief sentence per thing that changed. Use a bullet list when there are
several. The reasoning, the evidence and the history behind a change do not go
in the commit message — they go in `SPEC.md`.

## Where things are written down

- `SPEC.md` — what the kit must do, what is known broken, and the decision
  record. Every decision that constrains future work belongs here, with its
  provenance: `HW` (confirmed on hardware), `EMU` (confirmed in an emulator),
  `DOC` (from Sega documentation) or `ASSUMED`.
- `BACKLOG.md` — tracked work not yet done.
- `docs/*.md` — mechanism and background, for maintainers and for anyone going
  deeper than the API.
- Doxygen comments in `lib/` — usage notes only, written for a developer using
  the kit, for whom Megadev *is* the documentation. Manual citations, errata,
  and `SPEC`/`KB` references do not belong there.

## Tooling

Only tooling a user of the kit depends on is tracked: `tools/check/` (the
verification gate) and `tools/romfix.py` (invoked by the build). Tools written
to test a hypothesis or settle a hardware question live outside the repository,
alongside it. Where `SPEC.md` cites evidence produced by such a tool, it cites
it by external path and notes that it is outside the repo, so the finding
survives even though the apparatus is not shipped.
