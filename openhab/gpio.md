Items binding
=============

####Switch / Contact

#####activelow=no 
- (or omitted) the pins behaves normally: 

output

    high    pin it will be set to   ON
    low     pin it will be set to   OFF 

input 

    OPEN    when they are           high 
    CLOSED  when are                low

#####activelow=yes 
- the logic is inverted: 

output

    ON      pin it will be set to   low 
    OFF     pin it will be set to   high. 

input 

    OPEN    event when they are     low
    CLOSED  event on                high
