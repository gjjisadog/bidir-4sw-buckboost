# VS Code Debugging (no CCS GUI)

This folder contains templates to drive build/flash/debug from VS Code using TI command-line tools instead of the CCS IDE.

## Prerequisites
- TI C2000 toolchain with GDB (e.g. c2000-elf-gdb from the GNU C2000 tools) → set `TI_C2000_GDB` env to its full path.
- TI Debug Server (ccs_base) + DSLite CLI (ships with CCS/UniFlash) → set `TI_DSLITE` env to `.../ccs_base/DebugServer/bin/DSLite.sh` or `.bat`.
- A CCXML target config for your probe/SoC (copy to `tools/vscode/F28335.ccxml` or set `TI_CCXML` env). You can start from `F28335.ccxml.example`.

## How to use
1) Copy `tools/vscode/launch.json.example` → `.vscode/launch.json` and `tools/vscode/tasks.json.example` → `.vscode/tasks.json`.
2) Edit the copied files:
   - `program`: point to your built firmware `.out`.
   - `miDebuggerPath`: set to `${env:TI_C2000_GDB}` or a hard path.
   - `TI_DSLITE`/`TI_CCXML`: set via environment or inline in tasks.
   - Replace the placeholder build command with your real build (make/cmake/etc.).
3) Run the `start-dslite-gdbserver` task to launch the GDB server, then start the `F28335 (DSLite GDB server)` debug config in VS Code.
4) When done, run `stop-dslite-gdbserver` to kill the server.

Notes:
- The repo keeps `.vscode/` ignored; keep your local launch/tasks there. The checked-in templates stay in `tools/vscode/`.
- If your DSLite version uses different flags for GDB server, adjust the args in `tasks.json` accordingly.
- CCS GUI is not required; only the command-line Debug Server components are used.
