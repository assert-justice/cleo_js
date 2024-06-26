import {Engine, System, Input, Window, Graphics, Audio} from 'cleo';

let playerSpr, backgroundSpr;

const player = {
    x: 0,
    y: 0,
    speed: 100,
}

let fullscreen = false;
let wasToggleDown = false;
Window.setStats("test", 1920, 1080, "windowed");

const vertSrc = `#version 330 core
in vec3 aPos;
in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 camera;
uniform mat4 sprite;
uniform mat4 coord;
void main()
{
    gl_Position = camera * sprite * vec4(aPos, 1.0);
    TexCoord = (coord * vec4(aTexCoord, 1.0, 1.0)).xy;
}`;

const fragSrc = `#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main()
{
    FragColor = texture(ourTexture, TexCoord);
}`;

const quad = [
    1.0, 1.0, 0.0,     1.0, 1.0, // top right
    1.0, 0.0, 0.0,     1.0, 0.0, // bottom right
    0.0, 1.0, 0.0,     0.0, 1.0, // top left

    1.0, 0.0, 0.0,     1.0, 0.0, // bottom right
    0.0, 1.0, 0.0,     0.0, 1.0, // top left
    0.0, 0.0, 0.0,     0.0, 0.0, // bottom left
];

let audio;
Engine.init = ()=>{
    playerSpr = Graphics.Texture.fromFile('character_0000.png');
    backgroundSpr = Graphics.Texture.new(1920/4, 1080/4);
    Graphics.pushRenderTarget(backgroundSpr);
    Graphics.clear();
    playerSpr.draw(player.x, player.y);
    playerSpr.draw(player.x+100, player.y);
    playerSpr.draw(player.x+200, player.y);
    Graphics.popRenderTarget();
    // const shader = Graphics.Shader.new(vertSrc, fragSrc);
    // const mesh = Graphics.Mesh.new(shader, 6, 5, [[3, "aPos"], [2, "aTexCoord"]], quad);
    // Graphics.saveTexture("test.png", backgroundSpr);
    audio = Audio.Sound.fromFile("boss.mp3");
    audio.play();
}

Engine.update = (dt)=>{
    if(!audio.isPlaying) Engine.quit();
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

Engine.draw = (dt)=>{
    // System.println(dt)
    // Graphics.pushRenderTarget(backgroundSpr);
    // Graphics.clear();
    // playerSpr.draw(player.x, player.y);
    // Graphics.popRenderTarget();
    backgroundSpr.draw(0,0);
}