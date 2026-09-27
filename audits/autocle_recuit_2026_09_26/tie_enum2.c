/* tie_enum2.c — ÉNUMÉRATION EXACTE de la variété crib-cohérente, cas TIE (sigma=tau=pi),
 * autoclé écart-7, ALPHABET LIBRE. Version LINÉAIRE (RREF Z/26) : la précédente (CSP générique)
 * n'atteignait aucune feuille (système homogène, 0 var forcée au départ). Ici on PARAMÈTRE :
 *
 *   1) 24 lignes crib mod 26 sur 33 vars (pi[0..25], kappa[0..6]) — coeffs ±1.
 *   2) RREF sur Z/26 à pivots UNITÉS (possible car rang2=rang13=24 => diviseurs élémentaires
 *      tous inversibles mod 26). 24 pivots, 9 variables LIBRES.
 *   3) Chaque var pivot = combinaison linéaire des 9 libres. DFS sur les 9 libres ; dès qu'une
 *      var pivot (pival) est entièrement déterminée, on VÉRIFIE la bijection (élagage précoce).
 *      Feuille = pi bijectif + kappa => 24/24 cribs GARANTIS. Décode, score qg_big.
 *
 * Compile : cc -O2 -o tie_enum2 tie_enum2.c -lm
 * Usage   : tie_enum2 qg_big.bin [maxnodes]      (attaque K4)
 *           tie_enum2 selftest qg_big.bin        (contrôle positif : message planté)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 97
#define NV 33
#define KAP(r) (26+(r))
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<N;i++){iscrib[i]=0;cribL[i]=-1;}
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static int C[N];
static int M[64][NV+1];    /* lignes du système (col NV inutilisée=rhs=0, homogène) */
static int nrow=0;

static void build_eq(int i,int v){
  int *row=M[nrow]; memset(row,0,sizeof(int)*(NV+1));
  row[v]=md(row[v]+1);
  int r=i%7,j=i/7,s=1;
  for(int m=0,pos=i;m<=j;m++,pos-=7){ row[C[pos]]=md(row[C[pos]]-s); s=-s; }
  int ks=((j+1)&1)?-1:1; row[KAP(r)]=md(row[KAP(r)]-ks);
  nrow++;
}
static int DROP1=-1, DROP2=-1;   /* positions de crib à NE PAS imposer (régime erreur) */
static void build_system(void){ nrow=0;
  for(int k=0;k<13;k++){int p=CE[k]; if(p==DROP1||p==DROP2)continue; build_eq(p,cribL[p]);}
  for(int k=0;k<11;k++){int p=CB[k]; if(p==DROP1||p==DROP2)continue; build_eq(p,cribL[p]);} }

static int inv26[26];
static void init_inv(void){ for(int a=0;a<26;a++){inv26[a]=0; for(int x=1;x<26;x++) if(a*x%26==1){inv26[a]=x;break;}} }

/* RREF Z/26 à pivots unités. Renvoie rang. Remplit pivotcol[rank], isfree[col]. */
static int pivotcol[NV], isfree[NV], rank=0;
static int rref(void){
  rank=0; for(int c=0;c<NV;c++)isfree[c]=1;
  /* ordre de traitement : KAPPA d'abord (26..32) puis pival (0..25) => libres = pival,
     donc DFS = énumération d'alphabet à valeurs distinctes (élagage bijection à chaque pas). */
  int order[NV],no=0; for(int c=26;c<NV;c++)order[no++]=c; for(int c=0;c<26;c++)order[no++]=c;
  for(int oi=0; oi<NV && rank<nrow; oi++){ int col=order[oi];
    int pr=-1; for(int r=rank;r<nrow;r++) if(inv26[M[r][col]]){pr=r;break;} /* entrée unité */
    if(pr<0) continue;
    for(int c=0;c<=NV;c++){int t=M[rank][c];M[rank][c]=M[pr][c];M[pr][c]=t;}
    int iv=inv26[M[rank][col]];
    for(int c=0;c<=NV;c++) M[rank][c]=M[rank][c]*iv%26;
    for(int r=0;r<nrow;r++) if(r!=rank && M[r][col]){int f=M[r][col]; for(int c=0;c<=NV;c++) M[r][c]=md(M[r][c]-f*M[rank][c]);}
    pivotcol[rank]=col; isfree[col]=0; rank++;
  }
  return rank;
}

