#TeamCG
##gaoj
##jcisneros

###Style
* Consistent indentation. Helpful comments.

###Compilation
* No errors, no warnings

###Operation
* Correctly fetching and returning simple web requests.
* Handles non-existent hosts properly with a 404 Not Found message.
* Handles empty Host: header
* Handles missing Host: header with a 400 Bad Request message.
* Web surfing works well!

####Optional extensions
* You've got enough complication going with your command line arguments that you should probably invest the time to learn how to use `getopt`, which automates the process.
* It would be (very) helpful if your usage statement showed the usage for the optional arguments.
* Redirects to `hello.com` when hosts aren't found.  Seems to work well!  But now I really want to know what Hello is going to be.  I want to show the world who I am, express what I love, and create meaningful connections with people who share my passions.  Presumably in a less crass way than with twitter.
* Gives 404 not found for all non-.edu sites. Nice! Though you might have noticed that many sites use `https` these days and so bypass the proxy server and it's censorship.
* With .edu, your `copyhostname` variable is not large enough (by one), so you've got a buffer overflow problem which in all likelihood means a talented hacker could pwn your whole proxy server.
Nice job on these.  Let's go **[+10]** for the pair, even with the buffer bug.

###Score 110/100
