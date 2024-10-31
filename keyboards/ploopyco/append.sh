#!/usr/bin/env bash

echo '#include "../qmk_userspace/keyboards/ploopyco/ploopyco.h"' >> "../qmk_firmware/keyboards/ploopyco/ploopyco.h"
echo '#include "../qmk_userspace/keyboards/ploopyco/ploopyco.c"' >> "../qmk_firmware/keyboards/ploopyco/ploopyco.c"

echo "Ploopy source modified."
