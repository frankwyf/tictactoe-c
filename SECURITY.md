# Security Policy

## Supported Versions

| Version | Supported |
| ------- | --------- |
| 1.x     | ✅         |

## Reporting a Vulnerability

Please **do not** open a public GitHub Issue for security vulnerabilities.

Instead, send an email to **frankwyf66@hotmail.com** with:

- A clear description of the vulnerability
- Steps to reproduce (minimal reproducer preferred)
- Potential impact assessment

You will receive an acknowledgement within **48 hours**.  
If the report is confirmed, a fix will be prepared and released as soon as possible,
and you will be credited in the release notes unless you prefer anonymity.

## Scope

This is a small command-line game library. The most likely vulnerability classes are:

- Buffer overflows through malformed grid-size or win-length inputs
- Integer overflows in board indexing

All public API calls validate their parameters and return `TICTACTOE_ERR` on invalid input;
they never write out-of-bounds on a well-formed call.
