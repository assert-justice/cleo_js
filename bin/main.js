import {Engine, System, Input, Window, Graphics} from 'cleo';

let playerSpr, backgroundSpr;

const player = {
    x: 0,
    y: 0,
    speed: 100,
}

Engine.init = ()=>{
    playerSpr = Graphics.Texture.fromFile('character_0000.png');
    backgroundSpr = Graphics.Texture.new(1920/4, 1080/4);
}

Engine.update = (dt)=>{
    if(Input.keyIsDown(256)) Engine.quit();
    if(Input.keyIsDown(262)) player.x += player.speed * dt;
    if(Input.keyIsDown(263)) player.x -= player.speed * dt;
    // if(Input.joyButtonIsDown(0,12)) player.x += player.speed * dt;
    // if(Input.joyButtonIsDown(0,14)) player.x -= player.speed * dt;
}

Engine.draw = ()=>{
    Graphics.pushRenderTarget(backgroundSpr);
    Graphics.clear();
    playerSpr.draw(player.x, player.y);
    Graphics.popRenderTarget();
    backgroundSpr.draw(0,0);
}