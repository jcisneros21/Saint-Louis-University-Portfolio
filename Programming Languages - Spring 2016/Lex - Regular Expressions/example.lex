%{
/* chef.x - convert English on stdin to Mock Swedish on stdout
 *
 * The WC definition matches any word character, and the NW definition matches
 * any non-word character.  Two start conditions are maintained: INW (in word)
 * and NIW (not in word).  The first rule passes TeX commands without change.
 *
 * HISTORY
 *
 * Apr 26, 1993; John Hagerman: Added ! and ? to the Bork Bork Bork rule.
 *
 * Apr 15, 1992; John Hagerman: Created.
 */

static int i_seen = 0;
%}

WC		[A-Za-z'] 
NW		[^A-Za-z'] 

%start		INW NIW 

%%

\\[^ \n]+	ECHO; 

{NW}		{ BEGIN NIW; i_seen = 0; ECHO; } 
[.!?]$		{ BEGIN NIW; i_seen = 0;                 
		  printf("%c\nBork Bork Bork!",yytext[0]); } 

<NIW>"bork"/{NW} ECHO; 
<NIW>"Bork"/{NW} ECHO;

"an"		{ BEGIN INW; printf("un"); }   

%%

int main()
{
	yylex();
}