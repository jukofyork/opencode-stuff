---
description: Execute the plan for this worktree
agent: build
---

Implement the tasks defined in @PLAN.md for this worktree:

1. Review @PLAN.md, including the context about other worktrees' responsibilities
2. Update the todo list with work items from the plan
3. Implement all assigned tasks
4. Invoke @critic to review the implementation against @PLAN.md and capture the returned `task_id`
5. If @critic reports blockers:
   - Fix the identified issues
   - Resume the *same* @critic session using the `task_id` you saved to re-check
   - Repeat this step (step 5) until @critic confirms clean sign-off
6. Upon clean sign-off:
   - Write the final `PLAN_REPORT.md` detailing what you have done (or have not done) and why
   - Commit all changes to your assigned Git worktree branch

CONSTRAINT: Do not stop until @critic confirms clean sign-off, the report is written and the commit is made.
