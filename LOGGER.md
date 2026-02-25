# SonarPhony Logger

SonarPhony can be used as an autonomous logger on a Raspberry Pi 3b or newer model. Ensure your pi has been updated with the latest version of Raspbian OS or Ubuntu available.

## Sonar Logging

### Ubuntu or Raspbian Install

```bash
% Install cmake and build-essential
sudo apt install -y git cmake build-essential

% Install required Qt6 libraries
sudo apt install -y qt6-base-dev qt6-base-dev-tools libqt6network6

% Create a directory to install the software to
sudo mkdir /opt/sonarphony
sudo chmod a+rw /opt/sonarphony
cd /opt/sonarphony
mkdir logs

% clone SonorPhony directory and use cmake to build
git clone https://github.com/scherererer/SonarPhony.git
cd SonarPhony
mkdir build
cd build
cmake ..

% Specific to the Raspberry Pi 3 or other ram/core limited devices
make -j2
```

### Configure WiFi

Edit your netplan configuration, nominally at `/etc/netplan/50-cloud-init.yaml` to connect to the network for your sonar. An example is provided below. Be sure to change to match the SSID of your sonar's wifi network -- in this example, the SSID was "T-BOX-DE8".

*WARNING: Think about how you're connecting, this will change your network settings for both your ethernet and your wifi. Ensure you're not breaking the IP of your device*

```yaml
network:
  version: 2
  ethernets:
    eth0:
      ignore-carrier: true
      addresses:
        - 10.1.1.218/24
      routes:
        - to: default
          via: 10.1.1.1
      match:
        macaddress: 2c:cf:67:88:2c:95
      nameservers:
        search:
          - "mycompany.local"
        addresses:
          - 8.8.8.8
          - 10.1.1.1
  wifis:
    wlan0:
      optional: true
      dhcp4: true
      dhcp6: false
      access-points:
        "T-BOX-DE8":
          password: "12345678"
      dhcp4-overrides:
        use-routes: false
```

And apply the settings:

```bash
sudo netplan try
```

### Configure to run-on-boot

Create a systemd unit file, this will start the software on boot.

```bash
sudo nano /etc/systemd/system/sonarphonyd.service
```

With the following contents. This will log data to `/opt/sonarphony/logs` and will broadcrast UDP traffic on port 26026.

```
[Unit]
Description=SonarPhonyD
Wants=time-sync.target
After=network.target time-sync.target

[Service]
Type=simple
User=uncrewed
Group=uncrewed
ExecStart=/opt/sonarphony/SonarPhony/apps/sonarphonyd/sonarphonyd -p 26026 -r /opt/sonarphony/logs
RemainAfterExit=no
Restart=on-failure
RestartSec=2s

[Install]
WantedBy=default.target
```

Now enable the service

```bash
sudo systemctl enable sonarphonyd
sudo systemctl start sonarphonyd
```

## GPS Logging and Basic Dashboard

GPS logging can be implemented with tools such as OpenRVDAS

https://www.oceandatatools.org/openrvdas-docs/

## Time Synchronization

Good logging requires good timekeeping. The Raspberry Pi by default requires some extra work to use its real-time clock, which is covered in many other tutorials. RTC ensures that when power is lost to the pi, it will continue to keep time. This is especially relevant to loggers which might have intermittent access to internet to sync time.

https://learn.adafruit.com/adding-a-real-time-clock-to-raspberry-pi/
https://www.digikey.com/en/maker/projects/adding-a-real-time-clock-to-raspberry-pi/1d500968ed044a6792497a63d0d7c018

A barely cheaper, but possibly less reliable method, is to synchronize time from your USB GPS. The easiest way to do this, and still allow the GPS to be logged, is to use GPSD with Chrony or NTPD. GPSD is capable of sending your GPS data to both the time synchronization service (e.g. Chrony) and to the gps logger (e.g. OpenRVDAS). Note that while using a cheap USB GPS, you will not have a high-accuracy 1PPS sync line, so this isn't the most accurate time source.

https://gpsd.io/gpsd-time-service-howto.html

## Restart & go!

