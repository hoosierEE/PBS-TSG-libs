Overflowing Timers
==================
For polled timers, how can we know when the timer expires?  This document outlines an approach to the problem.

### start at 0 with an interval of 4
next is at `0 + interval == 4`; poll every `1` interval; overflow is from `9` to `0`

    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    . . . . x . . . . x . . . . x . . . . x . . . . x . . .
    ^now    ^next                             ^polling

Step through the sequence:

    0 expired? no: (now > next) == false
    1 expired? no: (now > next) == false
    2 expired? no: (now > next) == false
    3 expired? no: (now > next) == false
    4 expired? yes: (now > next) == true
        set new expire time: (next += interval)
    5 expired? no: (now > next) == false
    6 expired? no: (now > next) == false
    8 expired? no: (now > next) == false
    9 expired? yes: (now > next) == true
        set new expire time: (next += interval)
        next rolls over; becomes 4 instead of 14.
        timer also rolls over, becomes 0 instead of 10.
    0 expired? no: (now > next) == false
    ...

Here, only one comparison is required, even though overflow occurred.  This is in part because *all* of our variables overflowed at the same value.  But what if we started at a different time?

### start at 5 with an interval of 4
poll is at `5 + interval == 9`

    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    . . . . . . . . . x . . . . x . . . . x . . . . x
              ^now    ^next

    5 expired? no: (now > next) == false
    6 expired? no: (now > next) == false
    8 expired? no: (now > next) == false
    9 expired? yes: (now > next) == true
        set new expire time: (next += interval)
        next rolls over; becomes 4 instead of 14.
        timer also rolls over, becomes 0 instead of 10.
    0 expired? no: (now > next) == false
    1 expired? no: (now > next) == false
    2 expired? no: (now > next) == false
    3 expired? no: (now > next) == false
    4 expired? yes: (now > next) == true
        set new expire time: (next += interval)
    5 expired? no: (now > next) == false
    ...

Again, everything works out fine.

### Start at 7 with interval of 4
Let's look at one more scenario.  What if `next` is just after an overflow?

    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    . . . . . . . . . . . x . . . . x . . . . x . . . . x
                    ^now  ^next
    7 expired? no. (now > next) == false  ...uh-oh, we have a problem!

A single comparison is not sufficient because it does not take overflow into account.  Multiple approaches exist:

* use another variable and another comparison: `now > next AND now < last`
* compare after possible overflow: `next - now > interval`

Using the latter approach:

    8 expired? no. (next-now > interval) == false
    9 expired? no. (next-now > interval) == false
    0 expired? no. (next-now > interval) == false
    1 expired? yes. (next-now > interval) == true
        set new expire time: (next += interval)
    2 expired? no. (next-now > interval) == false
    3 expired? no. (next-now > interval) == false
    4 expired? no. (next-now > interval) == false
    5 expired? no. (next-now > interval) == false
    6 expired? yes. (next-now > interval) == true
        set new expire time: (next += interval)
    2 expired? no. (next-now > interval) == false
    7 expired? no. (next-now > interval) == false
    8 expired? no. (next-now > interval) == false
    ...

