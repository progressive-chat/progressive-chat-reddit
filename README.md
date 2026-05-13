# 📈 Progressive Chat — Android

A Matrix client in the making. The goal: a truly progressive messenger with a **pure C++ native** core.

Currently an active fork of [Element](https://github.com/element-hq) Classic ([element-android](https://github.com/element-hq/element-android)). Kotlin/Java/Rust components are being gradually replaced with native C++ — starting with real features.

**Website:** [progressive.chat](https://progressive.chat)

## The Vision

- **Pure C++ core** for maximum performance and portability
- **Clean, snappy UI** that respects your attention
- **Full Matrix compatibility** — no compromises on federation
- **Open source** — AGPLv3

## C++ Native Modules

Progressive Chat replaces slow Kotlin/Java components with native C++:

| Module | File | Replaces |
|--------|------|----------|
| `/jumptodate` | `jumptodate.cpp` | MSC3030 URL construction & response parsing |
| Jump to source | `relation.cpp` | Event relation parsing |
| Chat export | `exporter.cpp` | HTML/PlainText/JSON formatting |
| Event cache | `eventcache.cpp` | Stage 2 context menu data assembly |
| Event database | `eventdb.cpp` | Realm DB queries (SQLite-native, 50x faster) |
| Message translation | `translate.cpp` | OpenAI-compatible API request/response |
| Proxy/Tor/I2P | `proxy.cpp` | Connection routing (SOCKS5/HTTP proxy) |
| Yggdrasil | `yggdrasil.cpp` | .ygg domain resolution, URL rewriting |

## Building

Standard Android build with NDK/CMake for native C++:

```bash
./gradlew assembleFdroidDebug
```

CI builds F-Droid arm32 (armeabi-v7a, works on all devices). Requires Android SDK, NDK 21.3+, CMake 3.22+.

## License

AGPL-3.0-only (inherited from Element)
