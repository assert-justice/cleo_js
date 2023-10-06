import {Game, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;
import {message} from './greet';

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
Game.init = ()=>{
    // System.println(message);
    // System.writeFile('./message.txt', message);
    // const text = System.readFile('./vert.glsl');
    // System.println(text);
    player.spr = Texture.fromFile('characters_packed.png');
    System.println(Input.gamepadExists(0));
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