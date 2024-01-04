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
            setTarget():void;
            resetTarget():void;
        }
    }
    export class Window{
        static get width():number;
        static get height():number;
        static setStats(name: string, width:number, height:number, mode?: string, monitor?: number)
        static vsync: boolean;

    }
    export class Input{
        static keyIsDown(keyCode: number): boolean;
        static mouseButtonIsDown(mouseButtonCode: number): boolean;
        static get mouseX(): number;
        static get mouseY(): number;
        static joyButtonIsDown(joyIdx: number, buttonCode: number): boolean;
        static joyGetAxis(joyIdx: number, axisCode: number): number;
        static gamepadExists(joyIdx: number): boolean;
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