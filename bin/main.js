import {Game, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;

const player = {
    x: 0,
    y: 0,
    speed: 100,
    spr: null,
    sprOptions:{
        width: 24*4,
        height: 24*4,
        sw:24,
        sh:24,
    }
}
Game.init = ()=>{
    player.spr = Texture.fromFile('characters_packed.png');
}

Game.update = (dt)=>{
    if(Input.keyIsDown(256)) Game.quit();
    if(Input.keyIsDown(262)) player.x += player.speed * dt;
    if(Input.keyIsDown(263)) player.x -= player.speed * dt;
    if(Input.joyButtonIsDown(0,12)) player.x += player.speed * dt;
    if(Input.joyButtonIsDown(0,14)) player.x -= player.speed * dt;
}

Game.draw = ()=>{
    player.spr.draw(player.x,player.y, player.sprOptions);
}