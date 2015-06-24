# Universal Device Controller

The concept of a piece of interchangeable hardware for instrumenting various real-time data acquisition needs is not new. The problem appears in many different contexts. Here, at IU Psychological and Brain Sciences, we often have need of interfacing sensors with computers in a generic way. Since we do a lot of R&D, prototype rapidly, and iterate constantly, it makes sense for us to try to not reinvent the wheel all the time.

To speed up prototyping and help focus on solving the more interesting problems, we decided to standardize. The **Universal Device Controller** is one way to do that. By standardizing a hardware format, we gain many benefits. Using a common hardware interface means labs can share devices; or we can build a stockpile of them for future use should the need arise.

## Status

The first phase of this project is standardizing the hardware. Next is standardizing the firmware so that experimenters can simply plug in the device and hook it into their software of choice, knowing that the outputs will map reliably to the inputs given. *The firmware phase is not currently implmenented*.

## Pin Map

| Pin | Function        |
|:----|----------------:|
| 1   | 5V              |
| 2   | 12V             |
| 3   | Device ID 0     |
| 4   | Device ID 1     |
| 5   | Device ID 2     |
| 6   | Device ID 3     |
| 7   | Device ID 4     |
| 8   | D0              |
| 9   | D1              |
| 10  | D2              |
| 11  | D3              |
| 12  | D4              |
| 13  | D5              |
| 14  | D6  / DAC1      |
| 15  | D7  / DAC0      |
| 16  | D8  / A7        |
| 17  | D9  / A6        |
| 18  | D10 / A5        |
| 19  | D11 / A4        |
| 20  | D12 / A3        |
| 21  | D13 / A2        |
| 22  | D14 / DAC1 / A1 |
| 23  | D15 / DAC0 / A0 |
| 24  | 3.3V            |
| 25  | GND             |

Uses a DB-25 connector.

## Related Work

[Firmata](http://firmata.org/wiki/Main_Page) is a popular protocol for communications between an Arduino and a desktop operating system (e.g. Windows, OSX, Linux). This may be a viable option in the future.
