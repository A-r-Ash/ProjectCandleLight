param(
  [Parameter(Mandatory = $true)]
  [string]$Message,

  [string]$WebhookUrl = $env:DISCORD_WEBHOOK_URL
)

if (-not $WebhookUrl) {
  throw "Set DISCORD_WEBHOOK_URL or pass -WebhookUrl. See Docs/Discord_Updates.md."
}

$payload = @{ content = $Message } | ConvertTo-Json -Depth 3

Invoke-RestMethod `
  -Uri $WebhookUrl `
  -Method Post `
  -ContentType "application/json" `
  -Body $payload | Out-Null

"Posted to Discord."
