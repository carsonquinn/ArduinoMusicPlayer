# Arduino Music Player

CMPUT-274 final project with @kenboo98

# Note (to cleanup later)

Change the NUMSAMPLES to 3 instead of 2 in touchscreen library since 3 takes
median and is better at ignoring random touches that are not read. Our touch
library won't work with ignored values since there's a 1 in 5 rate of ignoring
touches which means the touch fsm would give errors
