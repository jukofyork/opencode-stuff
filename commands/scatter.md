---
description: Decompose the master plan into parallel worktrees
agent: build
---

Initial planning discussion is complete. Finalize the requirements into a master plan, then decompose it into parallel work.

1. Review the preceding discussion and context to understand the requirements
2. Identify anything unclear, contradictory, or missing
3. If real gaps exist, use the question tool to ask focused clarifying questions — one concern per question
4. Write the complete plan to `MASTER_PLAN.md` at the project root (overwrite if it exists)
5. Decompose the master plan into $1 independent (ie: **non-overlapping**) work items of approximately equal workload
6. Create $1 git worktrees inside `worktrees/` at the project root, each on its own branch (eg: `worktrees/wt-1/` on branch wt-1, etc.). Ensure `.gitignore` includes `worktrees/`.
7. Write a `PLAN.md` into each worktree containing that worktree's tasks, plus relevant context about what the other worktrees are handling (dependencies, shared interfaces, and boundaries)

Planning rules (steps 1–5):

- Only ask about genuine gaps. Do not ask about things already answered in context.
- Do not ask hypothetical or "what about future..." questions.
- Keep questions concrete and answerable.
- `MASTER_PLAN.md` is always the full current plan, never a partial update.
- Work items must not overlap. If shared changes are needed, assign them to exactly one worktree and note the dependency.

CONSTRAINT: Do not modify `MASTER_PLAN.md` during implementation and do not delete `MASTER_PLAN.md` unless instructed to do so.
