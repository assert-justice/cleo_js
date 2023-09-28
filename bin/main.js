import {Game, System, Input, Window, Graphics} from 'cleo';
const {Texture} = Graphics;

Window.setStats("hi", 300, 200);

let tex;
let spaceDown = false;
let lastSpaceDown = false;
Game.init = ()=>{
    tex = Texture.fromFile('characters_packed.png');
}

Game.update = (dt)=>{
    if(Input.getKey(256)) Game.quit();
    lastSpaceDown = spaceDown;
    spaceDown = Input.getKey(32);
    if(spaceDown && !lastSpaceDown){
        System.println(1/dt);
        // Window.setStats("yo", 600, 400, "borderless");
    }
}

Game.draw = ()=>{
    tex.draw(100,100);
}