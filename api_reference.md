# Api Reference

## Game

- init: ()=>{}
- update: (dt:number)=>{}
- quit()

## Graphics

### Texture

- constructor(width: number, height: number)
- fromFile(path: string)
- get width:number
- get height:number
- draw(x: number, y:number, options:any)

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
## System