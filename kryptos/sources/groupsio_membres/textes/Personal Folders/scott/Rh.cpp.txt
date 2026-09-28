#define	K4				"OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

// The Kryptos chart

char *fullchart[27]={
" ABCDEFGHIJKLMNOPQRSTUVWXYZABCD",
"AKRYPTOSABCDEFGHIJLMNQUVWXZKRYP",
"BRYPTOSABCDEFGHIJLMNQUVWXZKRYPT",
"CYPTOSABCDEFGHIJLMNQUVWXZKRYPTO",
"DPTOSABCDEFGHIJLMNQUVWXZKRYPTOS",
"ETOSABCDEFGHIJLMNQUVWXZKRYPTOSA",
"FOSABCDEFGHIJLMNQUVWXZKRYPTOSAB",
"GSABCDEFGHIJLMNQUVWXZKRYPTOSABC",
"HABCDEFGHIJLMNQUVWXZKRYPTOSABCD",
"IBCDEFGHIJLMNQUVWXZKRYPTOSABCDE",
"JCDEFGHIJLMNQUVWXZKRYPTOSABCDEF",
"KDEFGHIJLMNQUVWXZKRYPTOSABCDEFG",
"LEFGHIJLMNQUVWXZKRYPTOSABCDEFGH",
"MFGHIJLMNQUVWXZKRYPTOSABCDEFGHI",
"NGHIJLMNQUVWXZKRYPTOSABCDEFGHIJ",
"OHIJLMNQUVWXZKRYPTOSABCDEFGHIJL",
"PIJLMNQUVWXZKRYPTOSABCDEFGHIJLM",
"QJLMNQUVWXZKRYPTOSABCDEFGHIJLMN",
"RLMNQUVWXZKRYPTOSABCDEFGHIJLMNQ",
"SMNQUVWXZKRYPTOSABCDEFGHIJLMNQU",
"TNQUVWXZKRYPTOSABCDEFGHIJLMNQUV",
"UQUVWXZKRYPTOSABCDEFGHIJLMNQUVW",
"VUVWXZKRYPTOSABCDEFGHIJLMNQUVWX",
"WVWXZKRYPTOSABCDEFGHIJLMNQUVWXZ",
"XWXZKRYPTOSABCDEFGHIJLMNQUVWXZK",
"YXZKRYPTOSABCDEFGHIJLMNQUVWXZKR",
"ZZKRYPTOSABCDEFGHIJLMNQUVWXZKRY"};

// rh is a structure containing 3 pieces of data for each position in K4.

struct rh
{
	int row;		// Column AM from spreadsheet
	int	col;		// Column AN from spreadsheet
	int	row2;		// Column AR from spreadsheet
};

struct rh rh[97]={
{1,30,1},{26,1,1},{27,31,2},{1,31,1},{1,31,2},{27,30,1},{26,1,1},{1,31,1},{1,1,1},{1,1,1},
{26,31,2},{26,2,1},{2,31,1},{26,31,1},{27,31,1},{26,2,2},{27,1,1},{26,30,2},{2,2,1},{2,31,1},
{26,2,1},{27,2,1},{1,1,2},{1,31,1},{1,1,1},{1,30,1},{27,30,2},{2,1,1},{27,2,2},{26,1,1},
{1,1,1},{1,30,1},{27,31,1},{1,2,2},{1,31,1},{26,2,1},{2,2,1},{1,31,1},{26,2,1},{26,31,2},
{2,30,1},{26,30,2},{27,1,1},{26,31,1},{1,2,1},{2,2,1},{26,2,1},{0,0,0},{26,2,1},{27,30,1},{27,31,1},
{1,30,2},{1,2,1},{2,31,2},{0,0,0},{26,1,1},{2,31,1},{27,1,1},{26,30,1},{26,2,2},{26,1,1},
{1,30,2},{1,2,1},{26,2,1},{26,2,2},{2,1,1},{26,2,1},{2,1,1},{27,31,1},{1,1,1},{2,31,2},
{26,31,1},{26,1,2},{1,1,1},{0,0,0},{26,2,2},{1,30,2},{1,30,2},{26,2,1},{1,2,2},{26,1,1},
{26,31,1},{27,2,2},{27,2,1},{2,1,2},{2,2,2},{2,30,1},{26,2,2},{26,2,1},{0,0,0},{27,30,1},
{1,30,2},{2,2,1},{26,30,1},{26,2,2},{1,30,2},{27,30,2}
};

// Takes a ciphertext letter, and returns the expected plaintext based on the 'rh' structure

char	RhDo(char ct,struct rh *rh)
{
	char	ct2;
	int		j,k,row,col,row2;	row=col=0;
		
	for (j=0,k=-1; j<31; j++) {if (fullchart[j][rh->col-1]==ct)	{row=j+1; break;}}			// Get row with CT in col I want to use
	for (j=0,k=-1; j<31; j++) {if (fullchart[rh->row-1][j]==ct)	{col=j+1; break;}}			// Get col with CT in row I want to use
	ct2=fullchart[row-1][col-1];															// ct2 is 2nd ciphertext. -1 to be 1-based

	// Take CT2 ("L") and turn into PT. Where "L" is in inrow2.

	for (j=0,k=-1; j<31; j++) {if (fullchart[rh->row2-1][j]==ct2)	{row2=j+1; break;}}			// Get row with CT in col I want to use
	//printf("%c,%d,%d,%c,%d,%c\n",ct,row,col,ct2,row2,fullchart[row-1][row2-1]);
	return fullchart[row-1][row2-1];
}



void RhMain(void)
{
	int		i,r,c,r2;
	char	pt;
	struct	rh myrh;
	int		poss[26];

	// Post #24400. OBKRUOX->LEXIFCW->HOUSTON. For each position in K4, displays the determined plaintext, the various possible characters that can be used in that position (4 sets of 8 characters).
	// Finally, for each position, a sorted list of the possible letters for that position is shown.

	for (i=0; i<97; i++)
	{
		if ((rh[i].col!=0)&&(rh[i].row!=0)&&(rh[i].row2!=0))			// Print the selected PT character per Rag
		{
			pt=RhDo(K4[i],&rh[i]);
			printf("%c,",pt);
		}
		else
		{
			printf("?,");
		}

		memset(poss,0,26*sizeof(int));

		for (r=0; r<4; r++)												// Then show all 16 possibilities
		{
			for (c=0; c<4; c++)
			{
				for (r2=0; r2<2; r2++)
				{
					if (r==0) {myrh.row=1;} else if (r==1) {myrh.row=2;} else if (r==2) {myrh.row=26;} else if (r==3) {myrh.row=27;}
					if (c==0) {myrh.col=1;} else if (c==1) {myrh.col=2;} else if (c==2) {myrh.col=30;} else if (c==3) {myrh.col=31;}
					if (r2==0) {myrh.row2=1;} else {myrh.row2=2;}
					printf("%c",RhDo(K4[i],&myrh));
					poss[RhDo(K4[i],&myrh)-'A']=1;
				}
			}

			printf(" ");
		}

		for (r=0; r<26; r++) {if (poss[r]) {printf("%c",'A'+r);}}		// Show all 16 possibilities, de-duped.

		printf("\n");
	}

	return;
}


int	main(int argc, char* argv[])
{
	RhMain();
	return 0;
}
