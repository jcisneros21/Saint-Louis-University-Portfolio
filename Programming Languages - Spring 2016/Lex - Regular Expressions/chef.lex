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

WC		[A-Za-z'] // WC defines all capital and lowercase letters and as well as a apostrophe
NW		[^A-Za-z'] // NW defines everything except all capital and lowercase letters and as well as a apostrophe

%start		INW NIW // INW and NIW are start conditions

%%

\\[^ \n]+	ECHO; // echos all characters besides space and newlines no matter how many occur in a row

{NW}		{ BEGIN NIW; i_seen = 0; ECHO; } // starts NIW when NW is found, sets i_seen to 0  and echos all input
[.!?]$		{ BEGIN NIW; i_seen = 0; 
		    printf("%c\nBork Bork Bork!",yytext[0]); } // starts NIW if there is a ".", ",", or "!" before a newline occurs 
		                                               // and prints "Bork Bork Bork!" before the newline occurs.

<NIW>"bork"/{NW} ECHO; // when in the NIW state and "bork" is followed by a non-word then ECHO
<NIW>"Bork"/{NW} ECHO; // when in the NIW state and "Bork" is followed by a non-word then ECHO

// When any rules are found by the Scanner, they are stripped from input so they will not be echoed.
"an"		{ BEGIN INW; printf("un"); } // If "an" is found in Intial state then begin INW state and print "un" instead of "an"
"An"		{ BEGIN INW; printf("Un"); } // If "An" is found in Intial state then begin INW state and print "Un" instead of "An"
"au"		{ BEGIN INW; printf("oo"); } // If "au" is found in Intial state then begin INW state and print "oo" instead of "au"
"Au"		{ BEGIN INW; printf("Oo"); } // If "Au" is found in Intial state then begin INW state and print "Oo" instead of "Au"

// Both these rules are in the initial state.
"a"/{WC}	{ BEGIN INW; printf("e"); } // When "a" is found before a letter or apostrophe then begin INW state and print "e" 
"A"/{WC}	{ BEGIN INW; printf("E"); } // When "A" is found before a letter or apostrophe then begin INW state and print "E" 
"en"/{NW}	{ BEGIN INW; printf("ee"); } // When "en" is found before not a letter or apostrophe then begin INW state and print "ee" 

// each rule and action occurs in the INW or NIW state due to which state is assigned in the angle brackets.
<INW>"ew"	    { BEGIN INW; printf("oo"); } // When "ew" is found then begin INW state and print "oo" 
<INW>"e"/{NW}	{ BEGIN INW; printf("e-a"); } // When "e" is found before not a letter or apostrophe then begin INW state and print "e-a"
<NIW>"e"	    { BEGIN INW; printf("i"); } // When "e" is found in the NIW state then begin INW state and print "i" 
<NIW>"E"	    { BEGIN INW; printf("I"); } // When "E" is found in the NIW state then begin INW state and print "I" 
<INW>"f"	    { BEGIN INW; printf("ff"); } // When "f" is found in the INW state then begin INW state and print "ff" 
<INW>"ir"	    { BEGIN INW; printf("ur"); } // When "ir" is found in the INW state then begin INW state and print "ur" 
<INW>"i"	    { BEGIN INW; printf(i_seen++ ? "i" : "ee"); } // When "i" is found in the INW state then begin INW state and print "i" 
                                                              // if i_seen is more than 0 or and  print "ee" if i_seen is less than or 
                                                              // equal to 0. This statement also incements i_seen.
<INW>"ow"	    { BEGIN INW; printf("oo"); } // When "ow" is found in the INW state then begin INW state and print "oo" 
<NIW>"o"	    { BEGIN INW; printf("oo"); } // When "o" is found in the INW state then begin INW state and print "oo"
<NIW>"O"	    { BEGIN INW; printf("Oo"); } // When "O" is found in the INW state then begin INW state and print "Oo"
<INW>"o"	    { BEGIN INW; printf("u"); } // When "o" is found in the INW state then begin INW state and print "u"

"the"		{ BEGIN INW; printf("zee"); } // If "the" is found in Intial state then begin INW state and print "zee" instead of "the"
"The"		{ BEGIN INW; printf("Zee"); } // If "The" is found in Intial state then begin INW state and print "Zee" instead of "The"
"th"/{NW}	{ BEGIN INW; printf("t"); } // When "tion" is found before not a letter or apostrophe then begin INW state and print "shun" instead of "th"/{NW}
<INW>"tion"	{ BEGIN INW; printf("shun"); } // When "tion" is found in the INW state then begin INW state and print "shun" instead of "tion"
<INW>"u"	{ BEGIN INW; printf("oo"); } // When "u" is found in the INW state then begin INW state and print "oo" instead of "u"
<INW>"U"	{ BEGIN INW; printf("Oo"); } // When "U" is found in the INW state then begin INW state and print "Oo" instead of "U"
"v"		    { BEGIN INW; printf("f"); } // If "v" is found in Intial state then begin INW state and print "f" instead of "v"
"V"		    { BEGIN INW; printf("F"); } // If "V" is found in Intial state then begin INW state and print "F" instead of "V"
"w"		    { BEGIN INW; printf("v"); } // If "w" is found in Intial state then begin INW state and print "v" instead of "w"
"W"		    { BEGIN INW; printf("V"); } // If "W" is found in Intial state then begin INW state and print "V" instead of "W"

.		{ BEGIN INW; ECHO; } // If any characters are found begin INW state and ECHO any input.
