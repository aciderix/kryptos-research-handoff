CLEAR , , 20000
DIM text$(26)
DATA "A."
DATA "B..."
DATA "C..."
DATA "D.."
DATA "E"
DATA "F..."
DATA "G.."
DATA "H..."
DATA "I."
DATA "J..."
DATA "K.."
DATA "L..."
DATA "M."
DATA "N."
DATA "O.."
DATA "P..."
DATA "Q..."
DATA "R.."
DATA "S.."
DATA "T"
DATA "U.."
DATA "V..."
DATA "W.."
DATA "X..."
DATA "Y..."
DATA "Z..."

FOR t = 1 TO 26
  READ text$(t)
NEXT t



OPEN "I", #1, "kryptraw.txt"
WHILE NOT EOF(1)
  LINE INPUT #1, krypt$
WEND
PRINT LEN(krypt$)
CLOSE #1
'krypt$ = "0100000100100001011000"
'Now work through each possibility to score it on position and print offset fifferences
OPEN "I", #1, "FullDict.txt"
OPEN "O", #2, "Results.txt"
PRINT #2, "Embassy,MorsePos,StartPos,EndPos,WordLen,BitLen,Score,AvePos,TotPos,Positions"
WHILE NOT EOF(1)
  INPUT #1, embassy$, morse$, length$
  PRINT embassy$
  'embassy$ = "BERLIN"
  'morse$ = "1000001001000010"

  stoppoint = LEN(krypt$) - LEN(morse$) + 1
  FOR t = 1 TO stoppoint
    score = 0
    positions$ = ""
    lastpos = -1
    totalpos = 0
    poscount = 0
    avepos = 0
    FOR l = 1 TO LEN(morse$)
      codepos$ = MID$(krypt$, t - 1 + l, 1)
      embassypos$ = MID$(morse$, l, 1)
      'PRINT codepos$, embassypos$
      IF codepos$ = embassypos$ THEN
        score = score + 1
      ELSE
        positions$ = positions$ + STR$(l + t - 1) + ","
        poscount = poscount + 1
        IF lastpos <> -1 THEN
          totalpos = totalpos + l - lastpos
          lastpos = l
        ELSE
          lastpos = l
        END IF
      END IF
    NEXT l
    postext$ = SPACE$(t)
    FOR i = 1 TO LEN(embassy$)
      postext$ = postext$ + text$(ASC(MID$(embassy$, i, 1)) - 64)
    NEXT i
    PRINT postext$
    IF poscount <> 0 THEN avepos = totalpos / poscount
    'IF t = 1 THEN
      PRINT #2, embassy$; ","; postext$; ","; STR$(t); ","; STR$(t + LEN(morse$) - 1); ","; LEN(embassy$); ","; LEN(morse$); ","; score / LEN(morse$); ","; avepos; ","; poscount; ","; positions$
    'END IF
  NEXT t
WEND
CLOSE #1
CLOSE #2

