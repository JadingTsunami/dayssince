# dayssince

A small utility program to save a single date and tell you how many days since/until that date.

Useful for recurring items that require a number of days in between changes vs. a fixed date of the month, or for knowing how long until a future date.

For fixed events or future dates, you can just type it into [Google](https://www.google.com/search?q=how+many+days+until+winter) nowadays, so this is mostly useful as a multi-day timer.


## Build

Just run `make`.

Recommended to rename the binary or alias to `ds` so you don't have to type `dayssince` every time.


## Usage

2 modes, 3 operations:

1. Timer Mode
    1. `dayssince`: How long since last reset?
    2. `dayssince reset`: Write a file with the current time for comparison later on (writes a file named `time.dump` in the current working directory).
2. Comparison mode
    1. `dayssince MM DD YY[YY] [HH MM SS]`: Time from now until/since given time.

