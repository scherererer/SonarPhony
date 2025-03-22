# SonarPhony

It is easy to find simple depth sounders that are interoperable and have
open-ish interfaces, but you would be hard-pressed to find a device that gives
the full water column and provides an open interface to do so.

This is a reference implementation to communicate with Vexilar SonarPhone
fish finders (http://sonarphone.mobi/).

This project, and its author(s), are in no way affiliated with Vexilar. This was
created for educational purposes only on the author's personal time, and is
released without any warranty whatsoever.

## Discovery Method

The protocol was reverse engineered by monitoring the packets between the
device and the manufacturer's app. Hex dumps of the captured packets were
compared against what the app displayed and was commanding to the device.
Through some trial and error, a portion of the protocol was decoded.

## Screenshots

![Screenshot 0](/../screenshots/screenshots/screenshot-0.png?raw=true)
![Screenshot 1](/../screenshots/screenshots/screenshot-1.png?raw=true)
![Screenshot 2](/../screenshots/screenshots/screenshot-2.png?raw=true)

## Usage

### Compiling

I've only built this on Linux, but it will probably work in Windows.

Requires Qt 5 or 6.

```bash
$ git clone https://github.com/scherererer/SonarPhony.git
$ cd SonarPhony
$ qmake6 -r
$ make
```

### Running

To run, put your device in the water to turn it on, or shove a bit of wet paper
cloth across the metal contacts for an in-air test (I don't recommend you do
this too much, my experience has said running these in air isn't a good thing
but it is for the consumer so it's probably fairly hardy). From your computer,
a wifi network should appear (in my case it is T-POD-37A) and enter your wifi
password as the manufacturer instructs. Then, start the program and hit the
"run" button. The screen should start filling up with data.

### Daemon

A daemon is also distributed to run in a headless mode. It has options for
sending NMEA0183 depth and sea temperature data over UDP to a chosen host. It
can also be used to log to a local directory. Log files can later be visualzed
by the GUI.

## Notes / Troubleshooting

### Device Testing Limitations

This **software has only been tested with the T-POD** because it was the
cheapest one money could buy, however I suspect the protocol will be very
similar on the other variants since it has to interop with the same app. If
you are using one of the other products I would be really interested to see a
full log.

### Passwords

One of the things I have not fully experimented with is how the "master"
password protocol works, and so I strictly used the default password. If you're
having trouble controlling the device or getting pings back and you changed the
password, that may be why (although I haven't attempted that specific edge case
yet).