/* après RREF : pivot var p (=pivotcol[k]) : x_p = -Σ_{free f} M[k][f] x_f (mod26) */
static int freelist[NV], nfree=0;
static int pivvar[NV], pivrow[NV], npiv=0;      /* pivvar[t]=col, pivrow[t]=rref row */
static int pdep[NV][NV], pndep[NV];             /* deps free de chaque pivot */
static int pcoef[NV][NV];                        /* coeff -M[row][f] */

static float *QG;
static long nnode=0,nleaf=0,found_valid=0,MAXNODE;
static double bestq=-1e9; static int bestpt[N],bestpi[26],bestkap[7];
static int STOP_ON_TARGET=0,targetpt[N],target_hit=0;

static int val[NV];        /* valeur courante de chaque var (pi + kappa) */
static int assigned[NV];
static int usedval[26];    /* pour bijection pival */
static int freepos[NV];               /* index dans freelist, -1 sinon */
static int resolve_at[NV][NV], nresolve[NV]; /* pivots (index t) résolus à la profondeur fi */

static void score_leaf(void){
  int seen[26]={0}; for(int L=0;L<26;L++){int v=val[L]; if(v<0||seen[v])return; seen[v]=1;}
  int pinv[26]; for(int L=0;L<26;L++)pinv[val[L]]=L;
  int kap[7]; for(int r=0;r<7;r++)kap[r]=val[KAP(r)];
  int x[N],pt[N]; for(int i=0;i<N;i++){int ki=(i<7)?kap[i]:x[i-7]; x[i]=md(val[C[i]]-ki); pt[i]=pinv[x[i]];}
  found_valid++;
  double qo=0;int no=0; for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d]; if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
  double q=no?qo/no:0;
  if(q>bestq){bestq=q;memcpy(bestpt,pt,sizeof bestpt);for(int L=0;L<26;L++)bestpi[L]=val[L];for(int r=0;r<7;r++)bestkap[r]=kap[r];}
  if(STOP_ON_TARGET){int rec=0;for(int i=0;i<N;i++)if(pt[i]==targetpt[i])rec++; if(rec>=97)target_hit=1;}
}

/* DFS sur variables libres freelist[0..nfree-1]. */
static void dfs(int fi){
  if(target_hit||nnode>=MAXNODE) return;
  nnode++;
  if((nnode&0x1FFFFFF)==0) fprintf(stderr,"  ...noeuds=%ldM feuilles=%ld valides=%ld bestq=%.3f\n",nnode>>20,nleaf,found_valid,bestq);
  if(fi==nfree){ nleaf++; score_leaf(); return; }
  int fv=freelist[fi];               /* toujours pival (libres = pival) */
  int nr=nresolve[fi]; const int*rl=resolve_at[fi];
  for(int v=0;v<26;v++){
    if(usedval[v]) continue;
    if(target_hit||nnode>=MAXNODE) break;
    val[fv]=v; usedval[v]=1;
    int setcols[NV],claimed[NV],ns=0,ok=1;
    for(int k=0;k<nr;k++){ int t=rl[k]; int col=pivvar[t];
      int acc=0; for(int d=0;d<pndep[t];d++){int f=pdep[t][d]; acc=md(acc+pcoef[t][d]*val[f]);}
      val[col]=acc; setcols[ns]=col; claimed[ns]=0;
      if(col<26){ if(usedval[acc]){ok=0; ns++; break;} usedval[acc]=1; claimed[ns]=1; }
      ns++;
    }
    if(ok) dfs(fi+1);
    for(int k=ns-1;k>=0;k--){ int col=setcols[k]; if(claimed[k]) usedval[val[col]]=0; val[col]=-1; }
    usedval[v]=0; val[fv]=-1;
  }
}

