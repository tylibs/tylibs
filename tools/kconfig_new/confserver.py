# SPDX-FileCopyrightText: Copyright 2025 Clever Design (Switzerland) GmbH
# SPDX-License-Identifier: Apache-2.0

import subprocess
import sys

if __name__ == "__main__":
    sys.exit(
        subprocess.run([sys.executable, "-m", "kconfserver"] + sys.argv[1:]).returncode
    )
