# Wave - vibe coded Terminal Animation

## Installation

### macOS / Linux

```bash
# Clone the repo
git clone https://github.com/yourusername/wave.git
cd wave

# Compile and install
make install

# Add to your shell config (~/.bashrc, ~/.zshrc, etc)
echo "wave" >> ~/.bashrc

# Reload shell
source ~/.bashrc
```

### That's it!

Just add `wave` to your shell configuration file, and you're done. The animation will play once per day when you open a new terminal.

## How it works

- First terminal of the day: Animation plays ✨
- Subsequent terminals: Skipped (cached in `~/.wave-cache`)
- Cache resets daily at midnight

## Customization

Don't need customization? That's the point! It just works.

If you want to disable it temporarily:
```bash
# Don't add 'wave' to your shell config
```

To force the animation to play:
```bash
# Remove the cache
rm ~/.wave-cache
wave
```

## Requirements

- Linux or macOS
- G++ compiler (or any C++11 compatible compiler)
- Terminal that supports ANSI colors

## License

MIT
