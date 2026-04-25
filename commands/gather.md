---
description: Merge completed worktrees and verify the final result
agent: build
---

All worktree agents have completed. Finalize and merge the work.

1. Verify each worktree has committed all changes on its branch with no uncommitted files
2. For each worktree branch (wt-1, wt-2, etc), merge it into the current (master) branch
3. If merge conflicts arise, resolve them by consulting @MASTER_PLAN.md for intent
4. Invoke @critic sub-agent to review the implementation against @MASTER_PLAN.md and capture the returned task_id
5. If @critic reports blockers:
   - Fix the identified issues
   - Resume the *same* @critic session using the task_id you saved to re-check
   - Repeat this step (step 5) until @critic confirms clean sign-off
6. Summarize the verification
7. Upon clean sign-off:
   - Delete `MASTER_PLAN.md`
   - Remove the `worktrees/` directory (eg: `git worktree remove` for each)
   - Delete the worktree branches
   - Commit the final merged state to the current (master) branch

CONSTRAINT: Do not stop until all worktree branches are merged, worktrees are removed, and the final commit is made.
