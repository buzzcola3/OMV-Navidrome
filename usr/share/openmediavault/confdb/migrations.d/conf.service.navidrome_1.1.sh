#!/usr/bin/env bash
set -e

. /usr/share/openmediavault/scripts/helper-functions

SERVICE_PATH="/config/services/navidrome"

if omv_config_exists "${SERVICE_PATH}"; then
	omv_config_add_key "${SERVICE_PATH}" "music_sharedfolderref" ""
	omv_config_add_key "${SERVICE_PATH}" "data_sharedfolderref" ""
fi

exit 0
