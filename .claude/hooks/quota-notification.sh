#!/usr/bin/env bash
# Notification: inspect the message for quota/limit signals and report them to the
# mesh so other agents learn this account is blocked or back. Best-effort and
# heuristic — Claude Code does NOT emit a structured quota event to hooks, so this
# relies on the human-readable notification text matching the patterns below.
# If MESH_MCP_URL / MESH_MCP_TOKEN are not set in the environment, this no-ops.
set -euo pipefail
source "$(dirname "$0")/_mesh.sh"
read_hook_input
mesh_ready || exit 0

MSG="$(json_get "$HOOK_INPUT" 'str(d.get("message","")).lower()')"
[ -z "$MSG" ] && exit 0

# Try to lift a reset time (e.g. "resets at 13:11", "until 1pm") out of the text.
RESET="$(json_get "$HOOK_INPUT" 'import re
m=re.search(r"(?:reset|resets|resume|until|back at|à)\\D{0,12}(\\d{1,2}[:h]\\d{2}|\\d{1,2}\\s*(?:am|pm))", str(d.get("message","")).lower())
print(m.group(1) if m else "")')"

case "$MSG" in
  *"resume"*|*"reset"*|*"auto_resume"*|*"auto-resume"*|*"back online"*|*"restored"*)
    mesh_call report_quota_event '{"event_type":"quota_auto_resumed"}' >/dev/null
    mesh_call heartbeat_session '{"status":"available"}' >/dev/null ;;
  *"limit"*|*"quota"*|*"rate"*|*"usage"*|*"paused"*|*"overloaded"*)
    mesh_call report_quota_event "{\"event_type\":\"quota_warning\",\"error_details\":{\"source\":\"notification\",\"reset_hint\":\"${RESET}\"}}" >/dev/null
    # Make the pause visible in coordination status, not just as an event.
    mesh_call heartbeat_session '{"status":"waiting_for_reset"}' >/dev/null ;;
esac
exit 0
