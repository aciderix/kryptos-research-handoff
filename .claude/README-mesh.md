# Hooks Claude Agents Mesh (dépôt kryptos)

Ces hooks branchent ce dépôt sur le [Claude Agents Mesh](https://github.com/aciderix/claude-agents-mesh)
(coordination multi-agents via Supabase) sans installer le plugin.

## Ce qui est câblé (`.claude/settings.json`)

| Événement     | Script                        | Effet |
|---------------|-------------------------------|-------|
| `SessionStart`| `hooks/session-start.sh`      | Enregistre l'agent, injecte l'état de coordination, lance un heartbeat de fond (60 s) |
| `Stop`        | `hooks/stop.sh`               | Rafraîchit la présence à chaque fin de tour |
| `SessionEnd`  | `hooks/session-end.sh`        | Passe l'agent `offline` |
| `Notification`| `hooks/quota-notification.sh` | Remonte les signaux de quota au mesh |

`hooks/_mesh.sh` fournit les helpers partagés (`mesh_call`, `mesh_ready`).

## Pré-requis — 2 variables d'environnement

Les scripts appellent le mesh en HTTP et sortent en silence (`mesh_ready || exit 0`)
tant que ces deux variables ne sont pas définies. Un token est un **secret** : il ne
se committe pas — on le met dans l'environnement.

- `MESH_MCP_URL`   = `https://<project-ref>.supabase.co/functions/v1/coordinator/mcp`
- `MESH_MCP_TOKEN` = `mesh_…`  (token membre, en secret)
- `MESH_AGENT_NAME` = `Claude-Kryptos`  (optionnel, nom d'agent stable)

**En session cloud (claude.ai/code)** : réglages de l'environnement → *Edit* →
variables d'environnement (ou *API credentials* pour le token). Une **nouvelle**
session les prend en compte.

**En local** : `export MESH_MCP_URL=… MESH_MCP_TOKEN=…` avant de lancer `claude`.

## Limite importante (cloud)

Les hooks d'un `.claude/settings.json` de dépôt ne sont lus que dans une session
cloud **mono-dépôt**. Une session **multi-dépôts** démarre au-dessus des clones et
**ne lit pas** ces hooks (voir la doc *cloud-environments → What carries over*).
Ouvre donc tes sessions cloud sur `kryptos-research-handoff` seul pour que le mesh
s'active automatiquement.

## Vérifier

Dans une nouvelle session : `get_coordination_status` doit te montrer `online: true`,
et `last_heartbeat_at` doit rester frais même en idle (boucle 60 s).
