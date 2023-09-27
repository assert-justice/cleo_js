import {Game, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;

let tex;
Game.init = ()=>{
    // System.println(''+Texture);
    // tex = Texture.fromFile('characters_packed.png');
    tex = Texture.new(200,100);
    System.println(''+tex.width);
    System.println(''+tex.height);
    // System.println(''+Window.width);
    // System.println(''+Window.height);
    // Graphics.setClearColor(1,0,0);
    // Graphics.clear();
}

Game.update = ()=>{
    if(Input.getKey(256)) Game.quit();
    tex.draw(0, 0);
}