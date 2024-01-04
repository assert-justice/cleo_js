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
    const width = 10;
    const height = 10;
    const texData = [];
    for(let i = 0; i < width * height; i++){
        texData.push(i % 3 === 0 ? 255 : 0);
        texData.push(0);
        texData.push(0);
        texData.push(255);
    }
    player.spr = Texture.new(width, height, texData);
    // player.spr = Texture.fromFile('characters_packed.png');
}

Engine.update = (dt)=>{
    if(Input.keyIsDown(256)) Engine.quit();
    if(Input.keyIsDown(262)) player.x += player.speed * dt;
    if(Input.keyIsDown(263)) player.x -= player.speed * dt;
    if(Input.joyButtonIsDown(0,12)) player.x += player.speed * dt;
    if(Input.joyButtonIsDown(0,14)) player.x -= player.speed * dt;
}

Engine.draw = ()=>{
    player.spr.draw(0, 0,{width: 100, height: 100});
    // player.spr.draw(player.x,player.y, player.sprOptions);
}