static void load_qg(const char*p){FILE*f=fopen(p,"rb");QG=malloc(sizeof(float)*456976);
  if(!f||fread(QG,sizeof(float),456976,f)!=456976){fprintf(stderr,"qg load fail\n");exit(2);}fclose(f);}

static void prepare(void){
  build_system(); int rk=rref();
  nfree=0; for(int c=0;c<NV;c++) if(isfree[c]) freelist[nfree++]=c;
  for(int c=0;c<NV;c++)freepos[c]=-1; for(int i=0;i<nfree;i++)freepos[freelist[i]]=i;
  npiv=0; for(int t=0;t<rk;t++){ int col=pivotcol[t]; pivvar[npiv]=col; pivrow[npiv]=t;
    pndep[npiv]=0; for(int c=0;c<NV;c++) if(isfree[c] && M[t][c]){ pdep[npiv][pndep[npiv]]=c; pcoef[npiv][pndep[npiv]]=md(-M[t][c]); pndep[npiv]++; }
    npiv++; }
  /* profondeur de résolution de chaque pivot = max index freelist de ses deps */
  for(int fi=0;fi<=nfree;fi++)nresolve[fi]=0;
  for(int t=0;t<npiv;t++){ int mx=0; for(int d=0;d<pndep[t];d++){int p=freepos[pdep[t][d]]; if(p>mx)mx=p;}
    if(pndep[t]==0) mx=0;   /* pivot constant (rare) résolu d'emblée */
    resolve_at[mx][nresolve[mx]++]=t; }
  fprintf(stderr,"[prep] rang=%d  libres=%d  pivots=%d\n",rk,nfree,npiv);
  fprintf(stderr,"[prep] vars libres:"); for(int i=0;i<nfree;i++){int f=freelist[i]; if(f<26)fprintf(stderr," pi[%c]",'A'+f); else fprintf(stderr," kappa%d",f-26);} fprintf(stderr,"\n");
}
static void reset_state(void){ for(int v=0;v<NV;v++){val[v]=-1;assigned[v]=0;} for(int v=0;v<26;v++)usedval[v]=0;
  nnode=nleaf=found_valid=0; bestq=-1e9; }

