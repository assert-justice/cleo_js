import {Game, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;

let tex;
let spr,spr2;
Game.init = ()=>{
    // System.println(''+Texture);
    spr = Texture.fromFile('character_0000.png');
    spr2 = Texture.fromFile('character_0000.png');
    spr.scaleX = 4;
    spr.originX = 12;
    spr.originY = 12;
    spr.angle = 30;
    tex = Texture.new(400,200);
    System.println('tex width:', tex.width);
    System.println('tex height:', tex.height);
    // tex.setTarget();
    // spr.draw(100,100);
    // tex.resetTarget();
    // System.println(''+Window.width);
    // System.println(''+Window.height);
    // Graphics.setClearColor(1,0,0);
    // Graphics.clear();
}

Game.update = ()=>{
    if(Input.getKey(256)) Game.quit();
    spr.draw(100, 100);
    spr2.draw(100, 100);
}