<p align="center">
  <img src="assets/logo.png" alt="CAPS logo" width="200"/>
</p>

# CAPS — Client/Server for the Hotline Protocol

![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-blue)
![License](https://img.shields.io/badge/license-MIT-green)

CAPS is an open-source implementation of the [Hotline](https://en.wikipedia.org/wiki/Hotline_Communications) protocol, written in C++. It consists of three components:

- **BlueCap** — the server
- **RedCap/Console** — the command-line client
- **RedCap/GUI** — the graphical client (wxWidgets, work in progress)

## Features

### BlueCap (server)

- User authentication with account files
- User list management and broadcast
- Public chat, instant messages, and broadcast messages
- File browsing, download, upload, delete, move, and info
- Image and video previews (via ImageMagick)
- User administration (create, modify, delete accounts)
- Configurable via `conf/ServConf.conf`
- Cross-platform: Linux / Win32

### RedCap/Console (CLI client)

- Connect to Hotline servers
- Public and private chat
- File browsing, download and upload
- User administration
- Interactive prompt

### RedCap/GUI (graphical client)

- wxWidgets-based interface
- Multi-server support with bookmarks
- Tabbed interface: chat, file transfer, downloads, news
- Public and private chat
- Server and client file explorers with transfer queue
- AES encryption
- User administration panel

## Requirements

### BlueCap

- `g++` >= 3.2.1
- [ImageMagick](https://imagemagick.org) with `Magick++` (for previews)
- `pkg-config`

### RedCap/Console

- `gcc` >= 4
- `pthread`

### RedCap/GUI

- `g++` >= 4
- [wxWidgets](https://www.wxwidgets.org) >= 3.0 (`wx-config`)

## Build

### BlueCap

```bash
cd BlueCap
make
```

Binary: `BlueCap/build/bin/BlueCap`

### RedCap/Console

```bash
cd RedCap/Console
make
```

Binary: `RedCap/Console/build/bin/RedCap`

### RedCap/GUI

```bash
cd RedCap/GUI
make
```

Binary: `RedCap/GUI/build/bin/caps`
Launch from `RedCap/GUI/` so that icon assets resolve correctly:

```bash
./build/bin/caps
```

### Clean / Rebuild (all projects)

```bash
make clean    # remove object files
make fclean   # full clean including binary
make re       # full rebuild
```

## Configuration

Edit `BlueCap/conf/ServConf.conf` to set server options (port, file directory, etc.).

User accounts are stored in `BlueCap/accounts/`.

## Project Structure

```
Caps/
├── BlueCap/
│   ├── src/            # C++ source files (.cpp / .hpp)
│   ├── build/
│   │   ├── bin/        # Compiled server binary
│   │   └── obj/        # Object files
│   ├── conf/           # Server configuration
│   ├── accounts/       # User account files
│   ├── files/          # Shared files directory
│   └── Makefile
├── RedCap/
│   ├── Console/        # CLI client (C, autotools)
│   │   ├── src/
│   │   │   ├── shared/     # Socket and utility helpers
│   │   │   ├── engine/     # Command engine and functions
│   │   │   └── bridge/     # Network bridge and receive functions
│   │   └── build/bin/  # Compiled console binary
│   └── GUI/            # Graphical client (C++/wxWidgets)
│       ├── src/
│       │   ├── core/       # Application entry, main frame and panel
│       │   ├── ui/         # Dialogs and widgets
│       │   ├── chat/       # Chat panels, broadcast, newsgroup
│       │   ├── net/        # Connection, protocol engine, server data
│       │   ├── files/      # File explorers, transfers, download queue
│       │   └── users/      # User management, admin, authentication, crypto
│       ├── icons/          # Bitmap and icon assets
│       ├── res/            # Windows resource files
│       ├── docs/           # Notes and archived legacy sources
│       ├── legacy/         # MSVC project files and Windows-specific code
│       └── build/bin/  # Compiled GUI binary
└── README.md
```

## Version History

| Version | Highlights |
|---------|-----------|
| v0.1a   | Authentication, user list, user broadcast |
| v0.1b   | Public chat, instant message, file listing |
| v0.2    | Download/upload, Win32 port |
| v0.3    | Previews, user administration, account management |
| v0.4    | Bug fixes, RPM packaging, selective debug output |
| v0.5a   | Video previews, JPEG image previews |
| v0.5b   | Demonstration release |

## Big Up

Mad respect to those still keeping the Hotline protocol alive more than 20 years later:

- **[hotline](https://github.com/mierau/hotline)** — A modern, open-source remake of the classic 1997 Hotline app, written in Swift for macOS with iOS/iPadOS support in development. Chat, file sharing, message boards — rebuilt with a native contemporary interface.

- **[Mobius](https://github.com/jhalter/mobius)** — A cross-platform Hotline server written in Go, distributed as a single binary and fully compatible with existing clients. Runs on macOS, Linux, and Windows, with an optional HTTP API for administration.

- **[Hotline Navigator](https://github.com/fuzzywalrus/Hotline-Navigator)** — A modern Hotline client built with Tauri, React, and Rust, available on macOS, Windows, Linux, iOS, iPadOS, and Android. File sharing, chat, and bulletin boards on virtually every platform.

## License

See [LICENSE](LICENSE).
