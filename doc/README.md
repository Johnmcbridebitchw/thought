Thought Core 0.18.1
=====================

This is the official reference wallet for Thought digital currency and
comprises the backbone of the Thought peer-to-peer network. You can [download
Thought Core](https://github.com/thoughtnetwork/thought/releases) or [build it
yourself](#building) using the guides below.

Running
---------------------
The following are some helpful notes on how to run Thought on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/thought-qt` (GUI) or
- `bin/thoughtd` (headless)

### Windows

Unpack the files into a directory, and then run thought-qt.exe.

### OS X

Drag Thought-Qt to your applications folder, and then run Thought-Qt.

### Need Help?

* See the [Thought website](https://thought.live) for more information about
Thought Network.
* See the other documents in this directory for help and more information.

Building
---------------------
The following are developer notes on how to build Thought Core on your native
platform. They are not complete guides, but include notes on the necessary
libraries, compile flags, etc.

- [OS X Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- Open/FreeBSD Build Notes - Soon!
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The Thought Core repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- Source Code Documentation ***TODO***
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [Travis CI](travis-ci.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Read the original whitepaper on [our website](https://thought.live).
* Discuss Thought with our community on [our Telegram](https://t.me/ThoughtAI).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).

This product includes software developed by the OpenSSL Project for use in the
[OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young
([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by
Thomas Bernard.
