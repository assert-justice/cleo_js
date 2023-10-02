# Api Reference

## Game

- init: ()=>{}
- update: (dt:number)=>{}
- draw: ()=>{}
- quit()

## Graphics

- setClearColor(r: number, g: number, b: number, a: number = 0)
- clear()

### Texture

- constructor(width: number, height: number)
- fromFile(path: string)
- get width:number
- get height:number
- draw(x: number, y:number, options:any)
- setTarget()
- resetTarget()
- new(with: number, height: number)

## Window

- get width:number
- get height:number
- setStats(name: string, width:number, height:number, mode: string, monitor: number)
- get set vsync: bool

## Input

- keyIsDown(keyCode: number): boolean
- mouseButtonIsDown(mouseButtonCode: number): bool
- get mouseX: number
- get mouseY: number
- joyButtonIsDown(joyIdx, buttonCode: number): bool
- joyGetAxis(joyIdx, axisCode: number): bool

## Audio

### Sound

- static fromFile(path: string)
- play()
- pause()
- stop()
- get isPlaying: boolean
- get set volume: number
- get set isLooping: boolean

## System

- println(...string[])

### Relatively Safe

- readFile(path: string): string
- writeFile(path: string, text: string)

### Annoying but doable

- pathExists(path: string): bool
- pathIsFile(path: string): bool
- pathIsDir(path: string): bool

### Yikes

- makeDir(path: string)
- copy(sourcePath: string, destPath: string, recursive: bool = true)
