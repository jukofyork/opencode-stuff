#!/bin/bash
# file: ~/.local/bin/opencode

echo "[SANDBOX] Running opencode in restricted environment." >&2

# Base directory for opencode sandbox configuration
OPENCODE_BASE="$HOME/LLMs/opencode"

# Don't auto-update, model fetch and share.
export OPENCODE_DISABLE_AUTOUPDATE=1
export OPENCODE_DISABLE_MODELS_FETCH=1
export OPENCODE_DISABLE_SHARE=1

# Increase to 2 hours to allow long tests to run
export OPENCODE_EXPERIMENTAL_BASH_DEFAULT_TIMEOUT_MS=$((2*60*60*1000))

# Leave at default for now
export OPENCODE_EXPERIMENTAL_OUTPUT_TOKEN_MAX=32000

# Use geany as our editor
#export EDITOR="geany --wait"

# Use our custom `restricted_bash.c` that calls bash with `bwrap --unshare-net`
export SHELL="$OPENCODE_BASE/restricted_bash"

# Read extra bind mounts from config file (one argument per line)
EXTRA_BWRAP_ARGS=""
CONFIG_FILE="$OPENCODE_BASE/opencode_sandbox.cfg"
if [[ -f "$CONFIG_FILE" ]]; then
    while IFS= read -r line || [[ -n "$line" ]]; do
        # Skip empty lines and lines starting with #
        [[ -z "$line" ]] && continue
        [[ "$line" =~ ^# ]] && continue
        # Expand variables like $HOME in the line
        eval "expanded=\"$line\""
        EXTRA_BWRAP_ARGS="$EXTRA_BWRAP_ARGS $expanded"
    done < "$CONFIG_FILE"
    EXTRA_BWRAP_ARGS="${EXTRA_BWRAP_ARGS# }"  # Remove leading space
fi
export EXTRA_BWRAP_ARGS

# Check for git worktree .git file and bind parent .git directory
if [[ -f "$PWD/.git" ]]; then
    gitdir=$(awk '/^gitdir: /{print $2}' "$PWD/.git")
    if [[ -n "$gitdir" ]]; then
        parent_git="${gitdir%/*}"
        parent_git="${parent_git%/*}"
        if [[ -d "$parent_git" ]]; then
            EXTRA_BWRAP_ARGS="$EXTRA_BWRAP_ARGS --bind $parent_git $parent_git"
            echo "[SANDBOX] Worktree .git file found... Binding parent .git directory: '$parent_git'." >&2
        fi
    fi
fi

# Auto-compile if binary missing or source updated
if [[ ! -x "$SHELL" || "${SHELL}.c" -nt "$SHELL" ]]; then
    echo -n "[SANDBOX] Building restricted_bash... " >&2
    gcc -O2 -o "$SHELL" "${SHELL}.c" || exit 1
    echo "Done." >&2
fi

# Run the sand box (Note: No need for `--setenv` if not using `--clearenv`)
exec bwrap \
  --unshare-pid \
  --unshare-uts \
  --unshare-ipc \
  --share-net \
  --die-with-parent \
  --new-session \
  --dev /dev \
  --ro-bind /usr /usr \
  --ro-bind /bin /bin \
  --ro-bind /etc /etc \
  --ro-bind /lib /lib \
  --ro-bind-try /lib64 /lib64 \
  --tmpfs /run \
  --bind "$PWD" "$PWD" \
  --bind /tmp /tmp \
  --bind "$HOME/.local/share/opencode" "$HOME/.local/share/opencode" \
  --bind "$HOME/.cache/opencode" "$HOME/.cache/opencode" \
  --ro-bind "$HOME/.opencode/bin/opencode" "$HOME/.opencode/bin/opencode" \
  --ro-bind "$HOME/.config/opencode" "$HOME/.config/opencode" \
  --ro-bind-try "$OPENCODE_BASE/opencode.json" "$OPENCODE_BASE/opencode.json" \
  --ro-bind-try "$OPENCODE_BASE/AGENTS.md" "$OPENCODE_BASE/AGENTS.md" \
  --ro-bind-try "$OPENCODE_BASE/agents" "$OPENCODE_BASE/agents" \
  --ro-bind-try "$OPENCODE_BASE/commands" "$OPENCODE_BASE/commands" \
  --ro-bind-try "$OPENCODE_BASE/skills" "$OPENCODE_BASE/skills" \
  --ro-bind-try "$OPENCODE_BASE/plugins" "$OPENCODE_BASE/plugins" \
  --ro-bind "$SHELL" "$SHELL" \
  $EXTRA_BWRAP_ARGS \
  --proc /proc \
  --cap-drop ALL \
  -- "/usr/bin/opencode-cli" "$@"
