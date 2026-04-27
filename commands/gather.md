---
description: Merge completed worktrees and verify the final result
agent: build
---

All worktree agents have completed. Finalize and merge the work:

1. For each worktree:
   - Verify they have committed all changes to their assigned Git worktree branch
   - Read `PLAN_REPORT.md` to see what they did and why
   - Delete `PLAN_REPORT.md`
   - Attempt to merge their branch into the current (master) branch
   - If merge conflicts arise, resolve them by consulting @MASTER_PLAN.md for intent
2. Invoke @critic to review the implementation against @MASTER_PLAN.md and capture the returned `task_id`
3. If @critic reports blockers:
   - Fix the identified issues
   - Resume the *same* @critic session using the `task_id` you saved to re-check
   - Repeat this step (step 3) until @critic confirms clean sign-off
4. Upon clean sign-off:
   - Summarize the verification
   - Delete `MASTER_PLAN.md` — it is no longer needed
   - Remove the `worktrees/` directory (eg: `git worktree remove` for each)
   - Delete the worktree branches
   - Ensure none of the worktrees' old `PLAN.md` or `PLAN_REPORT.md` files are staged — these are no longer needed
   - Commit the final merged state to the current (master) branch

CONSTRAINT: Do not stop until all worktree branches are merged, worktrees are removed, and the final commit is made.
