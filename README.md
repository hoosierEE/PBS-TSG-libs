PBS-TSG-libs
============
These libraries help me do less wheel-reinventing.

Installation
------------

```sh
$ git clone https://github.com/hoosierEE/PBS-TSG-libs.git
```

Usage (PlatformIO)
------------------
Right now I'm digging [PlatformIO](http://platformio.org/) for developing on the command line.  If you use the Arduino IDE, you can copy libraries out of here and into your own Arduino libraries folder.

Example usage (Teensy 3.1 with Arduino framework)

```sh
$ pip install platformio
$ mkdir foo && cd foo
$ platformio init --board=teensy31
$ # write some code in src/foo.ino
$ # add some librarires in lib/
$ cat src/foo.ino
#include "../lib/SomeLibrary.h"

SomeLibrary x;
setup(){}
loop(){}

$ platformio run  # compiles and uploads to the Teensy
```

Caveats
-------
These libraries are under active development and may change at any time; use with caution.

*Note: if you have a library already installed with the same name, it will cause an error.*
