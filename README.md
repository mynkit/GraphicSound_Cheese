# チーズ工場 For Raspberry Pi

# Setup SuperCollider

Set `~/.jackdrc`

```sh
/bin/bash update_jackdrc.sh
```

Set SuperCollider startup file.

```sh
rm ~/.config/SuperCollider/startup.scd
ln -s ${PWD}/startup.scd ~/.config/SuperCollider/startup.scd
```

# How to run SuperCollider

```sh
tmux new-session -d -s sc-session 'sclang'
```

kill command

```sh
tmux kill-session -t sc-session
```

## add addon

```sh
git clone git@github.com:Lacty/ofxJoystick.git ../../../addons/ofxJoystick
```

# How to run oF

```sh
make
make run
```
