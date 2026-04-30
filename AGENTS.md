# Agent Directives

## Guiding Principles

- **Solve the actual problem** - implement only current requirements, not anticipated future needs
- **Use appropriate sophistication** - prefer simple solutions, but do not under-engineer when complexity is genuinely required
- **Avoid premature abstraction** - no complex class hierarchies, interfaces, or over-generalized functions for their own sake
- **Favor composition** - use "has-a" relationships rather than "is-a" inheritance unless inheritance genuinely models the domain
- **Maintain boundaries** - integrate features cleanly without entangling unrelated components

## Task Completion

- **Phase large refactors** - never attempt multi-file refactors in a single response; break work into explicit phases (eg: complete phase 1, verify using @critic, move to phase 2, verify using @critic, and so on)
- **Execute fully** - complete every aspect of the assigned task. Do not ask whether to continue or unilaterally decide the work is "good enough"
- **No shortcuts** - implement full functionality methodically according to the plan rather than skipping steps

## Sub-agent Delegation

- **Use @explore freely** - always try to delegate read-only research and codebase analysis to @explore; safe to run multiple instances in parallel
- **Use sequential @general freely** - offload isolated coding work to @general rather than handling everything directly in the main session
- **Use parallel @general with great care** - spawning parallel @general sub-agents can cause them to conflict and overwrite each other's work; either use them sequentially or carefully assign non-overlapping tasks
- **Use @troubleshoot when stuck** - invoke this read-only agent when you are confused, going in circles, or unable to make progress
- **Use @critic for verification** - invoke this read-only agent to compare output against requirements and flag errors before finishing

## External Dependencies

- **Never add external dependencies** unless they provide substantial, measurable value (eg: graphics, networking, JSON/XML parsing)
- **Justify worthwhile dependencies** - if a dependency is clearly worthwhile, explain trade-offs and ask the user before adding

## Error Handling

- **Never silently skip errors** - no catch-and-ignore, no empty exception handlers, no "safe" defaults that mask failures; if you cannot properly recover, crash immediately with a clear error
- **Fail early, fail hard** - strictly validate inputs at function boundaries, then assume validity thereafter
- **Handle only recoverable cases** (eg: network timeouts, retryable operations); unrecoverable errors must terminate execution, not propagate as silent data corruption
- **Avoid redundant validation** after initial boundary checks (eg: defensive null checks, unnecessary exception wrapping)

## Backwards Compatibility

- **Never consider backwards compatibility** (API changes, file versioning, etc) unless explicitly requested by the user

# Sandbox Restrictions

- **External network is blocked** - commands are run inside `bwrap --unshare-net` so network tools (`wget`, `curl`, `ping`, etc) and API requests will not work
- **Package installation is blocked** - package managers (`pip3`, `npm`, `gem`, `cargo`, `apt`, `brew`, etc) cannot reach external repositories
