# openmediavault-navidrome

> ⚠️ This plugin was generated end-to-end by AI. It has been smoke-tested in a clean OMV 7 VM and *should* work, but there has been no human fine-tuning—review the code before trusting it in production.

- Simple OpenMediaVault (OMV) plugin that installs the latest Navidrome build, writes `navidrome.toml`, and manages the systemd unit.

## Requirements
- OMV 7.x on Debian 12 Bookworm with outbound HTTPS access to `github.com`.
- Core dependencies (`curl`, `ca-certificates`, `python3`, `ffmpeg`) are pulled in automatically.

## Build & Install
```bash
# Build inside the repo
make build

# On your OMV host
sudo dpkg -i ../openmediavault-navidrome_<version>_all.deb
```

Setup creates the `navidrome` user, fetches the binary, seeds `conf.service.navidrome`, and renders `/etc/navidrome/navidrome.toml` with safe defaults. After installation, run `sudo omv-salt deploy run webgui` once so Workbench picks up the new Navidrome manifests.

## Configuration & Usage
You can manage settings from the OMV Workbench UI (Services → Navidrome). The form now uses OMV shared folders for both the music library and the Navidrome data directory, so make sure those shared folders exist under Storage → Shared Folders before opening the service page. Until that UI is available, the CLI offers the same workflow:

```bash
# Inspect current settings
omv-rpc -u admin -p openmediavault navidrome get

# Apply updates
omv-rpc -u admin -p openmediavault navidrome set '{
	"enable":true,
	"listen_address":"0.0.0.0",
	"port":4533,
	"music_directory":"/srv/dev-disk-by-label-media/music",
	"data_directory":"/var/lib/navidrome",
	"log_level":"info",
	"auto_update":true,
	"navidrome_version":"latest"
}'

# ...or target specific shared folders by UUID
omv-rpc -u admin -p openmediavault navidrome set '{
	"music_sharedfolderref":"d4c5a458-...",
	"data_sharedfolderref":"f13b2e3d-..."
}'

**Legacy note:** the older `{"config":{...}}` payload still works for backward compatibility.

# Verify service
systemctl status navidrome
curl -I http://<omv-host>:4533/
```

- Tips:
- In the UI pick existing shared folders for both the music library and the Navidrome data set; the CLI can still set `music_directory`/`data_directory` directly if you prefer raw paths.
- Both directory fields start blank—set them (or their shared folder counterparts) before toggling Enable on.
- Set `navidrome_version` to something like `0.58.5` if you need to pin; `latest` tracks upstream.
- Export `NAVIDROME_ARCH=<arch>` before running `/usr/lib/navidrome/fetch-navidrome.sh` when cross-testing.

## Development & Release Workflow
1. Bump `debian/changelog` and tag the repo as needed.
2. Run `make build` followed by `make lint` (or `lintian ../openmediavault-navidrome_<ver>_all.deb`) to catch packaging regressions.
3. Install the `all.deb` on a clean OMV test node, exercise enable/disable flows, and run `omv-rpc navidrome get/set` smoke tests.
4. Push a tag like `git tag v0.1.0 && git push origin v0.1.0`; the GitHub Actions workflow builds the package and attaches the `.deb`, `.changes`, and `.buildinfo` files to the release automatically (check `.github/workflows/release.yml`).

## Dev loop
1. Update `debian/changelog` and tag versions as usual.
2. `make build && lintian ../openmediavault-navidrome_<ver>_all.deb` to vet packaging.
3. Install on a clean OMV box, flip `enable` on/off via `omv-rpc`, and confirm `systemctl status navidrome` reacts.
4. Ship the `.deb`, `.buildinfo`, and `.changes` artifacts (sign them if you host an APT repo).

Happy hacking, and double-check things—it’s AI code after all.
