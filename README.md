# Crypt Keeper

Crypt Keeper is an alternative to `gopass` or `pass` that obfuscates secret
metadata behind an encrypted index.

## Installation

### Unix
To install to `~/.local/bin`:
```
just install
```

To install `/anywhere/you/like`
```
just install /anywhere/you/like
```

I decided to use `just` instead of `make` because it's simply superb. If you're
not quite ready to leave `make` behind, manually run the `release` target and
install the binary wherever you prefer.

### Windows

You're on your own for now. Use the `release` target and install the binary
manually. Contributions are welcome.

## Target Users

This tool might be for you if:
- You would prefer to pipe secrets straight to the clipboard rather than
  navigate a web application.
- You need a script-friendly password manager
- The BitWarden cli is too slow for you
- You're drawn to the speed and simplicity of `pass`, but don't want to risk
  leaking metadata

## Purpose

### Own your data with ease

Store your secrets wherever works best for you — locally, a remote git
repository, a USB drive, or cloud storage. No server to maintain, no backup
strategy to design, no availability concerns. Just files under your control.

### Secure metadata

Unlike `pass` and `gopass`, metadata (i.e. the directory/tree structure that
points to secrets) is encrypted in an index file. For most users this won't
matter much, but if your secret *names* reveal sensitive information, this is a
meaningful distinction.

If an attacker gains access to a system with a `pass/gopass` password store, the
directory structure plainly reveals where all the owner's accounts are held. A
path like `investing/fidelity/...` tells an attacker exactly where to look.
Paired with access to the owner's email client, an attacker could quickly cause
serious damage with this sort of metadata access.

## Goal

Crypt Keeper is not a drop in replacement for `pass` or `gopass`. The goal is to
take the best features of both and build on them with a tighter UX. Reaching
feature parity with `gopass` is a long-term aim.

## Design

### Mounts

I'm a big fan of the `gopass` mount design, so I made this a core feature
of Crypt Keeper. The first vault you create automatically becomes the root
mount. This can easily be changed later with the `chroot` command. Additional
mounts can be added with `mount` and are placed under the root mount at a
specified alias.

### Index

The encrypted index is a TOML file containing an array of secret entries.


## Quick Start Guide

### Initialize a new vault
For now, you must provide the full gpg key fingerprint to initialize a vault.
```bash
gpg --list-keys
ck init -v myvault -k {full_gpg_key_fingerprint}
```

### List current configuration
```
ck config
```

**Note**: Most of these are just placeholders at this point in development.
Focus your attention mainly on `core.home`, this is where new vaults are added
by default when an alternate path is not specified.

### Insert a secret
```bash
ck insert some/secret
```

### Show secrets
Show secret tree
```bash
ck show
```

Show secret
```bash
ck show some/secret
```

```text
Crypt Keeper ⟶  (/home/cjordan/.local/share/crypt-keeper/myvault)
└── some
    └── secret
```

### Mount another vault

```bash
ck init -v anothervault -k {full_gpg_key_fingerprint}
```

At this point, you have some options. If the vault was initialized in
`core.home` and you don't want to provide an alternate alias, the vault will be
mounted with its name as the alias by default.

```bash
ck mount anothervault
```
```text
Crypt Keeper ⟶  (/home/user/.local/share/crypt-keeper/myvault)
└── anothervault ⟶  (/home/user/.local/share/crypt-keeper/anothervault)
```

If instead the vault is in an alternate location or you wish to use an alternate
alias, use this:

```bash
ck mount anothervault some/other/location/in/mount/tree
```

```text
Crypt Keeper ⟶  (/home/user/.local/share/crypt-keeper/myvault)
└── some
    ├── other
    │   └── location
    │       └── in
    │           └── mount
    │               └── tree ⟶  (/home/user/.local/share/crypt-keeper/anothervault)
    └── secret
```

**Note**: Mounts are designed to shadow items in the root vault as long as a
secret isn't located along the specified alias path. In other words, a terminal
node can never have children.

### Unmount a vault

```bash
ck umount some/other/location/in/mount/tree
```

```text
ck  [.]  some/other/location/in/mount/tree has been unmounted
Crypt Keeper ⟶  (/home/user/.local/share/crypt-keeper/myvault)
```

### Change root vault

```bash
ck chroot anothervault
```

### Desired Near-Term Features

- Integrate git
  - as of right now, initializing and managing a repo is manual
- Add keys
  - as of now, there is no mechanism in place to add gpg keys to a vault
- Migrate `pass` and `gopass` stores to a crypt keeper vault
- Migrate BitWarden items to a Crypt Keeper vault
- Browser integration
- Any of the other great `gopass` features that are missed

## Contributing

## Credit and License