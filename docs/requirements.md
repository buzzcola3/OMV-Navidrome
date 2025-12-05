# OMV Navidrome Plugin Requirements

## Goals
- Provide an OpenMediaVault plugin that installs and manages a Navidrome music server instance on the host (no Docker dependency).
- Surface essential Navidrome settings inside the OMV web UI (e.g. enable/disable service, HTTP port, data/cache directories, log level, version selection).
- Ensure Navidrome runs as a dedicated unprivileged systemd service with persistent state in `/var/lib/navidrome` and logs in `/var/log/navidrome`.
- Integrate with OMV's configuration management (datamodels + mkconf) so that changes triggered via RPC get rendered into `/etc/navidrome/navidrome.toml` and the systemd unit reloads as needed.

## Functional Requirements
1. **Service lifecycle**
   - Create `navidrome` system group/user.
   - Ship a systemd unit `navidrome.service` managed via OMV's service framework.
   - Support start/stop/reload actions via OMV RPC.
2. **Configuration**
   - Persist plugin state under `/etc/openmediavault/config.xml` via `conf.service.navidrome` datamodel.
   - Render a TOML configuration file from a Jinja2 template using mkconf tooling.
   - Expose UI fields: enable toggle, listen address/port, music library path, data path, log level, auto-update toggle.
3. **Binary management**
   - Download Navidrome release tarball during postinst (default stable version) and place binary under `/usr/lib/navidrome/navidrome`.
   - Optionally allow custom download URL/version via CLI hook.
4. **Logging & Monitoring**
   - Write logs to `/var/log/navidrome/navidrome.log` (rotated by logrotate snippet delivered by the plugin).
   - Register service status with OMV dashboard widget via workbench datamodel.

## Non-Functional Requirements
- Target OMV 7.x on Debian 12 Bookworm.
- Package as a signed Debian package that can be built with `fakeroot debian/rules binary`.
- Follow OMV naming conventions: package `openmediavault-navidrome`.
- Keep dependencies minimal (curl, adduser, systemd).
- Provide documentation covering installation, configuration, and troubleshooting.
