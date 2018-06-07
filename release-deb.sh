#!/bin/bash

DEB_BUILD_OPTIONS="nostrip noopt" fakeroot ./debian/DEBIAN/rules install

dpkg-deb -b debian pbs-licenses_$(echo $(grep "Version:" debian/DEBIAN/control) | awk '{print $2}')_amd64.deb
