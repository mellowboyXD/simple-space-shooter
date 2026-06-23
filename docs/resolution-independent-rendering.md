# Independent Screen Resolution Rendering

The internal render target patter, also known as **Resolution-Independent Rendering**, separates the game's internal rendering resolution from the physical display resolution. The 2D or 3D game world gets rendered into a fixed-size text (render target or frame buffer), then stretched/scaled to fit the fullscreen or windowed monitor backbuffer.

## How it works

This pattern allows developers to decouple performance (rendering at a lower internal resolution) from display quality (outputting to a crisp 4K monitor). Additionally, the game's aspect ratio is maintained.

1. **Define a Virtual Resolution**

Choose a target internal resolution (e.g., 1280 × 720 for 3D games or 320 × 180 for pixel art). This is the resolution your engine will calculate game logic, physics, and draw calls against, ensuring consistent behavior regardless of the display.

2. **Offscreen Rendering**

Direct your rendering pipeline to draw all cameras, post-processing effects, and world geometry into an off-screen Render Target (or framebuffer) matching your virtual resolution, instead of writing directly to the screen.

3. **Scaling to the Backbuffer**

Take the completed offscreen render target and draw it as a full-screen quad or sprite directly to the actual hardware monitor's backbuffer.If aspect ratios match: The texture stretches to fit the full screen.If aspect ratios differ: You can calculate a destination rectangle with pillarbox (black bars on the sides) or letterbox (black bars on top/bottom) to preserve your target aspect ratio.

4. **Overlays and UI (The Dual Pass)**

Render user interface (UI) elements, like text and HUDs, separately from the main game world. Draw the UI elements directly onto the native monitor's backbuffer (at the full fullscreen resolution) after the scaled game world texture has been drawn. This prevents UI text and health bars from becoming blurry or pixelated when stretched across large screens.

---

> **References**:
> raylib - https://www.raylib.com/cheatsheet/cheatsheet.html

