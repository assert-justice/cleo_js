import {Game, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;

let tex;
let spr;
Game.init = ()=>{
    // System.println(''+Texture);
    spr = Texture.fromFile('character_0000.png');
    tex = Texture.new(200,100);
    System.println('tex width:', tex.width);
    System.println('tex height:', tex.height);
    tex.setTarget();
    spr.draw(0,0);
    tex.resetTarget();
    // System.println(''+Window.width);
    // System.println(''+Window.height);
    // Graphics.setClearColor(1,0,0);
    // Graphics.clear();
}

Game.update = ()=>{
    if(Input.getKey(256)) Game.quit();
    tex.draw(0, 0);
}