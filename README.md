# OmniFolder

A self-hosted, distributed backup suite built from scratch in C++ – designed as a fully-owned alternative to cloud backup services, with no third-party dependency for storage, transport, or recovery.

**STATUS:** Work in progress. Core client-server architecture, encryption, GUI, and disaster-recovery design are implemented. Backend data operations are still a work in progress.

## Overview

OmniFolder was built solo over 7 weeks (~7,000+ lines of C++) as a way for me to own my entire data storage and backup pipeline — from the network transport layer up through credential handling and disaster recovery — without relying on a cloud provider or a NAS. I wanted a system for managing my personal storage that was private, cross-platform, involved working with files directly, and resilient to a single point of failure. OmniFolder's data model consists of a single data folder (an "OmniFolder") that is seamlessly mirrored across multiple user-provided backup drives. The data exists solely on these backup drives, while a lightweight self-hosted coordination server glues the system together. In this way, OmniFolder combines the benefits of peer-to-peer data mirroring with the benefits of working with a centralized pool of data. Where OmniFolder differs from a product like syncthing is in its syncing behavior. Syncthing involves continuous, automatic syncing between peer nodes; however, OmniFolder involves a strict, manual file checkout system to completely eliminate sync conflict issues.

OmniFolder is like an Ender Chest in Minecraft (you access a central pool of data), except the data is stored in a distributed fashion by being mirrored in every chest (no "master copy" all the others reference), making the system tolerant to a single point of failure.

## Architecture

- **Client-server model** built from the socket level, using TLS for transport security
- **Protocol Buffers** define the wire protocol between client and server
- **Length-prefixed message framing** with per-connection buffering to correctly reassemble Protocol Buffer messages delivered across partial TCP reads
- **Self-signed TLS certificate generation** and automated port-forwarding diagnostics for NAT traversal, so a self-hosted server can be reached without manual network configuration

## GUI / Desktop Application
Both the client and server ship as full Qt desktop applications, not just command-line tools:
- **Multi-page setup and recovery wizards** (built with Qt's `QWizard` framework) walk a user through initial server configuration — naming, credentials, GitHub-based recovery beacon setup, and port configuration — plus a parallel recovery-mode flow for restoring a server from backup.
- **Live-updating client dashboard**, implemented with a custom `QAbstractTableModel`, that reflects connected clients in real time as they connect and disconnect — proper use of Qt's Model/View architecture rather than manually-managed widgets.
- **Client-side login and folder-browser UI**, including a custom login screen with password-visibility toggling and a tree-view-based folder browser.

## Security

- **Argon2id** for credential hashing
- **ChaCha20-Poly1305** for encrypted local config storage

## Disaster Recovery

If the primary server is ever lost entirely, OmniFolder includes a recovery beacon built on GitHub Gists — a lightweight, independent channel for reconstituting server state without relying on the original infrastructure surviving.

## Setup / Running Locally

*TODO — setup instructions are still being written. In the meantime, feel free to reach out (christian@christiancaamano.com) if you'd like to see it running.*

## Project Structure

- `/client` — client-side application
- `/server` — server-side application

---

Copyright © 2026 Christian Caamano. All rights reserved. The contents of this repository have been made public for viewing purposes only and may not be used, copied, modified, or distributed without explicit permission from the author. Any unauthorized usage will be prosecuted to the fullest extent of copyright law.

For inquiries, please email christian@christiancaamano.com
