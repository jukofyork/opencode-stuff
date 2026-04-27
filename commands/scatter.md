---
description: Decompose the master plan into parallel worktrees
agent: build
---

Initial planning discussion is complete. Finalize the requirements into a master plan, then decompose it into parallel work:

1. Review the preceding discussion and context to understand the requirements
2. Identify anything unclear, contradictory, or missing
3. If real gaps exist, use the question tool to ask focused clarifying questions — one concern per question
4. Write the complete plan to `MASTER_PLAN.md` at the project root (overwrite if it exists)
5. Decompose the master plan into $1 independent (ie: **non-overlapping**) work items
6. Initialize a new Git repo if one does not already exist and ensure `.gitignore` includes `worktrees/`
7. Create $1 Git worktrees inside `worktrees/` at the project root — each on its own branch (eg: `worktrees/<wt-branchname>/` on branch <wt-branchname>, etc.)
8. Write a `PLAN.md` into each worktree containing:
   - The worktree's specific task(s) along with a brief outline of the master plan
   - Any relevant context about what the other worktrees are handling (dependencies, shared interfaces, and boundaries)
   - A reminder to never modify `PLAN.md` during implementation
   - Instructions to write a final `PLAN_REPORT.md` detailing what they have done (or have not done) and why

Planning rules (steps 1–4):
- Only ask about genuine gaps. Do not ask about things already answered in context
- Do not ask hypothetical or "what about future..." questions
- Keep questions concrete and answerable
- `MASTER_PLAN.md` is always the full current plan, never a partial update
- Work items must not overlap — if shared changes are needed, assign them to exactly one worktree and note the dependency

CONSTRAINT: Never modify `MASTER_PLAN.md` during implementation and do not delete `MASTER_PLAN.md` until instructed to do so.

$2
