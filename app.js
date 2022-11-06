#!/usr/bin/env node

const readline = require('readline')
  .createInterface({ input: process.stdin, output: process.stdout });

const table = require('tty-table');
const yargs = require('yargs');

const argv = yargs
  .options({
    r: {
      alias: 'rows',
      describe: 'Number of rows in the board',
      type: 'number',
      default: 10
    },
    c: {
      alias: 'cols',
      describe: 'Number of columns in the board',
      type: 'number',
      default: 10
    },
    m: {
      alias: 'multiplayer',
      describe: 'Play against another player in hotseat mode',
      type: 'boolean'
    },
    s: {
      alias: 'swap',
      describe: 'Swap player turns (Computer/Player2 plays first)',
      type: 'boolean'
    },
    e: {
      alias: 'end',
      describe: 'End attack turn even if player hits',
      type: 'boolean'
    },
    f: {
      alias: 'first',
      describe: 'Name of first player',
      type: 'string'
    },
    n: {
      alias: 'second',
      describe: 'Name of second player',
      type: 'string'
    }
  })
  .help()
  .alias('help', 'h')
  .version()
  .alias('version', 'v')
  .argv;

const boardChars = ['O', 'B', 'F', 'D', 'C', 'A', 'X', '*'];

const board1 = [];
const board2 = [];

let rows;
let cols;

const getRandom = (lower, upper) => {
  return Math.floor(lower + Math.random() * (upper - lower + 1));
};

const getPosition = position => {
  const letter = position.charCodeAt(0);
  if (letter >= 97 && letter <= 122 && letter - 97 < rows) {
    const digit = parseInt(position.slice(1));
    if (!isNaN(digit) && digit - 1 < cols) {
      return [letter - 97, digit - 1];
    }
  }
  return [];
};

const askQuestion = q => {
  return new Promise((resolve, reject) => {
    readline.question(q + ' ', answer => {
      resolve(answer.toLowerCase());
    });
  });
};

const askNumber = async q => {
  let number;
  do {
    number = parseInt(await askQuestion(q));
  } while (isNaN(number));
  return number;
};

const askPosition = async () => {
  let position;
  do {
    position = getPosition(await askQuestion('Enter the position (ex: A1, b2).'));
  } while (position.length === 0);
  return position;
};

const askPositionWithRandom = async () => {
  let position;
  do {
    const input = await askQuestion('Enter the position (ex: A1, b2) or type r for random.');
    if (input === 'r') {
      return [];
    }
    position = getPosition(input);
  } while (position.length === 0);
  return position;
};

const printBoard = (player1, board1, player2, board2, boardShow2) => {
  const prettyBoard = [];
  for (let i = 0; i < rows; i++) {
    const row = [String.fromCharCode(65 + i)];
    for (let j = 0; j < cols; j++) {
      row.push(boardChars[board1[i][j]]);
    }
    if (board2 !== undefined) {
      row.push('');
      row.push(String.fromCharCode(65 + i));
      for (let j = 0; j < cols; j++) {
        const unit = board2[i][j];
        if (boardShow2) {
          row.push(boardChars[unit]);
        } else {
          if (unit === 0 || unit === 6 || unit === 7) {
            row.push(boardChars[unit]);
          } else {
            row.push(boardChars[0]);
          }
        }
      }
    }
    prettyBoard.push(row);
  }
  const header = [{ value: player1, color: 'yellow' }];
  for (let i = 1; i <= cols; i++) {
    header.push({ value: i });
  }
  if (player2 !== undefined) {
    header.push(0);
    header.push({ value: player2, color: 'yellow' });
    for (let i = 1; i <= cols; i++) {
      header.push({ value: i });
    }
  }
  console.log(table(header, prettyBoard).render());
};

const controlBoard = board => {
  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols; j++) {
      const unit = board[i][j];
      if (unit !== 0 && unit !== 6 && unit !== 7) {
        return true;
      }
    }
  }
  return false;
};

