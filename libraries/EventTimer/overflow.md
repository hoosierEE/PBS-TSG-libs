Overflowing Timers
==================
Is it sufficient to only do a single comparison?  Let's walk through some simplified scenarios.

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
        set new expire time: (next = next + interval)
    5 expired? no: (now > next) == false
    6 expired? no: (now > next) == false
    8 expired? no: (now > next) == false
    9 expired? yes: (now > next) == true
        set new expire time: (next = next + interval)
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
        set new expire time: (next = next + interval)
        next rolls over; becomes 4 instead of 14.
        timer also rolls over, becomes 0 instead of 10.
    0 expired? no: (now > next) == false
    1 expired? no: (now > next) == false
    2 expired? no: (now > next) == false
    3 expired? no: (now > next) == false
    4 expired? yes: (now > next) == true
        set new expire time: (next = next + interval)
    5 expired? no: (now > next) == false
    ...

Again, everything works out fine.

### Start at 7 with interval of 4
Let's look at one more scenario.  What if `next` is just after an overflow?

    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    . . . . . . . . . . . x . . . . x . . . . x . . . . x
                    ^now  ^next
    7 expired? no. (now > next) == false  ...uh-oh, we have a problem!

Our single comparison is not sufficient.  We must take overflow into account.  Multiple valid approaches exist:

* use another variable and another comparison: `now > next && now < last`
* compare after possible overflow: `next - now > interval`

Using the latter approach:

    8 expired? no. (next-now > interval) == true
    9 expired? no. (next-now > interval) == true
    0 expired? no. (next-now > interval) == true
    1 expired? yes. (next-now > interval) == false
    2 expired? no. (next-now > interval) == true
    3 expired? no. (next-now > interval) == true
    4 expired? no. (next-now > interval) == true
    5 expired? no. (next-now > interval) == true
    6 expired? yes. (next-now > interval) == true
    7 expired? no. (next-now > interval) == true
    8 expired? no. (next-now > interval) == true

