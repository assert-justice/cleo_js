import{setInit,setUpdate} from 'cleo';
// import{println} from 'cleo';
import {setClearColor, loadImage, drawImage, setCameraPosition, setRenderTarget} from 'cleo-renderer';
import {getKey} from 'cleo-input';
import {quit} from 'cleo-window'
import {message} from './greet.js';
// println('yo');
const player = {
    x: 0,
    y: 0,
    speed: 100,
};
let spriteSheet = 0;
let spr = 0;
setInit(()=>{
    // setClearColor(0,0,1);
    spriteSheet = loadImage('./characters_packed.png');
    spr = loadImage('./character_0000.png');
    setRenderTarget(spriteSheet);
    drawImage(spr, 24, 24*2, 24*2, 24*2);
    setRenderTarget();
    // spr.width = 10;
    // println('width ' + spr.width);
    // println('height ' + spr.height);
    // println(JSON.stringify({test:"hope"}));
});
setUpdate((dt)=>{
    if(getKey(256)) quit();
    // println('sup');
    if(getKey(262)) player.x += player.speed * dt;
    if(getKey(263)) player.x -= player.speed * dt;
    // setCameraPosition(50, 50);
    drawImage(spriteSheet, 0, 0);
    // drawImage(spriteSheet, player.x, player.y, 24*4, 24*4, 0, 0, 24, 24);
    // drawImage(spriteSheet, 24*4, 0, 24*4, 24*4, 24*2, 0, 24, 24);
});
