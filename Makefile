PKG=openmediavault-navidrome
VERSION:=$(shell dpkg-parsechangelog --show-field Version)

.PHONY: build clean lint

build:
	@dpkg-buildpackage -b -us -uc

clean:
	@dpkg-buildpackage -T clean

lint:
	@lintian ../$(PKG)_$(VERSION)_all.deb || true
