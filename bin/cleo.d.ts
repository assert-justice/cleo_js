declare module "cleo" {
    export namespace System {
        function println(...args:any[]):void;
        function readFile(path:string):string;
        function writeFile(path:string,text:string):void;
    }
    export class Engine{
        static init: ()=>void;
        static update: (dt:number)=>void;
        static draw: ()=>void;
        static quit():void;
    }
    export namespace Graphics{
        function setClearColor(r: number, g: number, b: number, a:number): void;
        function clear():void;
        function pushRenderTarget(target: Texture): void;
        function popRenderTarget(): void;
        interface TextureParams{
            width?: number;
            height?: number;
            sx?: number;
            sy?: number;
            sw?: number;
            sh?: number;
            ox?: number;
            oy?: number;
            angle?: number;
            visible?: boolean;
        }
        class Texture{
            width:number;
            height:number;
            draw(x:number, y:number, options?: TextureParams): void;
            static fromFile(path: string): Texture;
            static fromArray(width: number, height: number, data: number[]): Texture;
            static fromColor(width: number, height: number, red: number, green: number, blue: number, alpha: number): Texture;
            static new(width: number, height: number, data?: ArrayBuffer): Texture;
            // setTarget():void;
            // resetTarget():void;
        }
    }
    export class Window{
        static get width():number;
        static get height():number;
        static setStats(name: string, width:number, height:number, mode?: string, monitor?: number)
        static vsync: boolean;

    }
    export namespace Input{
        interface InputEvent{
            type: 'key' | 'mouseMotion' | 'mouseButton' | 'joyButton' | 'joyMotion',
            device: number,
            code: number,
            altCode: number,
            strength: number,
            altStrength: number,
            action: string,
        }
        function inputCallback(callbackFn: (event: InputEvent)=>void)
        function keyCallback(callbackFn: (keyCode: number, actionType: 'press' | 'release')=>void): void;
        function keyIsDown(keyCode: number): boolean;
        function mouseButtonIsDown(mouseButtonCode: number): boolean;
        const mouseX: number;
        const mouseY: number;
        function joyButtonIsDown(joyIdx: number, buttonCode: number): boolean;
        function joyGetAxis(joyIdx: number, axisCode: number): number;
        function gamepadExists(joyIdx: number): boolean;
    }
    export namespace Audio{
        class Sound{
            static fromFile(path: string);
            play();
            pause();
            stop();
            get isPlaying(): boolean;
            volume: number;
            isLooping: boolean;
        }
    }
}