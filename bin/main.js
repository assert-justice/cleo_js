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
Engine.init = ()=>{
    Input.keyCallback = (keyCode, actionType)=>{
        System.println("code:", keyCode, "type:", actionType);
    }
    const width = 10;
    const height = 10;
    const buffer = new ArrayBuffer(width * height * 4);
    const view = new Uint8Array(buffer);
    for(let x = 0; x < width; x++){
        for(let y = 0; y < height; y++){
            const idx = (x * width + y)*4;
            view[idx] = x === height - y - 1 ? 255 : 0;
            view[idx + 1] = 0;
            view[idx + 2] = 0;
            view[idx + 3] = 255;
        }
    }
    // player.spr = Texture.fromColor(width, height, 255, 0, 0, 255);
    player.spr = Texture.new(width, height, view.buffer);
    // player.spr = Texture.fromArray(width, height, texData);
    // player.spr = Texture.fromFile('characters_packed.png');
}

Engine.update = (dt)=>{
    if(Input.keyIsDown(256)) Engine.quit();
    // if(Input.keyIsDown(262)) player.x += player.speed * dt;
    // if(Input.keyIsDown(263)) player.x -= player.speed * dt;
    // if(Input.joyButtonIsDown(0,12)) player.x += player.speed * dt;
    // if(Input.joyButtonIsDown(0,14)) player.x -= player.speed * dt;
}

Engine.draw = ()=>{
    player.spr.draw(0, 0,{width: 100, height: 100});
    // player.spr.draw(player.x,player.y, player.sprOptions);
}