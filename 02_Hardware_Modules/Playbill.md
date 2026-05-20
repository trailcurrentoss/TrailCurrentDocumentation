# TrailCurrent Playbill

In-rig entertainment head for stationary use. A 10-foot, remote-driven Live TV / radio / library / streaming / cast surface that turns the Radxa Dragon Q6A into the rig's entertainment center when work is done in the evening. **Not the vehicle dash head unit.**

## At a glance

| | |
|---|---|
| Hardware | Radxa Dragon Q6A (Qualcomm QCS6490, 8 GB) |
| OS | Ubuntu Noble 24.04 + GNOME on Wayland, branded TrailCurrent |
| App | Electron (no TypeScript) installed on a normal desktop — not a kiosk |
| Audio out | Built-in 3.5 mm analog jack (WCD938x) — pinned in WirePlumber so HDMI doesn't win |
| Video out | HDMI; native panel resolution auto-detected |
| Instances per rig | Up to 3 (e.g. Living Room / Bedroom / Bunkhouse) |
| CAN address blocks | `0x100–0x10F` · `0x110–0x11F` · `0x120–0x12F` |
| MQTT topic root | `local/playbill/<deviceId>/<feature>/{command,status}` |
| Time source | Headwaters (NTP, port 123) |

## Wire-only operation is a first-class path

Third-party CAN MCUs — a steering-wheel button MCU, an IR receiver, a hard-buttons remote panel — can control any Playbill instance **directly on the CAN bus with no MQTT, no Headwaters service, and no cloud**. This is the strongest end-to-end demonstration of the platform's wire-only capability.

- Wire-level contract: [Headwaters `DOCS/CAN-REMOTE.md`](../../TrailCurrentHeadwaters/DOCS/CAN-REMOTE.md)
- Full message layouts: [CAN_BUS_REFERENCE.md — Playbill block](../10_Reference/CAN_BUS_REFERENCE.md#playbill-multi-instance-block)
- Narrative rationale: `/Product/TrailCurrentPlaybill/docs/app/dbc-additions.md`

## Source of truth

All operational, build, flash, and per-feature documentation lives in the project repo at **`/Product/TrailCurrentPlaybill/`**:

- `README.md` — desktop-vs-appliance framing, hardware, repo layout
- `STAGE1_PLAN.md` — the Stage-1 implementation plan in full
- `docs/SETUP.md` — operator guide: build → flash → first boot
- `docs/KERNEL_UPDATE_POLICY.md` — why kernel + Mesa + linux-firmware are pinned
- `docs/RADXA_LESSONS_LEARNED.md` — Q6A-specific gotchas (boot, audio, video, AirPlay)
- `docs/netflix-setup.md`, `docs/youtube-setup.md` — per-source setup
- `docs/app/` — per-feature docs (Live TV, Radio, Cast, architecture walkthrough, DBC narrative)

Anything in this file that disagrees with the repo is wrong — defer to the repo. This file exists only to keep the central documentation index complete.
