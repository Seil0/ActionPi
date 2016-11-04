#!/bin/bash
#install script ActionPi by Seil0
#version 0.1
echo "ActionPi install script by Seil0 version 0.1"

cd ~/Downloads/
echo "downloading and installing raspicam c++ lib ..."
wget -O raspicam_c++-lib.zip https://sourceforge.net/projects/raspicam/files/raspicam-0.1.3.zip/download#
unzip raspicam_c++-lib.zip
cd raspicam-0.1.3/
cmake install
mkdir build
cd build/
cmake ..
sudo make install
sudo ldconfig

echo "finished installing raspicam c++ lib, start downloading bcm2835 lib ..."
wget -O bcm2835-1.50.tar.gz http://www.airspayce.com/mikem/bcm2835/bcm2835-1.50.tar.gz
tar zxvf bcm2835-1.50.tar.gz
cd bcm2835-1.50/
./configure
make
sudo make check
sudo make install
echo "finished installing bcm2835 lib, removing unused files ..."
cd ~/Downloads/
sudo rm raspicam_c++-lib.zip
sudo rm -r raspicam-0.1.3

echo "starting to compile ActionPi Software ..."
cd ActionPi-master
g++ -Wall ActionPi.cpp -oActionPi -l raspicam -l bcm2835 -std=c++0x
mkdir ~/ActionPi
mv -i ActionPi ~/ActionPi/ActionPi
cd ~/ActionPi/
chmod +x ActionPi

echo "Do you wish to autostart the ActionPi Software?"
echo "!WARNING! this is not tested use only if you know wath you do!"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) sudo echo "sudo ~/ActionPi./ActionPi" /etc/rc.local; break;;
        No ) break;;
    esac
done

echo "Do you wish to add i2c configuration for real time clock?"
echo "!WARNING! this is not tested use only if you know wath you do!"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) sudo i2cdetect -y 1; sudo modprobe rtc-ds1307; sudo echo ds1307 0x68 > /sys/class/i2c-adapter/i2c-1/new_device; break;;
        No ) break;;
    esac
done

echo "Do you wish to deactivate dhcpcd?"
echo "!WARNING! this is not tested use only if you know wath you do!"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) sudo service dhcpcd stop; sudo systemctl disable dhcpcd break;;
        No ) break;;
    esac
done

echo "Installation completed!"

echo "Would you like to reboot now (recommenden)?"
echo "!WARNING! this is not tested use only if you know wath you do!"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) sudo reboot; break;;
        No ) break;;
    esac
done