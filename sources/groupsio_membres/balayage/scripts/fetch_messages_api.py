# Lecture seule des messages de kryptos.groups.io par l API (24/09/2026).
# Identifiants lus dans l environnement (GROUPSIO_EMAIL, GROUPSIO_PASSWORD), jamais ecrits.
import os, json, time, requests
API = "https://groups.io/api/v1/"
S = requests.Session()
S.headers["User-Agent"] = "kryptos-research-reader/1.0"
def login():
    r = S.post(API + "login", data={"email": os.environ["GROUPSIO_EMAIL"],
                                    "password": os.environ["GROUPSIO_PASSWORD"]}, timeout=60)
    j = r.json()
    if r.status_code != 200 or j.get("object") == "error":
        raise SystemExit("login failed: %s %s" % (r.status_code, {k: j.get(k) for k in ("type", "extra")}))
    return j
def get(ep, **params):
    for attempt in range(5):
        r = S.get(API + ep, params=params, timeout=120)
        if r.status_code == 429:
            time.sleep(10 * (attempt + 1)); continue
        j = r.json()
        return j
    raise SystemExit("rate limited")

# Telecharge les messages recents (ordre decroissant) jusqu a la date `stop`.
login()
out = []
tok = None
stop = "2018-12-01"
page = 0
while True:
    params = dict(group_id=31445, limit=100, sort_dir="desc")
    if tok: params["page_token"] = tok
    j = get("getmessages", **params)
    if j.get("object") == "error":
        print("error", j.get("type")); break
    data = j.get("data", [])
    page += 1
    older = False
    for x in data:
        if x.get("created", "")[:10] < stop:
            older = True; continue
        out.append({k: x.get(k) for k in ("id", "msg_num", "topic_id", "created", "subject", "name", "user_id",
                                          "is_reply", "is_plain_text", "body", "attachments", "hashtags")})
    print(page, len(out), data[-1].get("created", "")[:10] if data else "-", flush=True)
    if older or not j.get("has_more"): break
    tok = j.get("next_page_token")
    time.sleep(1.5)
json.dump(out, open("raw_messages.json", "w"))  # brut (HTML) : a nettoyer avant versement
print("done", len(out))
