# ActionPi

ActionPi is an easy to use Action Camera for the Raspberry Pi.

Used Librarys:     
- http://www.uco.es/investiga/grupos/ava/node/40   
- http://www.airspayce.com/mikem/bcm2835/

## Installation

to build with geany add this to Build Commands:    
-  `g++ -Wall -c "%f" -l raspicam -l bcm2835 -std=c++0x`   
- if the first command doesn't work `g++ -Wall -o"%e" "%f" -l raspicam -l bcm2835 -sdt=c++14`

ActionPi © 2016 Software Development Kellerkinder (SDK)     
www.kellerkinder.xyz