const controlBoardAdding = (board, shipSize) => {
  let count = 0;
  let ver = 0;
  let hor = 0;
  for (let i = 0; i < rows; i++) {
    count = 0;
    for (let j = 0; j < cols; j++) {
      if (board[i][j] === 0) {
        count++;
      }
      if (count === shipSize) {
        hor++;
        count = 0;
      }
    }
  }
  for (let i = 0; i < cols; i++) {
    count = 0;
    for (let j = 0; j < rows; j++) {
      if (board[j][i] === 0) {
        count++;
      }
      if (count === shipSize) {
        ver++;
        count = 0;
      }
    }
  }
  return Math.max(hor, ver);
};

const addShip = (player, board, shipIndex, shipSize, shipName, begRow, endRow, begCol, endCol) => {
  const shipDigit = boardChars.indexOf(shipName.slice(0, 1));
  let counter = 0;
  if (begRow === endRow) {
    for (let i = begCol; i <= endCol; i++) {
      if (board[begRow][i] === 0) {
        counter++;
      }
    }
    if (counter === shipSize) {
      for (let i = begCol; i <= endCol; i++) {
        board[begRow][i] = shipDigit;
      }
      console.log(`${player} placed ${shipIndex}. ${shipName}.`);
      return true;
    }
  } else if (begCol === endCol) {
    for (let i = begRow; i <= endRow; i++) {
      if (board[i][begCol] === 0) {
        counter++;
      }
    }
    if (counter === shipSize) {
      for (let i = begRow; i <= endRow; i++) {
        board[i][begCol] = shipDigit;
      }
      console.log(`${player} placed ${shipIndex}. ${shipName}.`);
      return true;
    }
  }
  return false;
};

const addShipRandom = (player, board, shipIndex, shipSize, shipName) => {
  if (player === 'Computer') {
    console.log(`${player} is placing ${shipIndex}. ${shipName}.`);
  }
  let begRow;
  let endRow;
  let begCol;
  let endCol;
  do {
    if (Math.random() < 0) {
      begRow = getRandom(0, rows - 1);
      endRow = begRow;
      begCol = getRandom(0, cols - shipSize);
      endCol = begCol + shipSize - 1;
    } else {
      begCol = getRandom(0, cols - 1);
      endCol = begCol;
      begRow = getRandom(0, rows - shipSize);
      endRow = begRow + shipSize - 1;
    }
  } while (!addShip(player, board, shipIndex, shipSize, shipName, begRow, endRow, begCol, endCol));
};

const addShipUser = async (player, board, shipIndex, shipSize, shipName) => {
  console.log(`${player} is placing ${shipIndex}. ${shipName}.`);
  printBoard(player, board);
  let position = await askPositionWithRandom();
  if (position.length === 0) {
    addShipRandom(player, board, shipIndex, shipSize, shipName);
  } else {
    let begRow = position[0];
    let endRow;
    let begCol;
    let endCol;
    while (true) {
      if (shipSize === 1) {
        endRow = begRow;
        endCol = begCol;
      } else {
        const orientation = (await askQuestion('Enter orientation (h for horizontal or v for vertical).')).slice(0, 1);
        if (orientation === 'h') {
          endRow = begRow;
          endCol = begCol + shipSize - 1;
        } else {
          endCol = begCol;
          endRow = begRow + shipSize - 1;
        }
      }
      if (addShip(player, board, shipIndex, shipSize, shipName, begRow, endRow, begCol, endCol)) {
        break;
      }
      console.log('This space is not suitable for this ship, try again.');
      position = await askPosition();
      begRow = position[0];
      begCol = position[1];
    }
  }
};

const addShipMain = async (firstPlay, singlePlayer, player1, player2, shipSize, shipName) => {
  if (rows >= shipSize || cols >= shipSize) {
    const maxShips = Math.min(controlBoardAdding(board1, shipSize), controlBoardAdding(board2, shipSize));
    if (maxShips > 0) {
      const ships = await askNumber(`How many ${shipName}s (${shipSize} spaces) do you want?`);
      if (ships > 0) {
        for (let i = 1; i <= ships; i++) {
          if (controlBoardAdding(board1, shipSize) < 1 || controlBoardAdding(board2, shipSize) < 1) {
            console.log(`There is no space left in this board for ${shipName}.`);
            break;
          }
          if (firstPlay) {
            await addShipUser(player1, board1, i, shipSize, shipName);
          }
          if (singlePlayer) {
            addShipRandom(player2, board2, i, shipSize, shipName);
          } else {
            await addShipUser(player2, board2, i, shipSize, shipName);
          }
          if (!firstPlay) {
            await addShipUser(player1, board1, i, shipSize, shipName);
          }
        }
      }
    } else {
      console.log(`There is no space left in this board for ${shipName}.`);
    }
  } else {
    console.log(`${shipName} is too big for this board.`);
  }
};

