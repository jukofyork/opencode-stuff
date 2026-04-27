---
description: Invoke the critic sub-agent for a final review
agent: build
---

Ready for final review. Please finalize this work:

1. Invoke @critic to review the implementation against @PLAN.md and capture the returned `task_id`
2. If @critic reports blockers:
   - Fix the identified issues
   - Resume the *same* @critic session using the `task_id` you saved to re-check
   - Repeat this step (step 2) until @critic confirms clean sign-off
3. Upon clean sign-off:
   - Summarize the verification
   - Delete `PLAN.md` — it is no longer needed
   - Reset the todo list
   - Commit all changes to the Git repo

CONSTRAINT: Do not stop until @critic confirms the work is correct and complete.
