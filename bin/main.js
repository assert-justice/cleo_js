import {Engine, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;

const player = {
    x: 48,
    y: 48,
    speed: 100,
    spr: null,
    sprOptions:{
        width: 24*4,
        height: 24*4,
        sw:24,
        sh:24,
        ox: 12,
        oy: 12,
        angle: 180,
        visible: true,
    }
}
let fb = null;
Engine.init = ()=>{
    // Input.keyCallback = (keyCode, actionType)=>{
    //     System.println("code:", keyCode, "type:", actionType);
    // }
    // fb = Graphics.Texture.new(100, 100);
    // System.println(Graphics.getTransform());
    const width = 10;
    const height = 10;
    const buffer = new ArrayBuffer(width * height * 4);
    const view = new Uint8Array(buffer);
    for(let x = 0; x < width; x++){
        for(let y = 0; y < height; y++){
            const idx = (x * width + y)*4;
            view[idx] = x === height - y - 1 ? 255 : 0;
            view[idx + 1] = 255;
            view[idx + 2] = 255;
            view[idx + 3] = 255;
        }
    }
    // fb.setTarget();
    // player.spr = Texture.fromColor(width, height, 255, 0, 0, 255);
    fb = Graphics.Texture.fromColor(100, 100, 0, 255, 0, 255);
    player.spr = Texture.new(width, height, view.buffer);
    const temp = Texture.fromColor(10, 10, 255, 0, 0, 255);
    Graphics.pushRenderTarget(player.spr);
    Graphics.pushTransform();
    Graphics.scale(0.5, 0.5);
    temp.draw(0,0);
    Graphics.popTransform();
    Graphics.popRenderTarget();
    Graphics.pushRenderTarget(fb);
    for(let x = 0; x < 10; x++){
        for(let y = 0; y < 10; y++){
            player.spr.draw(x * width, y * height);
        }
    }
    Graphics.popRenderTarget();
    // Graphics.translate(30, 0, 0);
    // Graphics.scale(2, 2);
    // Graphics.rotate(Math.PI/4);
    // fb.resetTarget();
    // player.spr = Texture.fromArray(width, height, texData);
    // player.spr = Texture.fromFile('characters_packed.png');
}

Engine.update = (dt)=>{
    if(Input.keyIsDown(256)) Engine.quit();
    if(Input.keyIsDown(32)){
        // globalThis.setTimeout(()=>System.println('yo'), 500);
        // System.println(OS);
        for (const [key, value] of Object.entries(globalThis)) {
            System.println(key, value);
        }
    }
    // if(Input.keyIsDown(262)) player.x += player.speed * dt;
    // if(Input.keyIsDown(263)) player.x -= player.speed * dt;
    // if(Input.joyButtonIsDown(0,12)) player.x += player.speed * dt;
    // if(Input.joyButtonIsDown(0,14)) player.x -= player.speed * dt;
}

Engine.draw = ()=>{
    // Graphics.pushRenderTarget(fb);
    // for(let x = 0; x < 10; x++){
    //     for(let y = 0; y < 10; y++){
    //         player.spr.draw(x * 10, y * 10);
    //     }
    // }
    // Graphics.popRenderTarget();
    fb.draw(0, 0);
    // player.spr.draw(0, 0);
    // player.spr.draw(0, 0,{width: 100, height: 100});
    // player.spr.draw(player.x,player.y, player.sprOptions);
}