<img src="https://i.ibb.co/yDs5vmy/thumb2.jpg">
<h1 align="center" id="title"><a href="https://youtu.be/BO-yjuZFHkE">TUTORIAL</a></h1>
  <p id="description"> </p>
  <p id="description">Editing mod for Call of Duty: Black Ops II Redacted (t6mpv43 and t6zmv41)</p>

<h2>🧐 Features</h2>

Here're some of the project's best features:

*   Demo browsing
*   Advanced dolly camera
*   Bone camera
*   Custom lights
*   Streams
*   Weapon camo & animations replacement
*   Holdgun
*   Custom sliders
<p id="description"> </p>
<h2>🛠️ Installation</h2>

<p>Download latest release and drop the files to your Redacted plugin folder</p>

```
...\Redacted\Plugins
```
<p id="description"> </p>
  
<h2>🤔 Things to keep in mind</h2>

* Always remove or rename <b>patch_redacted.ff</b> in <b>...Redacted\zone\redacted</b> if you want to load demos ripped from steam BO2.
* Demos from different Redacted version couldn't be loaded on other Redacted versions (e.g. demo from <b>43.1337.4</b> can't be loaded on <b>43.1734.4</b> and vice versa).
* If you need to set larger custom light radius than it's allowed in the menu use <b>mvm_lightRadiusLimit</b> console command to inrease radius limit.  Same for <b>mvm_lightColorLimit</b>.
* Always check for errors in console
* Please consider to skip back at least once when demo is loaded to avoid tick mismatches and let <b>timeline</b> and other components work properly.
* In case your mouse scroll wheel is broken use <b>mvm_cameraRoll</b> and <b>mvm_cameraFov</b> commands during setting dolly camera markers.

<p id="description"> </p>

# 📜 Complete T6MVM DVARS reference table

## 🎥 Camera Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `mvm_shake`                   | bool   | -        | -         | `false`   | Enable dolly camera shake. |
| `mvm_shakeSpeed`              | float  | `0.0`    | `10.0`    | `1.0`     | Dolly camera shake speed. |
| `mvm_shakeAmount`             | float  | `0.0`    | `10.0`    | `1.0`     | Overall shake intensity. |
| `mvm_shakePosition`           | float  | `0.0`    | `10.0`    | `1.0`     | Position shake intensity. |
| `mvm_shakeRotation`           | float  | `0.0`    | `10.0`    | `1.0`     | Rotation shake intensity. |
| `mvm_shakeX`                  | float  | `0.0`    | `10.0`    | `1.0`     | X-axis shake intensity. |
| `mvm_shakeY`                  | float  | `0.0`    | `10.0`    | `1.0`     | Y-axis shake intensity. |
| `mvm_shakeZ`                  | float  | `0.0`    | `10.0`    | `1.0`     | Z-axis shake intensity. |
| `mvm_shakePitch`              | float  | `0.0`    | `10.0`    | `1.0`     | Pitch shake intensity. |
| `mvm_shakeYaw`                | float  | `0.0`    | `10.0`    | `1.0`     | Yaw shake intensity. |
| `mvm_shakeRoll`               | float  | `0.0`    | `10.0`    | `1.0`     | Roll shake intensity. |
| `mvm_shakePreview`            | bool   | -        | -         | `false`   | Preview dolly camera shake. |
| `mvm_rollStep`                | float  | `1.0`    | `90.0`    | `5.0`     | Camera roll step (mouse wheel). |
| `mvm_showMarkerInfo`          | bool   | -        | -         | `true`    | Show dolly camera marker info (FOV/roll). |
| `mvm_frozenCam`               | bool   | -        | -         | `false`   | Enable frozen dolly camera. |
| `mvm_demoTimescaleStep`       | float  | `0.001`  | `10.0`    | `0.1`     | Demo timescale adjustment step. |
| `mvm_dollyCamOffsetPos`       | vec3   | `-5000.0`| `5000.0`  | `(0,0,0)` | Dolly camera position offset (X/Y/Z). |
| `mvm_dollyCamOffsetRot`       | vec3   | `-360.0` | `360.0`   | `(0,0,0)` | Dolly camera rotation offset (pitch/yaw/roll). |
| `mvm_freeRoamSpeed`           | float  | `0.0`    | `10.0`    | `1.0`     | Free camera movement speed. |
| `mvm_accelerationFactor`      | float  | `0.0`    | `10.0`    | `2.0`     | Free camera acceleration multiplier. |
| `mvm_slowingFactor`           | float  | `0.0`    | `10.0`    | `0.25`    | Free camera deceleration multiplier. |
| `mvm_cameraFov`               | float  | `1.0`    | `180.0`   | `90.0`    | Free camera field of view. |
| `mvm_cameraRoll`              | float  | `-180.0` | `180.0`   | `0.0`     | Free camera roll angle. |

## 🟢 Misc Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `mvm_greenScreen`             | bool   | -        | -         | `false`   | Enable green screen. |
| `mvm_greenScreenPlayers`      | int    | `0`      | `3`       | `1`       | Green screen targets (`0=None`, `1=Selected`, `2=Threshold`, `3=All`). |
| `mvm_greenScreenPlayersThreshold` | float | `0.0` | `5000.0` | `500.0` | Distance threshold for green-screened players. |
| `mvm_greenScreenColor`        | vec3   | `0.0`    | `1.0`     | `(0,1,0)` | Green screen RGB color. |
| `mvm_zdepth`                  | bool   | -        | -         | `false`   | Enable Z-depth rendering. |
| `mvm_zdepthDistance`          | float  | `0.0`    | `5000.0`  | `1000.0`  | Z-depth render distance. |

