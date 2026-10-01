savedcmd_safecore_driver.mod := printf '%s\n'   safecore_driver.o | awk '!x[$$0]++ { print("./"$$0) }' > safecore_driver.mod
