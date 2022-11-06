# Battleship.js

[![NPM](https://nodei.co/npm/battleshipjs.png)](https://nodei.co/npm/battleshipjs/)

[![Sonarcloud Status](https://sonarcloud.io/api/project_badges/measure?project=berkerol_battleshipjs&metric=alert_status)](https://sonarcloud.io/dashboard?id=berkerol_battleshipjs)
[![CI](https://github.com/berkerol/battleshipjs/actions/workflows/lint.yml/badge.svg?branch=master)](https://github.com/berkerol/battleshipjs/actions/workflows/lint.yml)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg)](https://github.com/berkerol/battleshipjs/issues)
[![semistandard](https://img.shields.io/badge/code%20style-semistandard-brightgreen.svg)](https://github.com/Flet/semistandard)
[![ECMAScript](https://img.shields.io/badge/ECMAScript-latest-brightgreen.svg)](https://www.ecma-international.org/ecma-262)
[![license](https://img.shields.io/badge/license-GNU%20GPL%20v3.0-blue.svg)](https://github.com/berkerol/battleshipjs/blob/master/LICENSE)

A Battleship CLI game with customizations.

## Features

* Set the number of each 5 ship type.
* Set the number of rows and columns in the board.
* Play against computer or against another player in hotseat mode.
* Choose which player plays first.
* Choose whether to end attack turn even if player hits.
* Set names of players.
* Also randomly place your ships and attack opponent's ships.

## Board

* Empty: **O**
* Boat (1 space): **B**
* Frigate (2 spaces): **F**
* Destroyer (3 spaces): **D**
* Cruiser (4 spaces): **C**
* Aircraft carrier (5 spaces): **A**
* Miss: **X**
* Hit: **\***

## Installation

```
$ npm install battleshipjs
```

## Usage

Flag | Short | Description | Usage
-----|-------|---------|------
`--rows` | `-r` | Set number of rows in the board | `-r 15`
`--cols` | `-c` | Set number of cols in the board | `-c 20`
`--multiplayer` | `-m` | Play against another player | `-m`
`--swap` | `-s` | Swap player turns (Player2 plays first) | `-s`
`--end` | `-e` | End attack turn even if player hits | `-e`
`--first` | `-f` | Set name of first player | `-f berk`
`--second` | `-n` | Set name of second player | `-n erol`

## Examples

Example | Description
--------|------------
`$ battleship` | Starts a new game with default settings
`$ battleship -r 15` | Starts a new game with 15 rows
`$ battleship -c 20` | Starts a new game with 20 cols
`$ battleship -r 15 -c 20` | Starts a new game with 15 rows and 20 cols
`$ battleship -m` | Starts a new game in mutliplayer hotseat mode
`$ battleship -s` | Starts a new game in which Player2 plays first
`$ battleship -e` | Starts a new game which ends attack turn even if player hits
`$ battleship -f berk` | Starts a new game with name of first player
`$ battleship -n erol` | Starts a new game with name of second player
`$ battleship -f berk -n erol` | Starts a new game with names of first and second players
`$ battleship -m -f berk` | Starts a new game in multiplayer with name of first player

## Continous Integration

It is setup using GitHub Actions in `.github/workflows/lint.yml`

## Contribution

Feel free to [contribute](https://github.com/berkerol/battleshipjs/issues) according to the [semistandard rules](https://github.com/Flet/semistandard) and [latest ECMAScript Specification](https://www.ecma-international.org/ecma-262).

## Distribution

You can distribute this software freely under [GNU GPL v3.0](https://github.com/berkerol/battleshipjs/blob/master/LICENSE).
