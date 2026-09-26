#!/usr/bin/env bash
# SessionStart: (1) relaunch a mesh-independent keep-alive loop so the container
# stays warm across restarts, (2) register this session as an agent and inject a
# coordination summary (best-effort; needs the mesh env, otherwise skipped).
set -euo pipefail
source "$(dirname "$0")/_mesh.sh"
read_hook_input

# --- (1) keep-alive loop, UNCONDITIONAL (no mesh env needed) ---------------------
# Survives fresh-clone restarts because this hook is committed. A pidfile guard
# prevents duplicate loops on repeated SessionStart events (startup/resume/clear).
KA_LOCK="${TMPDIR:-/tmp}/.kryptos-keepalive.pid"
if ! { [ -f "$KA_LOCK" ] && kill -0 "$(cat "$KA_LOCK" 2>/dev/null)" 2>/dev/null; }; then
  ( while true; do
      # best-effort mesh heartbeat when the env is present; harmless no-op otherwise
      mesh_ready && mesh_call heartbeat_session '{}' >/dev/null 2>&1 || true
      sleep 60
    done ) &
  echo $! > "$KA_LOCK" 2>/dev/null || true
fi

# --- (2) mesh registration + coordination context (needs mesh env) --------------
if mesh_ready; then
  SESSION_ID="$(json_get "$HOOK_INPUT" 'd.get("session_id","")')"
  NAME="${MESH_AGENT_NAME:-$(json_get "$HOOK_INPUT" 'd.get("session_name","")')}"
  [ -z "$NAME" ] && NAME="claude-$(hostname 2>/dev/null || echo agent)"
  mesh_call register_session "{\"name\":\"$NAME\",\"session_id\":\"$SESSION_ID\",\"status\":\"available\"}" >/dev/null
  STATUS="$(mesh_call get_coordination_status '{}')"
  CONTEXT="$(json_get "$STATUS" '"Claude Agents Mesh — %d agent(s), %d active task(s), %d recent message(s). Use whoami / get_coordination_status to coordinate; re-arm the Kryptos keep-alive cron (CronList/CronCreate */5) and read_messages if resuming an exchange." % (len(json.loads(d["result"]["content"][0]["text"]).get("agents",[])), len(json.loads(d["result"]["content"][0]["text"]).get("active_tasks",[])), len(json.loads(d["result"]["content"][0]["text"]).get("recent_messages",[])))')"
else
  # No mesh env: still nudge the assistant to re-arm its own keep-alive on resume.
  CONTEXT="Session (re)started. If a background exchange/loop was running, re-arm it: CronList and recreate the */5 keep-alive cron if missing, then read any pending mesh messages and continue."
fi

if [ -n "${CONTEXT:-}" ]; then
  printf '{"hookSpecificOutput":{"hookEventName":"SessionStart","additionalContext":%s}}\n' \
    "$(printf '%s' "$CONTEXT" | python3 -c 'import sys,json;print(json.dumps(sys.stdin.read()))')"
fi
exit 0
