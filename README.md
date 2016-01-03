# SonarPhony

It is easy to find simple depth sounders that are interoperable and have
open-ish interfaces, but you would be hard-pressed to find a device that gives
the full water column and provides an open interface to do so.

This is a reference implementation to communicate with Vexilar SonarPhone
fish finders (http://sonarphone.mobi/).

This project, and its authors, are in no way affiliated with Vexilar. This was
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

## Notes

### Device Testing Limitations

This software has only been tested with the T-POD because it was the cheapest
one money could buy, however I suspect the protocol will be very similar on the
other variants since it has to interop with the same app. If you are using one
of the other products I would be really interested to see a full log.

### Passwords

One of the things I have not fully experimented with is how the "master"
password protocol works, and so I strictly used the default password. If you're
having trouble controlling the device or getting pings back and you changed the
password, that may be why (although I haven't attempted that specific edge case
yet).
