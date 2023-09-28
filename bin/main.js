import {Game, System, Input, Window, Graphics, Audio} from 'cleo';
const {Texture, drawSprite} = Graphics;
const {Sound} = Audio;

let tex;
let spr,spr2;
let sound;
let playing = 0;
Game.init = ()=>{
    sound = Sound.fromFile('Jump__001.wav');
    sound.play();
    // System.println('loop', sound.isLooping)
    // sound.isLooping = true;

    tex = Texture.fromFile('characters_packed.png');
    // tex.sx = 24*2;
    // tex.sy = 24;
    // tex.sw = 24;
    // tex.sh = 24;
    // System.println(tex.width, tex.height);
    // drawSprite(tex, 0, 0);
    // spr = new Texture(100, 100);
    // System.println(''+Texture);
    // spr = Texture.fromFile('character_0000.png');
    // spr2 = Texture.fromFile('character_0000.png');
    // spr.scaleX = 4;
    // spr.originX = 12;
    // spr.originY = 12;
    // spr.angle = 30;
    // tex = Texture.new(400,200);
    // System.println('tex width:', tex.width);
    // System.println('tex height:', tex.height);
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
    tex.draw(100,100,{width: 24, height: 24, sx: 24*2, sw: 24, sh: 24, originX: 12, originY: 12, angle: 30});
    tex.draw(100,100,{width: 24, height: 24, sx: 24*2, sw: 24, sh: 24, originX: 0, originY: 0, angle: 0});
    // if(sound.isPlaying) System.println('playing', playing);
    playing++;
    // drawSprite(tex, 0, 0, {});
    // spr.draw(100, 100);
    // spr2.draw(100, 100);
}