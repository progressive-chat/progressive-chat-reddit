# ⛓️ Progressive Chat — Android

A Matrix client in the making. The goal: a truly progressive messenger with a **pure C++ native** core.

Currently an active fork of Element Classic Android. Kotlin/Java/Rust components are being gradually replaced with native C++ — starting with real features.

**Website:** [progressive.chat](https://progressive.chat)

## The Vision

- **Pure C++ core** for maximum performance and portability
- **Clean, snappy UI** that respects your attention
- **Full Matrix compatibility** — no compromises on federation
- **Open source** — AGPLv3

## First Feature: `/jumptodate`

Jump to any date in the chat timeline. Powered by a native C++ module (`progressive_native`) — date validation, MSC3030 URL construction, and response parsing happen in C++, with JNI bridge to the Kotlin UI layer.

```
/jumptodate 2024-06-15
```

## Status

🚧 Active development. Shipping features while rewriting the foundation.

## Building

Standard Android build with NDK/CMake for the native C++ module:

```bash
./gradlew assembleGplayDebug
```

Requires Android SDK, NDK 21.3+, CMake 3.22+.

## License

AGPL-3.0-only (inherited from Element)
