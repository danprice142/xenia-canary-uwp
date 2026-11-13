# Xbox Series X Call of Duty Performance and Stability Plan

## Scope

Target platform: Xbox Series X, Xenia Canary UWP.

Primary test matrix:
- Call of Duty: Black Ops (Title ID 41560855): Zombies mode.
- Call of Duty: World at War (Title ID 4156081C): Campaign and Zombies modes.
- Primary failure class: severe stutter and intermittent emulator crashes during gameplay.

This document records the agreed investigation and validation plan. It does not claim that a performance fix has already been implemented.

## Investigation principles

1. Reproduce each failure on the exact same console build, game media, title update/DLC state, and emulator configuration.
2. Capture a clean baseline before changing emulator behavior.
3. Classify each event as host crash, guest exception, GPU device removal/reset, deadlock/hang, or performance regression.
4. Prefer root-cause fixes in shared emulator subsystems. Do not add game-specific hacks unless a reproducible title-specific incompatibility is demonstrated.
5. Keep each change isolated and measurable; do not combine speculative tweaks.
6. Preserve correctness and compatibility. Never trade broken rendering, audio, or game logic for a higher reported FPS.

## Test matrix

| Game | Mode | Scenarios |
|---|---|---|
| Black Ops (41560855) | Zombies | Boot to mode, map load, solo match, sustained gameplay, map restart/exit |
| World at War (4156081C) | Campaign | Boot, mission load, checkpoints, sustained gameplay, mission transition |
| World at War (4156081C) | Zombies | Boot to mode, map load, solo match, sustained gameplay, restart/exit |

For every scenario, record whether stutter or a crash occurs, elapsed time to failure, and the last repeatable action before failure.

## Data to capture

For each run, retain:
- Exact emulator commit/build identifier and package version.
- Xbox OS version and available storage state.
- Game media/revision, title update, DLC, and region where known.
- Effective emulator settings and graphics backend.
- Xenia log from process start through failure.
- Crash dump or call stack, if the platform exposes one.
- Frame-time trace or timestamped frame pacing samples, where instrumentation is available.
- Memory usage/high-water mark and allocation failures, where available.
- GPU errors, device-removed reason, fence waits, queue stalls, and shader/pipeline cache events, where available.

Do not infer a GPU fault from stutter alone. Correlate logs and timing data.

## Diagnostic workstreams

### 1. Frame pacing and CPU scheduling
- Measure frame-time distribution (median, p95, p99) rather than relying only on average FPS.
- Identify long host-side waits, thread starvation, lock contention, and excessive synchronization.
- Check whether audio, presentation, or title execution threads block one another.
- Only adjust priorities or scheduling after proving the affected thread and contention source.

### 2. GPU command submission and presentation
- Inspect command processor submission, queue synchronization, fence waits, swapchain presentation, and device-loss handling.
- Identify repeated waits or unnecessary flushes that correlate with frame-time spikes.
- Check shader/pipeline compilation and cache misses during gameplay.
- Preserve GPU ordering and resource lifetime guarantees; do not remove synchronization speculatively.

### 3. Memory and resource lifetime
- Track host memory high-water marks and allocation failures during map loading and long sessions.
- Inspect guest-memory mapping, graphics-resource caches, descriptor/resource lifetime, and cleanup on title exit.
- Look for unbounded growth, duplicate allocations, stale references, and use-after-free evidence.
- Verify whether the UWP process is terminated by platform resource limits before attributing the failure to a code defect.

### 4. Crash and hang classification
- Distinguish guest exceptions from host access violations, GPU device removal, watchdog termination, and deadlocks.
- Preserve the first failure and call stack; later log noise may be secondary.
- Add narrowly scoped diagnostics only where existing logs cannot identify the failing subsystem.

### 5. UWP / Xbox lifecycle
- Check graphics-device and resource recreation after suspend/resume or app focus changes.
- Verify cleanup and reinitialization paths around title launch, title exit, and emulator shutdown.
- Keep Xbox-specific changes behind platform guards where required.

## Change and validation policy

For each proposed fix:
1. Link the observation to a reproducible log, trace, or call stack.
2. Identify the owning subsystem and explain the causal mechanism.
3. Make the smallest isolated code change.
4. Build the target UWP configuration and retain build output.
5. Re-run the exact reproducer and compare against the baseline.
6. Run regression checks on both target games and a small non-Call-of-Duty smoke-test set.
7. Reject or revert changes that merely move the failure, reduce compatibility, or worsen p95/p99 frame time.

## Exit criteria

A change is considered validated only when:
- The original reproduction is repeated successfully on Xbox Series X.
- No new crash, hang, rendering, or audio regression is observed in the tested scenarios.
- Frame-time and resource measurements show a repeatable improvement or the targeted crash is demonstrably fixed.
- The test build, commit, logs, and before/after measurements are recorded.

## Current status

- Scope and game/mode matrix: agreed.
- Existing public compatibility reports: reviewed as leads, not proof of current-build behavior.
- Xbox Series X baseline logs and traces: not yet supplied.
- Root cause: not yet established.
- Code-level performance fixes: not yet made or validated.


## Resolution and upscaling workstream

Keep two independent concepts separate:

1. **Post-process upscaling:** the existing D3D12 presenter contains AMD FidelityFX Super Resolution 1.0 (EASU/RCAS) and CAS paths. The UI already exposes these under Display > Post-processing settings. The branch now selects FSR as the default effect so the Xbox Series X test build exercises the existing FSR path; this is a default-selection change, not a new FSR implementation.
2. **Guest/game render-mode selection:** `internal_display_resolution` is documented in the graphics system as a requested mode for games that support multiple resolutions. It is not a universal render-scale multiplier and must not be presented as a guaranteed way to force every Xbox 360 title to render at 1440p or 4K. Do not expand its limits or change game mode enumeration until its complete call path and title behavior are verified.

### Upscaling validation

- Test FSR against the current bilinear baseline on Black Ops Zombies and World at War Campaign/Zombies.
- Capture screenshots at identical scenes and compare edge stability, detail, and aliasing.
- Compare frame-time p50/p95/p99 and GPU queue/fence behavior; revert the FSR default if it creates a repeatable performance or stability regression.
- Verify the effective output/surface resolution on the Xbox build before claiming 1440p or 4K output.
- Keep guest render-mode changes separate from presenter upscaling so each effect can be measured independently.

Status: FSR default changed to `fsr` for testing. Build and Xbox runtime validation are still pending.
