# Simulator Manifest

This file tracks the merged sampler's initial registry configuration.

- Apollo 11 Lander: `assets/demos/apollo11_lander_loop.mp4`
- Howitzer Artillery: `assets/demos/howitzer_loop.mp4`
- Chess: `assets/demos/chess_loop.mp4`
- Orbital Simulator: `assets/demos/orbital_loop.mp4`

Storyboard manifests currently used by the macOS sampler:

- Apollo 11 Lander: `assets/demos/apollo11_lander_loop.preview.txt`
- Howitzer Artillery: `assets/demos/howitzer_loop.preview.txt`
- Chess: `assets/demos/chess_loop.preview.txt`
- Orbital Simulator: `assets/demos/orbital_loop.preview.txt`

These are the registered prerecorded looping preview asset paths used by the Play screen.
The current sampler shell now tracks preview selection, loop timing, and asset readiness from this registry.
Actual video decoding/rendering is still pending import of real media files and a playback backend.
