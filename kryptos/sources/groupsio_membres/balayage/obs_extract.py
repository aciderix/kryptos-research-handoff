import os, re, json, glob, collections
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
frags=set(K4[i:i+5] for i in range(len(K4)-4))
OBS=re.compile(r"\b(notic|observ|interest|curious|odd|strange|coincid|pattern|anomal|repeat|symmetr|mirror|every \d+|interval|period|column|diagonal|twice|doubl|same letter|appear|occurs|happen|significan|probab|chance|odds|statistic)\w*",re.I)
out=collections.defaultdict(list)
def scan(text, author, src):
    # découpe en phrases
    for s in re.split(r"(?<=[.!?])\s+|\n{2,}", text):
        s2=re.sub(r"\s+"," ",s).strip()
        if len(s2)<30 or len(s2)>700: continue
        up=re.sub(r"[^A-Z]","",s2.upper())
        # exclure les copies du chiffré entier
        if any(K4[i:i+25] in up for i in range(0,73,6)): continue
        words=re.findall(r"[A-Z]{5,}",s2)
        hit=[w for w in words if any(f in w for f in frags) and w not in ("BERLIN","CLOCK","BERLINCLOCK")]
        if hit and OBS.search(s2):
            out[author].append((src,s2))
base="gio/txt/Personal Folders"
for f in glob.glob(base+"/**/*.txt",recursive=True):
    author=f.split("/")[3]
    try: scan(open(f,errors="ignore").read(),author,f.split("/",3)[3])
    except Exception: pass
# archive des messages
M=json.load(open("gio/mbox.json"))
for m in M:
    scan(m.get("t",""),"[archive] "+(m.get("a","") or "?")[:30],m.get("d","")+" "+m.get("s","")[:60])
tot=sum(len(v) for v in out.values())
print(tot,"phrases,",len(out),"auteurs")
json.dump(out,open("obs_candidates.json","w"),ensure_ascii=False)
for a,v in sorted(out.items(),key=lambda x:-len(x[1]))[:60]: print(len(v),a)
