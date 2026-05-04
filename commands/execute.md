---
description: Execute the plan for this worktree
agent: build
---

Review @PLAN.md, including the context about other worktrees' responsibilities, and then:

1. Update the todo list with work items from the plan
2. Implement the agreed plan, updating the todo list as you go
3. Invoke @critic to review the implementation against `PLAN.md` and capture the returned `task_id`
4. If @critic reports blockers:
   - Fix the identified issues
   - Resume the *same* @critic session using the `task_id` you saved to re-check
   - Repeat this step (step 4) until @critic confirms clean sign-off
5. Upon clean sign-off:
   - Write the final `REPORT.md` detailing what you have (or have not) done, and why
   - Commit all changes to your assigned Git worktree branch

CONSTRAINT: Do not stop until @critic confirms clean sign-off, the report is written, and the commit is made.
