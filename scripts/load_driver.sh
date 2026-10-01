#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

if [ "$EUID" -ne 0 ]; then
    exec sudo "$0" "$@"
fi

if lsmod | grep -q "^safecore_driver"; then
    echo "SafeCore driver is already loaded."
else
    echo "Loading SafeCore driver..."
    insmod "$PROJECT_DIR/driver/safecore_driver.ko"
    sleep 1
fi

if [ -e /dev/safecore ]; then
    echo "SafeCore driver is ready."
    echo "Device: /dev/safecore"
else
    echo "ERROR: /dev/safecore was not found."
    echo "Check: dmesg | tail -20"
    exit 1
fi
