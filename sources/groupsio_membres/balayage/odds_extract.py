import json,re,glob,collections
PAT=re.compile(r"(\b1\s*(in|:|out of)\s*[\d,\.]+\s*(million|billion|thousand)?|\bodds\b|probabilit|p\s*[=<]\s*0?\.\d|\bchance of\b|standard deviations?|sigma|z-?score|statistically significant)",re.I)
K4CTX=re.compile(r"\bK4\b|part 4|fourth section|OBKR|NYPVTT|BERLIN|doubl|ciphertext",re.I)
out=[]
def scan(text,who,src):
    for s in re.split(r"(?<=[.!?])\s+|\n{2,}",text):
        s2=re.sub(r"\s+"," ",s).strip()
        if 40<len(s2)<600 and PAT.search(s2) and K4CTX.search(s2):
            out.append((who,src,s2))
for f in glob.glob("gio/txt/Personal Folders/**/*.txt",recursive=True):
    try: scan(open(f,errors="ignore").read(),f.split("/")[3],f.split("/",3)[3][:50])
    except: pass
M=json.load(open("gio/mbox.json"))
for m in M: scan(m.get("t",""),"[archive]",m.get("d","")+" "+m.get("s","")[:50])
seen=set(); res=[]
for w,src,s in out:
    k=s[:100]
    if k in seen: continue
    seen.add(k); res.append((w,src,s))
print(len(res))
with open("odds_candidates.txt","w") as fo:
    EM=re.compile(r'[\w.+-]+@[\w.-]+')
    for i,(w,src,s) in enumerate(res):
        t=EM.sub('[courriel]',s)[:420]
        fo.write("[%d] %s | %s | %s\n"%(i,w[:22],src,t))
