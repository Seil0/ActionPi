[![Latest](https://img.shields.io/github/release/Seil0/ActionPi/all.svg?style=flat-square)](https://github.com/Seil0/ActionPi/releases)
[![HitCount](http://hits.dwyl.io/Seil0/ActionPi.svg)](http://hits.dwyl.io/Seil0/ActionPi)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0)

# ActionPi

ActionPi is an easy to use Action Camera for the Raspberry Pi. The full documentation can be found in our [wiki](https://github.com/Seil0/ActionPi/wiki).

Used Librarys:     
- http://www.uco.es/investiga/grupos/ava/node/40   
- http://www.airspayce.com/mikem/bcm2835/

## Installation

to build with geany add this to Build Commands:    
-  `g++ -Wall -c "%f" -l raspicam -l bcm2835 -std=c++0x`   
- if the first command doesn't work `g++ -Wall -o"%e" "%f" -l raspicam -l bcm2835 -sdt=c++14`

OSI Certified Open Source Software, ActionPi © 2016-2018 Kellerkinder www.kellerkinder.xyz
