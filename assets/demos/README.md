# Demo Assets

This directory now holds Play-screen preview storyboards for the current macOS sampler build.
The registry still keeps the intended `.mp4` targets, but the active playback path uses
`.preview.txt` storyboard manifests until real media files and a video backend are added.

Current storyboard manifests:

- `apollo11_lander_loop.preview.txt`
- `howitzer_loop.preview.txt`
- `chess_loop.preview.txt`
- `orbital_loop.preview.txt`

Current keyframe images:

- `apollo11_lander_keyframe.ppm`
- `howitzer_keyframe.ppm`
- `chess_keyframe.ppm`
- `orbital_keyframe.ppm`

Planned video files:

- `apollo11_lander_loop.mp4`
- `howitzer_loop.mp4`
- `chess_loop.mp4`
- `orbital_loop.mp4`

Each storyboard manifest defines looping preview frames with title text, accent colors,
headline/subline copy, and optional `image=` entries pointing at ASCII `P3` `.ppm` files.
The Play screen renders those frames on a timed loop today.
