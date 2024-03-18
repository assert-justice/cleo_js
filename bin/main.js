import {Engine, System, Input, Window, Graphics} from 'cleo';

let playerSpr, backgroundSpr;

const player = {
    x: 0,
    y: 0,
    speed: 100,
}

let fullscreen = false;
let wasToggleDown = false;
Window.setStats("test", 1920, 1080, "windowed");

Engine.init = ()=>{
    playerSpr = Graphics.Texture.fromFile('character_0000.png');
    backgroundSpr = Graphics.Texture.new(1920/4, 1080/4);
    Graphics.pushRenderTarget(backgroundSpr);
    Graphics.clear();
    playerSpr.draw(player.x, player.y);
    playerSpr.draw(player.x+100, player.y);
    playerSpr.draw(player.x+200, player.y);
    Graphics.popRenderTarget();
    Graphics.saveTexture("test.png", backgroundSpr);
}

Engine.update = (dt)=>{
    if(!wasToggleDown && Input.keyIsDown(257)){
        fullscreen = !fullscreen;
        Window.setStats("test", 1920, 1080, fullscreen ? "borderless": "windowed");
    }
    wasToggleDown = Input.keyIsDown(257);
    // if(!fullscreen){
    //     fullscreen = true;
    //     Window.setStats("test", 1920, 1080, "borderless");
    // }
    if(Input.keyIsDown(256)) Engine.quit();
    if(Input.keyIsDown(262)) player.x += player.speed * dt;
    if(Input.keyIsDown(263)) player.x -= player.speed * dt;
    // if(Input.joyButtonIsDown(0,12)) player.x += player.speed * dt;
    // if(Input.joyButtonIsDown(0,14)) player.x -= player.speed * dt;
}

Engine.draw = ()=>{
    // Graphics.pushRenderTarget(backgroundSpr);
    // Graphics.clear();
    // playerSpr.draw(player.x, player.y);
    // Graphics.popRenderTarget();
    backgroundSpr.draw(0,0);
}