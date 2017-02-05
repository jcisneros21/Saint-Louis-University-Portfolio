%x  		  ABBR

%%   

[\n]           printf("\n\n");
[.!?]" "       ECHO; BEGIN ABBR;
<ABBR>" "      ECHO; BEGIN(0);

<ABBR>"lol"   { printf("Laugh out loud"); BEGIN(0);}
^"lol"        { printf("Laugh out loud"); }
"lol"         { printf("laugh out loud"); }

<ABBR>"yolo"  { printf("You only live once"); BEGIN(0);}
^"yolo"       { printf("You only live once"); }
"yolo"        { printf("you only live once"); }

<ABBR>"myl"   { printf("Mind your language"); BEGIN(0);}
^"myl"        { printf("Mind your language"); }
"myl"         { printf("mind your language"); }

<ABBR>"noy"   { printf("Not online yet"); BEGIN(0);}
^"noy"        { printf("Not online yet"); }
"noy"         { printf("not online yet"); }

<ABBR>"ntb"   { printf("Not too bright"); BEGIN(0);}
^"ntb"        { printf("Not too bright"); }
"ntb"         { printf("not too bright"); }

<ABBR>"nvm"   { printf("Never mind"); BEGIN(0);}
^"nvm"        { printf("Never mind"); }
"nvm"         { printf("never mind"); }

<ABBR>"k"     { printf("Okay"); BEGIN(0);}
^"k"          { printf("Okay"); }
"k"           { printf("okay"); }

<ABBR>"oml"   { printf("Oh my lord"); BEGIN(0);}
^"oml"        { printf("Oh my lord"); }
"oml"         { printf("oh my lord"); }

<ABBR>"oos"   { printf("Out of stock"); BEGIN(0);}
^"oos"        { printf("Out of stock"); }
"oos"         { printf("out of stock"); }

<ABBR>"ott"   { printf("Over the top"); BEGIN(0);}
^"ott"        { printf("Over the top"); }
"ott"         { printf("over the top"); }

<ABBR>"pcm"   { printf("Please call me"); BEGIN(0);}
^"pcm"        { printf("Please call me"); }
"pcm"         { printf("please call me"); }

<ABBR>"brb"   { printf("Be right back"); BEGIN(0);}
^"brb"        { printf("Be right back"); }
"brb"         { printf("be right back"); }

<ABBR>"btr"   { printf("Better"); BEGIN(0);}
^"btr"        { printf("Better"); }
"btr"         { printf("better"); }

<ABBR>"fyi"   { printf("For your information"); BEGIN(0);}
^"fyi"        { printf("For your information"); }
"fyi"         { printf("for your information"); }

<ABBR>"goi"   { printf("Get over it"); BEGIN(0);}
^"goi"        { printf("Get over it"); }
"goi"         { printf("get over it"); }

<ABBR>"btw"   { printf("By the way"); BEGIN(0);}
^"btw"        { printf("By the way"); }
"btw"         { printf("by the way"); }

<ABBR>"irl"   { printf("In real life"); BEGIN(0);}
^"irl"        { printf("In real life"); }
"irl"         { printf("in real life"); }

<ABBR>"nsfw"  { printf("Not safe for work"); BEGIN(0);}
^"nsfw"       { printf("Not safe for work"); }
"nsfw"        { printf("not safe for work"); }

<ABBR>"nsfl"  { printf("Not safe for life"); BEGIN(0);}
^"nsfl"       { printf("Not safe for life"); }
"nsfl"        { printf("not safe for life"); }

<ABBR>"omg"   { printf("Oh my god"); BEGIN(0);}
^"omg"        { printf("Oh my god"); }
"omg"         { printf("oh my god"); }

%%

int main()
{
    yylex();
}