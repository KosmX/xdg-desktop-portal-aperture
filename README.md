# XDG Desktop Portal Aperture

## UNFINISHED
This portal is not finished yet, I'm working on it, but it's not functional yet.

---

This is a work-in progress XDG-DESKTOP-PORTAL implementation for settings and style.
The only purpose of this is a desktop-environment independent settings portal.

It does not and will not provide any other functionality. For that, please use an existing one like KDE or GTK.

## Dark Mode, XDG portal settings
KDE doesn't have a way of telling whether the user has dark mode enabled or not, every KDE app makes that based on the window color.
FreeDesktopPortal has a standard for this.

### FreeDesktop accent color
[general]

### FreeDesktop color scheme
First, setting has
[general]
colorScheme = true/false
```

if not, the KDE window color will be used to determine color mode.
I use the same condition as KDE, it should behave exactly the same.
