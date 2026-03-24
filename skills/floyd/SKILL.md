---
name: floyd
description: Play an audio file (.mp3, .wav, or .flac) using the floyd command line player. Use when the user wants to play, listen to, or preview an audio file.
argument-hint: <file> | stop
allowed-tools: Bash(floyd *), Bash(kill *), Bash(echo *), Bash(cat *), Bash(rm *), Bash(curl *), Bash(chmod *), Bash(uname *), Bash(which *)
---

Play or stop audio using the floyd command line player.

Supported formats: `.mp3`, `.wav`, `.flac`

The PID of any running floyd process is stored in `/tmp/floyd.pid`.

## Steps

### Step 1 — Ensure floyd is installed

Check if floyd is on the PATH:

```bash
which floyd
```

If not found, detect the OS and download the appropriate binary from the latest release:

```bash
OS=$(uname -s)
if [ "$OS" = "Darwin" ]; then
    curl -L https://github.com/robrohan/floyd/releases/tag/v1.0.0/floyd-mac -o /usr/local/bin/floyd
else
    curl -L https://github.com/robrohan/floyd/releases/tag/v1.0.0/floyd -o /usr/local/bin/floyd
fi
chmod +x /usr/local/bin/floyd
```

If `/usr/local/bin` is not writable, try `~/.local/bin` instead (create it if needed) and inform the user they may need to add it to their PATH.

---

### If the user wants to stop playback (argument is "stop" or user says stop/pause):

1. Check if `/tmp/floyd.pid` exists. If not, tell the user nothing is playing.
2. Stop the process and clean up:

```bash
kill $(cat /tmp/floyd.pid) && rm /tmp/floyd.pid
```

3. Tell the user playback has stopped.

---

### If the user wants to start playback (argument is a file path):

1. If `$ARGUMENTS` is empty, ask the user which file they want to play.
2. Verify the file exists and has a supported extension (`.mp3`, `.wav`, `.flac`). If not, tell the user and stop.
3. If `/tmp/floyd.pid` exists, stop any currently playing audio first:

```bash
kill $(cat /tmp/floyd.pid) 2>/dev/null; rm -f /tmp/floyd.pid
```

4. Start floyd in headless mode in the background and save the PID:

```bash
floyd -d $ARGUMENTS &
echo $! > /tmp/floyd.pid
```

5. Tell the user what is playing and that they can stop it with `/floyd stop`.
