# Api Reference

## Game

- init: ()=>{}
- update: (dt:number)=>{}
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
- set(name: string, width:number, height:number, mode: string, monitor: number)

## Input

- {key_name}: number
- getKey(code: number): boolean

### Mouse

- get x: number
- get y: number
- get set mode: string

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