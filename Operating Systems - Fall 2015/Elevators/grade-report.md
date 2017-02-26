#TeamCG
##gaoj
##jcisneros

###Style
* Consistent indentation. Helpful comments.

###Compiliation
* No errors, no warnings

###Operation
* Passed single rider tests
* Passed multiple rider tests
* Nice use of lock + condition variable to handle the Waitlist.

You don't need the `grabbingNext` variable in `board_floor_of_first_rider_in_waitlist`. It's only set to true while the thread is holding the mutex, and so no other thread will ever see it set to true.

Otherwise, pretty simple algorithm, works very well. Nice job!

Found an issue with building, which causes the simulation to deadlock at high speeds. I haven't had a chance to try and find/fix it yet, but thanks! **[+2]**

###Score 102/100