## 🌫️ Fog Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `z_fogstartdist`              | float  | `-200.0` | `5000.0`  | `0.0`     | Fog start distance. |
| `z_fogfadedist`               | float  | `-200.0` | `37000.0` | `36000.0` | Fog fade distance. |
| `z_fogstartheight`            | float  | `-200.0` | `37000.0` | `2000.0`  | Fog start height. |
| `z_fogfadeheight`             | float  | `-200.0` | `50000.0` | `18070.0` | Fog fade height. |
| `z_fogopacity`                | float  | `-10.0`  | `10.0`    | `0.8`     | Fog opacity. |
| `z_fogsunpitch`               | float  | `0.0`    | `360.0`   | `0.0`     | Fog sun pitch angle. |
| `z_fogsunyaw`                 | float  | `0.0`    | `360.0`   | `0.0`     | Fog sun yaw angle. |
| `z_fogsuninner`               | float  | `0.0`    | `30.0`    | `0.0`     | Fog sun inner radius. |
| `z_fogsunouter`               | float  | `0.0`    | `60.0`    | `30.0`    | Fog sun outer radius. |
| `z_fogcolor`                  | vec3   | `0.0`    | `255.0`   | `(255,255,255)` | Fog RGB color. |
| `z_fogsunopacity`             | float  | `-10.0`  | `10.0`    | `0.8`     | Fog sun opacity. |
| `z_fogsuncolor`               | vec3   | `0.0`    | `255.0`   | `(255,255,255)` | Fog sun RGB color. |

## 💡 Lights Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `mvm_lightRadiusLimit`        | float  | `0.0`    | `10000.0` | `1000.0`  | Maximum light radius. |
| `mvm_lightColorLimit`         | float  | `0.0`    | `255.0`   | `1.0`     | Light color intensity limit. |

## 🎬 Streams Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `mvm_streams`                 | bool   | -        | -         | `false`   | Enable streams recording. |
| `mvm_streams_noFlash`         | bool   | -        | -         | `true`    | Disable screen flashes during recording. |
| `mvm_streams_fps`             | int    | `50`     | `1000`    | `50`      | Recording FPS. |
| `mvm_streams_JPG`             | bool   | -        | -         | `false`   | Record streams as JPG. |
| `mvm_streams_avi`             | bool   | -        | -         | `false`   | Record streams as AVI. |
| `mvm_streams_tickStart`       | int    | `-1`     | `99999999`| `-1`      | Start tick for recording. |
| `mvm_streams_tickEnd`         | int    | `-1`     | `99999999`| `-1`      | End tick for recording. |
| `mvm_streams_recordCam`       | bool   | -        | -         | `false`   | Record camera data per frame. |

## 🔫 Weapon Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `mvm_camoChanging`            | bool   | -        | -         | `false`   | Enable weapon camo changes. |
| `mvm_primaryCamo`             | int    | `-1`     | `44`      | `-1`      | Primary weapon camo index. |
| `mvm_secondaryCamo`           | int    | `-1`     | `44`      | `-1`      | Secondary weapon camo index. |
| `mvm_cameraSway`              | bool   | -        | -         | `false`   | Enable camera sway. |
| `mvm_cameraSwayMaxAngle`      | float  | `0.0`    | `50.0`    | `10.0`    | Max camera sway angle. |
| `mvm_holdgun`                 | bool   | -        | -         | `false`   | Enable corpses holding weapons. |
| `mvm_holdgunWrist`            | bool   | -        | -         | `false`   | Attach weapons to wrist (not `tag_weapon_right`). |

## 📜 Menu/Demo Dvars

| Dvar Name                     | Type   | Min      | Max       | Default   | Description |
|-------------------------------|--------|----------|-----------|-----------|-------------|
| `mvm_menuBlur`                | bool   | -        | -         | `true`    | Enable menu blur effect. |
| `mvm_showBone`                | bool   | -        | -         | `true`    | Show selected bone marker. |
| `mvm_tick`                    | int    | `11000`  | `99999999`| `0`       | Jump to a specific demo tick. |

---

<h2>💻 Built with</h2>

*   <a href="https://github.com/Amyst82/T6SDK">T6SDK</a>
*   <a href="https://github.com/gmzorz/libgmavi">libgmavi</a>
*   <a href="https://github.com/nlohmann/json">nlohmann json</a>

<p id="description"> </p>

<h2>👀 Resources links and usefull stuff</h2>

* <a href="https://github.com/Amyst82/T6Shake">t6shake (A lightweight alternative. Consider it as Advanced Dolly Cam v2)</a>
* <a href="https://gmzorz.com/weapanim.html">Weapon animation replacement</a>
* <a href="https://www.youtube.com/watch?v=QEtlHBVYEAo">201 legendary demos</a>
* <a href="https://www.youtube.com/watch?v=xdaQp8kdNGg">BO2 Console V4</a>
* <a href="https://youtu.be/vcZcwg03SzY">OG Advanced Dolly Camera</a>
* <a href="https://youtu.be/6pkkpgb8VYY">Advanced t6gr camera data importer for After Effects</a>
