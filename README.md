# UFO Aftermath Community Mod Loader

A fan-made **display fix** and **mod loader** for *UFO: Aftermath* (2003).

This makes the game:

- Run **borderless** at your monitor size
- Letterbox a **16:9** picture (or a large 4:3) into the middle
- Map the **mouse** to that letterbox
- Cap **FPS at 60** by default (change it in `ufo_mod_manager.exe`), so AMD and other uncapped GPUs do not freeze the UI
- Load **ALPine `.lua` plugins** from a `mods` folder on launch

Not affiliated with Altar, Fulqrum, ALPine, or Valve.

## Install

1. Download the release zip from [Releases](../../releases).
2. Copy `opengl32.dll`, `ufo_mod_manager.exe`, and the `plugins` folder next to `UFO.exe`:

   `Steam\steamapps\common\UFO Aftermath\`

3. Launch once. The DLL writes `ufo_display.ini` and creates a `mods` folder. Bundled plugins stay in `plugins` until you tick them on.

To uninstall, delete `opengl32.dll` and `ufo_mod_manager.exe`. You can also remove `mods`, `plugins`, `ufo_display.ini`, and `modalpine.vfs` if you want a clean folder.

## Mods

The release zip includes a `plugins` folder (off by default): **All Human Gear**, **TNT**, **Field of View**, and **Crisp Fonts**. Tick them in `ufo_mod_manager.exe` to enable. Extra files a plugin needs (for example the `tnt` folder next to `tnt.lua`) stay beside the script when you move it.

You can also drop other ALPine-style `.lua` plugins into `mods`. They are applied on the next launch. Stock `gamedata.vfs` is never edited.

Open `ufo_mod_manager.exe` to tick plugins on and off. Enabled mods live in `mods`. Unticked mods are parked in `plugins`, which the game does not read. Restart the game after changing ticks.

Gameplay ideas for the bundled plugins, and the TNT art, come from ALPine by Andrew 'Fulby' Campbell (2003). The scripts themselves were written for this loader.

Display options (borderless, 16:9 vs 4:3, frame rate, crisp UI, logging) are on the right of that same window. Changes apply the next time you start the game.

## Build (32-bit MSVC)

Only needed if you want to compile it yourself. Visual Studio with the **x86** C++ toolset, from this folder:

```bat
build.bat
```

That produces `opengl32.dll` and `ufo_mod_manager.exe`. Copy them into the game directory.

## Known Limits

- Not native 21:9 world rendering. Ultrawide is 16:9 (or 4:3) with side bars.
- FMVs stay 4:3 inside the letterbox.
- HUD is still a 1024×768 layout, scaled. Squad portraits are corrected; a few screens can look slightly wide.
- Localization-pack text from some ALPine plugins is not written back yet, so extra item names can fall back to short names.
- Antivirus may flag a game-folder `opengl32.dll`. That is a common false positive for wrappers. Build from this source if you would rather not run a prebuilt DLL.
- If the game will not start, delete `opengl32.dll`.

## Why a DLL

Windows loads `opengl32.dll` from the game folder first. That is how this hooks in: no patched `UFO.exe`, and Steam file verify stays clean aside from the extra files.

## License

MIT. *UFO: Aftermath* remains the property of its copyright holders.
