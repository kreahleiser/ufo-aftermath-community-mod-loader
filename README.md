# UFO Aftermath display helper

Fan-made **OpenGL wrapper** for *UFO: Aftermath* (2003). The stock game is locked to 1024×768 4:3. This loads instead of system `opengl32.dll` and:

- runs **borderless** at your monitor size
- letterboxes a **16:9** picture (or a large 4:3) into the middle
- maps the **mouse** to that letterbox
- **caps FPS at 60** so AMD (and other uncapped GPUs) do not freeze the UI / geoscape

Not affiliated with Altar, Fulqrum, or Valve.

**Prefer building it yourself.** A random `opengl32.dll` next to a game is exactly what malware looks like. The whole program is this repo. Releases also attach a DLL built from the same source.

## Install

1. Get `opengl32.dll` — run `build.bat` locally, or download it from [Releases](../../releases).
2. Copy it next to `UFO.exe`:

   `Steam\steamapps\common\UFO Aftermath\`

3. Launch once. The DLL writes `ufo_display.ini` beside itself with the defaults below. Delete `opengl32.dll` to uninstall.

## Settings (`ufo_display.ini`)

| Key | Default | Meaning |
|-----|---------|---------|
| `Mode` | `169` | `169` = 16:9 3D + HUD stretched to 16:9, bars on ultrawide. `43` = largest 4:3 that fits (HUD unstretched). |
| `Borderless` | `1` | Cover the current monitor. |
| `FrameLimit` | `60` | Aftermath UI breaks if the GPU runs uncapped. `0` = unlimited (not recommended on AMD). |
| `CrispUi` | `1` | Nearest-neighbour UI textures so bitmap fonts do not show cell borders. |
| `Logging` | `0` | `1` writes `ufo_display.log` next to the DLL. |

## Build (32-bit MSVC)

Visual Studio with the **x86** C++ toolset. From a VS developer prompt, or just run `build.bat`:

```bat
build.bat
```

That produces `opengl32.dll` in this folder. Copy it into the game directory.

`gl_forwards.cpp` / `opengl32.def` trampoline every export of 32-bit `opengl32.dll` (system `glu32` imports functions Aftermath itself never calls). Regenerate with `powershell -File generate_forwards.ps1` if you need to.

## Known limits

- Not native 21:9 world rendering. Ultrawide is 16:9 (or 4:3) with side bars. Stretching the HUD across 21:9 looks worse than boxing 16:9.
- FMVs stay 4:3 inside the letterbox.
- HUD is still a 1024×768 layout, scaled. Squad portraits are corrected; a few screens can look slightly wide.
- Antivirus may flag a game-folder `opengl32.dll`. That is a common false positive for wrappers. Build from this source.
- If the game will not start, delete `opengl32.dll`.

## Why a DLL

Aftermath is a 32-bit OpenGL 1.x binary with no source. The loader looks for `opengl32.dll` next to `UFO.exe` first. That is the least invasive hook: no patched `UFO.exe`, Steam file verify stays clean aside from the extra files.

## License

MIT. *UFO: Aftermath* remains the property of its copyright holders.
