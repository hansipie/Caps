# CAPS — Client/Server for the Hotline Protocol

CAPS is an open-source implementation of the [Hotline](https://en.wikipedia.org/wiki/Hotline_Communications) protocol, written in C++. It consists of two components:

- **BlueCap** — the server
- **RedCap** — the client (work in progress)

## Features (BlueCap server)

- User authentication with account files
- User list management and broadcast
- Public chat, instant messages, and broadcast messages
- File browsing, download, upload, delete, move, and info
- Image and video previews (via ImageMagick)
- User administration (create, modify, delete accounts)
- Configurable via `conf/ServConf.conf`
- Cross-platform: Linux / Win32

## Requirements

- `g++` >= 3.2.1
- [ImageMagick](https://imagemagick.org) with `Magick++` (for previews)
- `pkg-config`

## Build

```bash
cd BlueCap
make
```

The binary is produced in `build/bin/BlueCap`.

### Clean

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
BlueCap/
├── src/        # C++ source files
├── docs/       # Documentation and changelog
├── conf/       # Configuration files
├── accounts/   # User account files
├── files/      # Shared files directory
└── Makefile
RedCap/         # Client (coming soon)
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

## License

See [LICENSE](LICENSE).
