#!/usr/bin/env bash
# SessionStart: register this session as a mesh agent (if the mesh env is present)
# and inject coordination guidance. NO background while-true loop: on cloud the
# container is reclaimed when idle and any such loop is killed. Instead we tell the
# assistant to keep itself reactive by scheduling send_later wake-ups that RESUME
# THIS SAME SESSION (context preserved) — never by spawning new sessions.
set -euo pipefail
source "$(dirname "$0")/_mesh.sh"
read_hook_input

POLL_MIN="${MESH_POLL_MINUTES:-5}"

BASE_CTX="Tu participes a un Claude Agents Mesh (serveur MCP mesh, coordination multi-agents via Supabase). Regles de fonctionnement autonome:
- Au demarrage: register_session, puis read_messages et get_coordination_status.
- Pour rester reactif sans rester eveille et SANS perdre le contexte: a la fin de ton tour, programme un reveil avec send_later (delay ~${POLL_MIN} min, message du type 'Re-verifie le mesh: read_messages + get_coordination_status et traite'). Au reveil, la MEME session reprend: relis, agis, puis RE-programme un nouveau send_later. Ne cree JAMAIS de nouvelle session (pas de routine /fire) — utilise toujours send_later qui reprend cette session.
- Quand tu as fini pour de bon (plus rien a attendre), tu peux arreter la chaine de send_later.
- Communication: send_message / read_messages / ack_message. Taches: create_task / claim_task / heartbeat_task / complete_task."

if mesh_ready; then
  SESSION_ID="$(json_get "$HOOK_INPUT" 'd.get("session_id","")')"
  NAME="${MESH_AGENT_NAME:-$(json_get "$HOOK_INPUT" 'd.get("session_name","")')}"
  [ -z "$NAME" ] && NAME="claude-$(hostname 2>/dev/null || echo agent)"
  mesh_call register_session "{\"name\":\"$NAME\",\"session_id\":\"$SESSION_ID\",\"status\":\"available\"}" >/dev/null
  STATUS="$(mesh_call get_coordination_status '{}')"
  EXTRA="$(json_get "$STATUS" '" Etat actuel: %d agent(s), %d tache(s) active(s), %d message(s) recent(s)." % (len(json.loads(d["result"]["content"][0]["text"]).get("agents",[])), len(json.loads(d["result"]["content"][0]["text"]).get("active_tasks",[])), len(json.loads(d["result"]["content"][0]["text"]).get("recent_messages",[])))')"
else
  EXTRA=" (Variables mesh non definies dans cet environnement: appelle whoami via le connecteur MCP pour confirmer ton identite.)"
fi

CTX="${BASE_CTX}${EXTRA:-}"
printf '{"hookSpecificOutput":{"hookEventName":"SessionStart","additionalContext":%s}}\n' \
  "$(printf '%s' "$CTX" | python3 -c 'import sys,json;print(json.dumps(sys.stdin.read()))')"
exit 0
