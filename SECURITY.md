# Security policy

## Supported version

Security fixes are currently developed only against the latest source on the `main` branch. No public binary release is supported yet.

## Reporting

When the GitHub repository is public, prefer GitHub private vulnerability reporting if it is enabled. Do not place credentials, private sessions, personal data or an unredacted exploit in a public issue.

Ordinary crashes, validation failures and audio defects may be reported through GitHub Issues using the information requested in `CONTRIBUTING.md`.

## Secrets

The repository must never contain signing certificates, private keys, access tokens, passwords or provisioning profiles. The project ignore rules cover common credential formats, but contributors remain responsible for inspecting their changes before submission.
