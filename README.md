GearForge

GearForge is a terminal-based C++ application for calculating spur gear parameters, aiding machine shops in milling spur gears using horizontal milling machines. It supports gear calculations, involute cutter selection, dividing head instructions (40:1), and CSV data handling, all with a user-friendly TUI using ANSI/ASCII art and colors.

## Features

Calculate gear parameters (N, DP, M, PD, OD, RD, etc.)
Select involute gear cutters
Generate dividing head instructions
Terminal UI with WASD/arrow navigation, ANSI colors, and box-drawing
User authentication with custom SHA256 hashing
CSV data import/export
Command-line flags and settings menu

## Installation

Clone the repository:

`git clone https://github.com/gbowne1/gearforge.git`

`cd gearforge`

Install dependencies (CMake 3.10+, Google Log, Google Test):

`sudo apt-get install cmake libglog-dev googletest`

Build the project:

`mkdir build && cd build`
`cmake ..`
`make`

Run:

./gearforge

## Usage

Run `./gearforge --help` for command-line options. On start, the program displays a splash screen, then prompts for login/register. Use WASD or arrow keys to navigate menus. Enter gear parameters (e.g., number of teeth, diametrical pitch) to calculate dimensions, select cutters, and get machining instructions.

See docs/user_guide.md for detailed usage.

## Contributing

Contributions are welcome! Please:

Fork or clone the repo
Create a feature branch (git checkout -b feature/yourfeature)
Commit changes (git commit -m "Add your feature")
Push to the branch (git push origin feature/yourfeature)
Open a pull request
See docs/developer_guide.md for developer info.

## License

GearForge is licensed under the MIT License.