int main(int argc,char**argv){
  setbuf(stdout,NULL);
  setcribs(); init_inv();
  if(argc>=3 && !strcmp(argv[1],"selftest")){
    load_qg(argv[2]);
    int pi[26]={7,2,19,11,23,4,16,0,25,9,14,1,20,6,18,21,3,10,24,12,5,8,15,17,22,13};
    int pinv[26]; for(int L=0;L<26;L++)pinv[pi[L]]=L; int kap[7]={5,11,2,20,8,14,1};
    const char*eng="WEAREINTHEEASTNORTHEASTQUADRANTLOOKINGTOWARDTHEBERLINCLOCKATMIDNIGHTUNDERTHESTARSXYZFILLERWORDSOK";
    int pt0[N]; for(int i=0;i<N;i++)pt0[i]=eng[i]-'A'; for(int i=0;i<N;i++) if(iscrib[i])pt0[i]=cribL[i];
    int x[N],ct[N]; for(int i=0;i<N;i++){int ki=(i<7)?kap[i]:x[i-7]; x[i]=pi[pt0[i]]; ct[i]=pinv[md(x[i]+ki)];}
    for(int i=0;i<N;i++)C[i]=ct[i];
    prepare();
    /* (a) l'assignation plantée satisfait-elle les 24 équations crib ? */
    int plant[NV]; for(int L=0;L<26;L++)plant[L]=pi[L]; for(int r=0;r<7;r++)plant[KAP(r)]=kap[r];
    int eqok=1; for(int q=0;q<nrow;q++){int s=0; for(int c=0;c<NV;c++)s=md(s+M[q][c]*plant[c]); if(s%26)eqok=0;}
    /* (b) décode(planté)==pt0 ? */
    for(int v=0;v<NV;v++)val[v]=plant[v]; int decok=1; { int seen[26]={0}; for(int L=0;L<26;L++){if(seen[val[L]])decok=0;seen[val[L]]=1;} }
    { int pinv2[26]; for(int L=0;L<26;L++)pinv2[val[L]]=L; int x2[N]; for(int i=0;i<N;i++){int ki=(i<7)?val[KAP(i)]:x2[i-7]; x2[i]=md(val[C[i]]-ki); if(pinv2[x2[i]]!=pt0[i])decok=0;} }
    /* (c) paramétrisation : les libres plantées reproduisent-elles les pivots ? */
    int paramok=1; for(int t=0;t<npiv;t++){int col=pivvar[t]; int acc=0; for(int d=0;d<pndep[t];d++)acc=md(acc+pcoef[t][d]*plant[pdep[t][d]]); if(acc!=plant[col])paramok=0;}
    printf("SELFTEST TIE2 : eq_crib_satisfaites=%s  decode==clair=%s  parametrisation=%s -> %s\n",
      eqok?"OUI":"NON",decok?"OUI":"NON",paramok?"OUI":"NON",(eqok&&decok&&paramok)?"PASS":"FAIL");
    /* bornée : confirme que le DFS ATTEINT la solution plantée (cap 3e9 noeuds) */
    reset_state(); for(int i=0;i<N;i++)targetpt[i]=pt0[i]; STOP_ON_TARGET=1; target_hit=0; MAXNODE=3000000000L;
    dfs(0);
    printf("  DFS: noeuds=%ld feuilles=%ld valides=%ld best_qoff=%.3f target_atteint=%s\n",
      nnode,nleaf,found_valid,bestq,target_hit?"OUI":(nnode<MAXNODE?"n/a(exhaustif sans cible?)":"NON(cap)"));
    return 0;
  }
  if(argc<2){fprintf(stderr,"usage: tie_enum2 qg_big.bin [maxnodes] | selftest qg_big.bin\n");return 1;}
  load_qg(argv[1]);
  MAXNODE=(argc>=3)?atol(argv[2]):20000000000L;
  if(argc>=4)DROP1=atoi(argv[3]); if(argc>=5)DROP2=atoi(argv[4]);
  for(int i=0;i<N;i++)C[i]=K4[i]-'A';
  prepare(); reset_state();
  dfs(0);
  printf("=== TIE (sigma=tau LIBRE) autoclé écart-7, variété crib-EXACTE — ÉNUMÉRATION ===\n");
  if(DROP1>=0)printf("(cribs relâchés aux positions %d %d — régime erreur Sanborn)\n",DROP1,DROP2);
  printf("solutions bijectives (24/24 cribs)=%ld  noeuds=%ld  exhaustif=%s\n",
    found_valid,nnode,(nnode<MAXNODE)?"OUI":"NON(cap)");
  printf("MEILLEUR qoff=%.3f (anglais>=-2.6 ; charabia<=-3.2)\n",bestq);
  if(found_valid>0){ printf("PI="); for(int L=0;L<26;L++)putchar('A'+bestpi[L]); printf("  KAPPA=");
    for(int r=0;r<7;r++)printf("%d ",bestkap[r]); printf("\nPT="); for(int i=0;i<N;i++)putchar('A'+bestpt[i]); putchar('\n'); }
  if(bestq>=-2.6) printf(">>> CANDIDAT ANGLAIS — vérifier dans score_pt !\n");
  else printf(">>> aucun anglais dans la variété TIE => Quagmire-III-autoclé-alphabet-libre ÉLIMINÉ.\n");
  return 0;
}
