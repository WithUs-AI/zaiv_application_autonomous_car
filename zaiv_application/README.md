This program contains app to inference from video using ZAiV board basicaly.
# Setup Environment
#### Boost
* Remove all installed Boost
```bash
sudo apt-get remove libboost*
sudo apt-get purge libboost*
```
* Download
```bash
wget -c 'https://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.bz2'
tar -xvf download
```
* Build
```bash
cd boost_1_54_0
sudo ./bootstrap.sh
# Boost will be Installed to /usr/local/include, /usr/local/lib
sudo ./b2 install
```
* Trouble Shooting Case
    * bootstrap - Failed to build syntex error
    ```
    Building Boost.Build engine with toolset gcc... tools/build/v2/engine/bin.linux/b2
    Detecting Python version... 3.9
    Detecting Python root...   File "<string>", line 1
        import sys; print sys.prefix
                        ^
    SyntaxError: invalid syntax
    Unicode/ICU support for Boost.Regex?... /usr
    Generating Boost.Build configuration in project-config.jam...
    Bootstrapping is done. To build, run:
        ./b2
        
    To adjust configuration, edit 'project-config.jam'.
    Further information:
    - Command line help:
        ./b2 --help
        
    - Getting started guide: 
        http://www.boost.org/more/getting_started/unix-variants.html
        
    - Boost.Build documentation:
        http://www.boost.org/boost-build2/doc/html/index.html
    ```
    Python version change to 2.7 to solve.
    That is not interpriting on python version 3.x.
    
    Please refer below process.
    ```bash
    sudo update-alternatives --config python
    # if there is no candidate of alternatives.
    # please check python version using "ls -al /usr/bin | grep python"
    # And then install an installed python path to alternatives as like below.
    # If there is no python 2.x. Install python 2.7 through a command(sudo apt install python2.7)
    $ sudo update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
    $ sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.9 2
    # Select python version.
    $ sudo update-alternatives --config python
    There are 2 choices for the alternative python (providing /usr/bin/python).
    Selection    Path                Priority   Status
    ------------------------------------------------------------
    * 0            /usr/bin/python3.9   2         auto mode
    1            /usr/bin/python2.7   1         manual mode
    2            /usr/bin/python3.9   2         manual mode
    Press <enter> to keep the current choice[*], or type selection number: 1
    ```
#### OpenSSL
```bash
sudo apt-get install -y openssl
apt-get install libssl-dev
```
#### Node-Red
* Install Node-RED
```bash
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```
> Please enter "Y" below questions.
> * Are you really sure you want to do this ? [y/N] ? Y
> * Would you like to install the Pi-specific nodes ? [y/N] ? Y
* Node-RED Commands
    * Start: node-red-start
    * Restart: node-red-restart
    * Stop: node-red-stop
    * Log: node-red-log
* Node-RED Connect
    * Editor: http://node-red의IP:1880
    * Dashboard: http://node-red의IP:1880/ui
* ETC
    * Auto run
    ```bash
    sudo systemctl enable nodered.service
    ```
# Project Build
Build using the CMake.
```bash
cd build
cmake ..
make
```
# Usage
```bash
# In Build Directory.
# Copy zaiv_application configuration file to build directory. 
# And then configure items of ./zaiv_application.cfg
cp ../zaiv_application.cfg.bak ./zaiv_application.cfg
# Run!
./zaiv_application
```
# Licenses
MIT License
Copyright (c) 2023 Withus
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
## About
Copyright © 2023 Withus. All rights reserved.
<http://with-us.kr>
> # Hailo Ethernet DHCP 테스트
> 
> * ethernet hailo's `config.json`
> ```
> {
>     "network": {
>         "should_use_dhcp": true,
>         "rx_pause_frames_enable": true
>     },
>     "logger": {
>         "send_via_pci": false,
>         "send_via_uart": true
>     }
> }
> ```
> 
> ## linux setting
> 
> * /etc/dhcpcd.conf
> ```
> interface eth1
> static ip_address=10.3.0.1/24
> ```
> 
> * /etc/dnsmasq.conf
> ```
> interface=eth1
> dhcp-range=10.3.0.2,10.3.0.10,255.255.255.0,24h
> ```