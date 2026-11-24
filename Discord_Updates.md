# Sharing progress with the team in Discord

## What's available

OpenCode has **no built-in Discord plugin**. I can't see your server or post to it from this session.

Right now this machine has:
- one global plugin: `opencode-tts`
- **no MCP servers configured**
- the `opencode` CLI is not on the shell PATH in this session

So there is no Discord connection at the moment.

## Options, easiest first

| Option | Effort | Result |
|---|---|---|
| **Post the docs / digest manually** | None | I write the summary, you paste + pin it in `#general` |
| **Discord webhook (recommended)** | 5 minutes | A script posts updates from your machine to a channel |
| Export the session | CLI needed | `opencode session export <id> --sanitize` → JSON; teammates `opencode session import session.json` |
| Community Discord MCP server / plugin | Setup + bot token | I get Discord tools, but it's third-party code and needs a bot account |
| Custom OpenCode plugin | Development work | Hooks that post on session events — overkill for 4 weeks |

Webhook is the winner: no bot application, no OAuth, no plugin, and the URL stays on your machine.

## Webhook setup (5 minutes)

1. In Discord: **Server Settings → Integrations → Webhooks → New Webhook**.
2. Pick the channel (suggest `#general` or a read-only `#updates`), copy the webhook URL.
3. Keep the URL secret — anyone with it can post to that channel.
4. Store it in an environment variable on your machine, not in a file:

```powershell
$env:DISCORD_WEBHOOK_URL = "https://discord.com/api/webhooks/..."
```

5. Post an update:

```powershell
.\Docs\Discord_Post.ps1 -Message "Week 1: repo + LFS done, look test pending, Fermin deciding D01-D06."
```

Optional convenience: paste a prepared update from `00_START_HERE.md` or this week's Trello cards.

## Suggested rhythm

- Post after each work block with me: **what changed, what's next, who is blocked**.
- Keep it short — 3 lines. Long docs stay in the Docs folder and get pinned as links.
- Never post secrets, API keys, or private client data. The script has no sanitising built in.
