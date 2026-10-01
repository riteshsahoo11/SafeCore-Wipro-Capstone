#!/bin/bash

set -e

if [ "$EUID" -ne 0 ]; then
    exec sudo "$0" "$@"
fi

echo "Unloading SafeCore driver..."

rmmod safecore_driver

echo "SafeCore driver unloaded."