const attackShip = (player, board, row, col) => {
  if (board[row][col] !== 0) {
    board[row][col] = 7;
    console.log(`${player} hit.`);
    return true;
  } else {
    board[row][col] = 6;
    console.log(`${player} could not hit.`);
    return false;
  }
};

const attackShipRandom = (player, board) => {
  if (player === 'Computer') {
    console.log(`${player} is attacking.`);
  }
  let row;
  let col;
  do {
    row = getRandom(0, rows - 1);
    col = getRandom(0, cols - 1);
  } while (board[row][col] === 6 || board[row][col] === 7);
  return attackShip(player, board, row, col);
};

const attackShipUser = async (player1, player2, selfBoard, board) => {
  console.log(`${player1} is attacking.`);
  printBoard(player1, selfBoard, player2, board, false);
  let position = await askPositionWithRandom();
  if (position.length === 0) {
    return attackShipRandom(player1, board);
  } else {
    let row = position[0];
    let col = position[1];
    while (board[row][col] === 6 || board[row][col] === 7) {
      console.log('This space is already hit before, try again.');
      position = await askPosition();
      row = position[0];
      col = position[1];
    }
    return attackShip(player1, board, row, col);
  }
};

const main = async () => {
  rows = argv.r;
  cols = argv.c;
  let singlePlayer = true;
  if (argv.m) {
    singlePlayer = false;
  }
  let firstPlay = true;
  if (argv.s) {
    firstPlay = false;
  }
  let continueAttack = true;
  if (argv.e) {
    continueAttack = false;
  }
  let player1;
  let player2;
  if (singlePlayer) {
    player1 = 'Player';
    player2 = 'Computer';
  } else {
    player1 = 'Player 1';
    player2 = 'Player 2';
  }
  if (argv.f) {
    player1 = argv.f;
  }
  if (argv.n) {
    player2 = argv.n;
  }
  for (let i = 0; i < rows; i++) {
    const row = [];
    for (let j = 0; j < cols; j++) {
      row.push(0);
    }
    board1.push(row);
    board2.push(row.slice(0));
  }
  do {
    await addShipMain(firstPlay, singlePlayer, player1, player2, 5, 'Aircraft Carrier');
    await addShipMain(firstPlay, singlePlayer, player1, player2, 4, 'Cruiser');
    await addShipMain(firstPlay, singlePlayer, player1, player2, 3, 'Destroyer');
    await addShipMain(firstPlay, singlePlayer, player1, player2, 2, 'Frigate');
    await addShipMain(firstPlay, singlePlayer, player1, player2, 1, 'Boat');
  } while (!controlBoard(board1));
  while (controlBoard(board1) && controlBoard(board2)) {
    if (firstPlay) {
      while (controlBoard(board1) && controlBoard(board2) && await attackShipUser(player1, player2, board1, board2) && continueAttack); // eslint-disable-line no-unmodified-loop-condition
    }
    if (singlePlayer) {
      while (controlBoard(board1) && controlBoard(board2) && await attackShipRandom(player2, board1) && continueAttack); // eslint-disable-line no-unmodified-loop-condition
    } else {
      while (controlBoard(board1) && controlBoard(board2) && await attackShipUser(player2, player1, board2, board1) && continueAttack); // eslint-disable-line no-unmodified-loop-condition
    }
    if (!firstPlay) {
      while (controlBoard(board1) && controlBoard(board2) && await attackShipUser(player1, player2, board1, board2) && continueAttack); // eslint-disable-line no-unmodified-loop-condition
    }
  }
  console.log('Game over.');
  if (controlBoard(board1)) {
    console.log(`${player1} won!`);
  } else {
    console.log(`${player2} won!`);
  }
  printBoard(player1, board1, player2, board2, true);
  await askQuestion('Enter anything to exit.');
  readline.close();
};

main();
